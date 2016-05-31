#include "BlankExample.h"
#include <NGE/Core/Core.hpp>
#include <NGE/Rendering/Rendering.hpp>

bool BlankExample::Init() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	return true;
}

void BlankExample::Render() {
	Rendering::Renderer::GetInstance().ClearColor(0.9f, 0.95f, 1.0f, 1.0f);
	Rendering::Renderer::GetInstance().ClearBuffers();
	Rendering::Renderer::GetInstance().GetMatrixStack().Identity();
}

void BlankExample::Shutdown() {
}

void BlankExample::OnResize(int width, int height) {
	Application::OnResize(width, height);
}
