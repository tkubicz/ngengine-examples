#include "SubdivisionGeometryExample.h"
#include <NGE/Core/Core.hpp>
#include <NGE/Rendering/Rendering.hpp>
#include <NGE/Media/Shaders/ShaderManager.hpp>

using namespace NGE;

bool SubdivisionGeometryExample::Init() {

	Media::Shaders::ShaderManager& manager = Media::Shaders::ShaderManager::GetInstance();

	manager.LoadProgram("shader", "shader.xml");
	shader = manager.GetProgram("shader");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	return true;
}

void SubdivisionGeometryExample::Render() {
	Rendering::Renderer::GetInstance().ClearColor(0.9f, 0.95f, 1.0f, 1.0f);
	Rendering::Renderer::GetInstance().ClearBuffers();
	Rendering::Renderer::GetInstance().GetMatrixStack().Identity();
}

void SubdivisionGeometryExample::Shutdown() {
}

void SubdivisionGeometryExample::OnResize(int width, int height) {
	Application::OnResize(width, height);
}
