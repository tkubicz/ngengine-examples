#include "Background.h"
#include <NGE/Rendering/Rendering.hpp>
#include <NGE/Media/MediaManager.hpp>
#include <NGE/Media/Images/GetTextureEvent.hpp>
#include <NGE/Media/Images/LoadTextureEvent.hpp>
#include <NGE/Events/EventManager.hpp>

bool Background::Initialize(int slices, float radius) {
	// Load background shader.
	NGE::Media::MediaManager::GetInstance().getShaderManager().loadProgram("backgroundShader", "background.xml");

	// Initialise the sphere.
	NGE::Geometry::Basic::Sphere::Initialize(slices, radius, NGE::Media::MediaManager::GetInstance().getShaderManager().getProgram("backgroundShader"));

	// Create the background cubemap from XML description file.
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("config/backgroundCubemap.xml");
	pugi::xml_node cubeMap = doc.child("TextureCubeMap");

	loadTextureEvent = std::make_shared<NGE::Media::Images::LoadTextureEvent>(cubeMap);
	NGE::Events::EventManager::GetInstance().TriggerEvent(loadTextureEvent);
	log_debug("LoadTextureEvent was successful: {}", loadTextureEvent->IsSuccessful());
	log_debug("Texture count: {}", NGE::Media::Images::TextureManager::GetInstance().GetTextureCount());


	std::shared_ptr<NGE::Media::Images::GetTextureEvent> getTextureEvent = std::make_shared<NGE::Media::Images::GetTextureEvent>("backgroundCubemap");
	NGE::Events::EventManager::GetInstance().TriggerEvent(getTextureEvent);
	log_debug("GetTextureEvent was successful: {}", getTextureEvent->IsSuccessful());
	log_debug("Texture count: {}", NGE::Media::MediaManager::GetInstance().GetTextureManager().GetTextureCount());

	this->texture = getTextureEvent->GetTexture();

	//Bind cubemap to the shader.
	shader->bindShader();
	{
		texture->activate(0);
		shader->sendUniform("u_cubemap", 0);
	}
	shader->unbindShader();

	return true;
}

void Background::Render() {
	// Clear color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set visibility to the back of the sphere. As we use sphere as a skydome,
	// we want to see the back of it.
	glFrontFace(GL_CW);

	shader->bindShader();
	{
		// Send modelview and projection matrix to the shader.
		shader->sendUniform4x4("modelviewMatrix", NGE::Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(NGE::MODELVIEW_MATRIX));
		shader->sendUniform4x4("projectionMatrix", NGE::Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(NGE::PROJECTION_MATRIX));

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// Render the sphere.
		glBindVertexArray(vertexArray);
		{
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		}
		glBindVertexArray(0);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	shader->unbindShader();
}

