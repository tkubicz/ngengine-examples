#include "SimpleImageLoader.h"
#include <NGE/Core/Core.hpp>
#include <NGE/Rendering/Rendering.hpp>
#include <NGE/Media/Images/PngImage.hpp>
#include <NGE/Media/Images/LoadTextureEvent.hpp>

namespace i = NGE::Media::Images;

bool SimpleImageLoader::Init() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	NGE::Media::Images::PngImage pngImage;

	if (!pngImage.load("images/Lenna.png")) {
		log_error("Could not load image");
		return false;
	} else {
		log_debug("Image loaded");
	}
	

	return true;
}

void SimpleImageLoader::Render() {
	Rendering::Renderer::GetInstance().ClearColor(0.9f, 0.95f, 1.0f, 1.0f);
	Rendering::Renderer::GetInstance().ClearBuffers();
	Rendering::Renderer::GetInstance().GetMatrixStack().Identity();
}

void SimpleImageLoader::Shutdown() {
}

void SimpleImageLoader::OnResize(int width, int height) {
	Application::OnResize(width, height);
}
