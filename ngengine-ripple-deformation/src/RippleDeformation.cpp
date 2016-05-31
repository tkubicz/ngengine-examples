#include "RippleDeformation.h"
#include <NGE/Core/Core.hpp>
#include <NGE/Rendering/Rendering.hpp>
#include <NGE/Media/Shaders/ShaderManager.hpp>
#include <NGE/Tools/GLError.hpp>

bool RippleDeformation::Init() {
	Media::Shaders::ShaderManager& shaderManager = Media::Shaders::ShaderManager::GetInstance();

	shaderManager.LoadProgram("shader", "shader.xml");
	shader = shaderManager.GetProgram("shader");
	if (shader == nullptr) {
		log_error("Could not load shader");
		return false;
	}

	check_gl_error();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	return true;
}

void RippleDeformation::Render() {
	Rendering::Renderer::GetInstance().ClearColor(0.9f, 0.95f, 1.0f, 1.0f);
	Rendering::Renderer::GetInstance().ClearBuffers();
	Rendering::Renderer::GetInstance().GetMatrixStack().Identity();
}

void RippleDeformation::Shutdown() {
}

void RippleDeformation::OnResize(int width, int height) {
	Application::OnResize(width, height);
}

void RippleDeformation::CreateTopology() {
	//	int count = 0, i = 0, j = 0;
	//	for (j = 0; j < NUM_Z; ++j) {
	//		for (i = 0; i < NUM_X; ++i) {
	//			vertices[count++] = Math::vec3f()
	//		}
	//	}
}
