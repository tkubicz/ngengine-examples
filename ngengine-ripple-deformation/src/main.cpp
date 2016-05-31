#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <NGE/Tools/Tools.hpp>
#include <NGE/Windows/Windows.hpp>
#include <NGE/Rendering/Rendering.hpp>
#include <NGE/Media/MediaManager.hpp>
#include <NGE/Events/EventManager.hpp>

#include "RippleDeformation.h"

using namespace NGE::Tools;
using namespace NGE::Windows;
using namespace NGE::Media;
using namespace NGE::Rendering;
using namespace NGE::Events;

const char* EXAMPLE_NAME = "ngengine-ripple-deformation";

#ifdef _WIN32
#include <windows.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else 

int main()
#endif
{
	Logger::NewLogger& log = Logger::NewLogger::GetInstance();
	log["console"]->SetAutoFlushEnabled(true);
	log["console"]->SetFlushAfter(1);
	log["console"]->SetLogLevel(Logger::LogTypes::LOG_LEVEL::DEBUG);

	log_info("Starting application: '{}'", EXAMPLE_NAME);

	if (!glfwInit()) {
		log_error("Error starting GLFW");
		return 1;
	}

	RippleDeformation app;
	GLFW3Window programWindow;
	programWindow.SetApplication(&app);

	pugi::xml_document doc;
	std::string configPath = "config/config.xml";

	pugi::xml_parse_result result = doc.load_file(configPath.c_str());
	if (!result) {
		log_error("Could not find default configuration at: '{}'", configPath);
		return 1;
	}

	pugi::xml_node window = doc.child("Window");
	if (!programWindow.LoadXMLSettings(window)) {
		log_error("Unable to load window settings");
		return 1;
	}

	MediaManager::GetInstance().getMediaPathManager().loadXMLSettings(doc.child("Config"));

	if (!programWindow.Create()) {
		log_error("Unable to create OpenGL window");
		programWindow.Destroy();
		return 1;
	}

	if (!programWindow.Init()) {
		log_error("Could not initialise GLEW");
		programWindow.Destroy();
		return 1;
	}

	Renderer::GetInstance().GetRendererInformation();
	Renderer::GetInstance().GetMatrixStack().Initialize();

	if (!app.Init()) {
		log_error("Could not initialise application");
		programWindow.Destroy();
		return 1;
	}

	Timing& timing = Timing::GetInstance();
	EventManager& eventManager = EventManager::GetInstance();

	timing.Initialize();
	programWindow.SetInputCallbacks();
	app.OnResize(programWindow.GetWidth(), programWindow.GetHeight());

	while (programWindow.IsRunning()) {
		timing.Update();
		float elapsedTime = static_cast<float> (timing.GetLastFrameDuration());
		eventManager.UpdateAll(17);

		app.Prepare(elapsedTime);
		app.Render();

		programWindow.SwapBuffers();
		programWindow.ProcessEvents();
	}

	log_info("Stopping application: '{}'", EXAMPLE_NAME);

	app.Shutdown();

	MediaManager::GetInstance().deinitialize();
	Renderer::GetInstance().GetMatrixStack().Deinitialize();

	programWindow.Destroy();

	log_info("Application stopped: '{}'", EXAMPLE_NAME);
	Logger::NewLogger::GetInstance().Flush();

	return 0;
}