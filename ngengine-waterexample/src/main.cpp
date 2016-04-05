#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <NGE/Tools/Tools.hpp>
#include <NGE/Windows/Windows.hpp>
#include <NGE/Rendering/Rendering.hpp>
#include <NGE/Media/MediaManager.hpp>
#include <NGE/Events/EventManager.hpp>

#include "WaterExample.h"

using namespace NGE::Tools;
using namespace NGE::Windows;
using namespace NGE::Media;
using namespace NGE::Rendering;
using namespace NGE::Events;

#ifdef _WIN32
#include <windows.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else 

int main()
#endif
{
	Logger::NewLogger& log = Logger::NewLogger::GetInstance();

	log.GetOutputs()["file"]->SetEnabled(true);
	log.GetOutputs()["console"]->SetEnabled(true);
	log.SetLogLevel(Logger::LogTypes::LOG_LEVEL::TRACE);
	log.SetAutoFlushEnabled(true);
	log.SetFlushAfter(1);

	log_info("Starting application ngengine-waterexample");

	log_debug("Initialising GLFW");
	if (!glfwInit()) {
		log_error("Error starting GLFW");
		return 1;
	}

	WaterExample app;
	GLFW3Window programWindow;
	programWindow.SetApplication(&app);

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("config/config.xml");
	pugi::xml_node window = doc.child("Window");

	log_debug("Loading window settings");
	if (!programWindow.LoadXMLSettings(window)) {
		log_error("Unable to load window settings");
		return 1;
	}

	MediaManager::GetInstance().getMediaPathManager().loadXMLSettings(doc.child("Config"));

	log_debug("Creating OpenGL window");
	if (!programWindow.Create()) {
		log_error("Unable to create OpenGL window");
		programWindow.Destroy();
		return 1;
	}

	log_debug("Initialising GLEW");
	if (!programWindow.Init()) {
		log_error("Could not initialise GLEW");
		programWindow.Destroy();
		return 1;
	}

	Renderer::GetInstance().GetRendererInformation();
	Renderer::GetInstance().GetMatrixStack().Initialize();

	log_debug("Initialising application");
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
		eventManager.UpdateAll(1000);

		app.Prepare(elapsedTime);
		app.Render();

		programWindow.SwapBuffers();
		programWindow.ProcessEvents();
	}

	log_info("Stopping application ngengine-waterexample\n");

	Logger::NewLogger::GetInstance().Flush();
	app.Shutdown();

	MediaManager::GetInstance().deinitialize();
	Renderer::GetInstance().GetMatrixStack().Deinitialize();

	programWindow.Destroy();

	return 0;
}