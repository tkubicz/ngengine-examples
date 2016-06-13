#include "SimpleImageLoader.h"
#include <NGE/Core/Core.hpp>
#include <NGE/Events/EventManager.hpp>
#include <NGE/Rendering/Rendering.hpp>
#include <NGE/Media/Images/TextureManager.hpp>
#include <NGE/Media/Images/LoadTextureEvent.hpp>
#include <NGE/Media/Images/GetTextureEvent.hpp>

namespace e = NGE::Events;
namespace i = NGE::Media::Images;

bool SimpleImageLoader::Init() {
	e::EventManager& eventManager = e::EventManager::GetInstance();
	i::TextureManager& textureManager = i::TextureManager::GetInstance();
	textureManager.Initialise();

	std::string imageName = "Lenna";

	pugi::xml_document doc;
	auto result = doc.load_file("config/image.xml");
	if (!result) {
		log_error("Could not load image description");
		return false;
	}
	auto imageNode = doc.child("Texture2D");

	auto loadEvent = std::make_shared<i::LoadTextureEvent>(imageNode);
	auto getEvent = std::make_shared<i::GetTextureEvent>(imageName);

	eventManager.TriggerEvent(loadEvent);
	eventManager.TriggerEvent(getEvent);

	if (!getEvent->IsSuccessful()) {
		log_error("Could not get '{}' image", imageName);
		return false;
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
