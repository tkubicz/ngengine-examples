#include "SimpleImageLoader.h"
#include <NGE/Core/Core.hpp>
#include <NGE/Events/EventManager.hpp>
#include <NGE/Rendering/Rendering.hpp>
#include <NGE/Media/Images/TextureManager.hpp>
#include <NGE/Media/Images/LoadTextureEvent.hpp>
#include <NGE/Media/Images/GetTextureEvent.hpp>
#include <NGE/Media/Shaders/ShaderManager.hpp>
#include <NGE/Media/Shaders/LoadShaderEvent.hpp>
#include <NGE/Media/Shaders/GetShaderEvent.hpp>
#include <NGE/Tools/GLError.hpp>

namespace e = NGE::Events;
namespace i = NGE::Media::Images;
namespace s = NGE::Media::Shaders;

bool SimpleImageLoader::Init() {
	e::EventManager& eventManager = e::EventManager::GetInstance();
	i::TextureManager& textureManager = i::TextureManager::GetInstance();
	s::ShaderManager& shaderManager = s::ShaderManager::GetInstance();

	textureManager.Initialise();
	shaderManager.Initialise();

	std::string imageName = "Lenna";
	std::string shaderName = "shader";

	pugi::xml_document doc;
	auto result = doc.load_file("config/image.xml");
	if (!result) {
		log_error("Could not load image description");
		return false;
	}
	auto imageNode = doc.child("Texture2D");

	auto loadTextureEvent = std::make_shared<i::LoadTextureEvent>(imageNode);
	auto getTextureEvent = std::make_shared<i::GetTextureEvent>(imageName);
	auto loadShaderEvent = std::make_shared<s::LoadShaderEvent>(shaderName, "shader.xml");
	auto getShaderEvent = std::make_shared<s::GetShaderEvent>(shaderName);

	eventManager.QueueEvent(loadTextureEvent);
	eventManager.QueueEvent(loadShaderEvent);

	eventManager.QueueEvent(getTextureEvent);
	eventManager.QueueEvent(getShaderEvent);

	eventManager.UpdateAll();

	if (!getTextureEvent->IsSuccessful()) {
		log_error("Could not get '{}' image", imageName);
		return false;
	}

	if (!getShaderEvent->IsSuccessful()) {
		log_error("Could not get '{}' shader", shaderName);
		return false;
	}

	shader = getShaderEvent->GetShader();
	texture = getTextureEvent->GetTexture();

	CreateGeometry();

	texture->Activate(GL_TEXTURE0);

	return true;
}

void SimpleImageLoader::Render() {
	Rendering::Renderer::GetInstance().ClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	Rendering::Renderer::GetInstance().ClearBuffers();

	shader->BindShader();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	shader->UnbindShader();
}

void SimpleImageLoader::Shutdown() {
}

void SimpleImageLoader::OnResize(int width, int height) {
	Application::OnResize(width, height);
}

void SimpleImageLoader::CreateGeometry() {
	// Setup quad geometry
	vertices[0] = Math::vec2f(0.f, 0.f);
	vertices[1] = Math::vec2f(1.f, 0.f);
	vertices[2] = Math::vec2f(1.f, 1.f);
	vertices[3] = Math::vec2f(0.f, 1.f);

	// Setup quad indices
	GLushort* id = &indices[0];
	*id++ = 0;
	*id++ = 1;
	*id++ = 2;
	*id++ = 0;
	*id++ = 2;
	*id++ = 3;

	glGenVertexArrays(1, &vaoId);
	glGenBuffers(1, &vboVerticesId);
	glGenBuffers(1, &vboIndicesId);

	glBindVertexArray(vaoId);

	glBindBuffer(GL_ARRAY_BUFFER, vboVerticesId);
	glBufferData(GL_ARRAY_BUFFER, sizeof (vertices), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(shader->GetAttribLocation("vVertex"));
	glVertexAttribPointer(shader->GetAttribLocation("vVertex"), 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof (indices), &indices[0], GL_STATIC_DRAW);

	check_gl_error();
}
