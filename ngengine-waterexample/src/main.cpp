#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <NGE/Tools/Tools.hpp>
#include <NGE/Windows/Windows.hpp>
#include <NGE/Rendering/Rendering.hpp>
#include <NGE/Media/MediaManager.hpp>

#include "WaterExample.h"

using namespace NGE::Tools;
using namespace NGE::Windows;
using namespace NGE::Media;
using namespace NGE::Rendering;

#ifdef _WIN32
#include <windows.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else 

int main()
#endif
{
    Logger::Initialize("log.txt", true);

    if (!glfwInit())
    {
        Logger::WriteFatalErrorLog("Error starting GLFW");
        return 1;
    }

    WaterExample app;
    GLFW3Window programWindow;
    programWindow.SetApplication(&app);

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("config/config.xml");
    pugi::xml_node window = doc.child("Window");

    if (!programWindow.LoadXMLSettings(window))
    {
        Logger::WriteFatalErrorLog("Unable to load window settings");
        return 1;
    }

    MediaManager::getInstance().getMediaPathManager().loadXMLSettings(doc.child("Config"));

    if (!programWindow.Create())
    {
        Logger::WriteFatalErrorLog("Unable to create OpenGL window");
        programWindow.Destroy();
        return 1;
    }

    if (!programWindow.Init())
    {
        Logger::WriteFatalErrorLog("Could not initialize GLEW");
        programWindow.Destroy();
        return 1;
    }

    Renderer::GetInstance().GetRendererInformation();
    Renderer::GetInstance().GetMatrixStack().Initialize();

    if (!app.Init())
    {
        Logger::WriteFatalErrorLog("Could not initialise application");
        programWindow.Destroy();
        return 1;
    }

	Timing& timing = Timing::GetInstance();
	
    timing.Initialize();
    programWindow.SetInputCallbacks();
    app.OnResize(programWindow.GetWidth(), programWindow.GetHeight());

    while (programWindow.IsRunning())
    {
        timing.Update();
        float elapsedTime = static_cast<float> (timing.GetLastFrameDuration());

        app.Prepare(elapsedTime);
        app.Render();

        programWindow.SwapBuffers();
        programWindow.ProcessEvents();
    }

    Logger::Flush();
    app.Shutdown();

    MediaManager::getInstance().deinitialize();
    Renderer::GetInstance().GetMatrixStack().Deinitialize();

    programWindow.Destroy();

    return 0;
}