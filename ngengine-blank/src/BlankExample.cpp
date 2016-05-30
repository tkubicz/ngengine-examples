#include "BlankExample.h"
#include "NGE/Media/MediaManager.hpp"
#include "NGE/Media/Shaders/GLSLProgram.hpp"
#include "NGE/Events/EventManager.hpp"
#include "NGE/Core/Delegate.hpp"

BlankExample::BlankExample() {
}

BlankExample::~BlankExample() { }

bool BlankExample::Init() {
    log_debug("Register mouse listener");
    EventDelegate mouseDelegate("mouse-listener", Core::make_delegate(*this, &BlankExample::MouseListener));
    Events::EventManager::GetInstance().AddListener(mouseDelegate, Events::MouseEvent::eventType);

    log_debug("Initialising MediaManager");
    Media::MediaManager::GetInstance().initialize();

    log_debug("Loading shaders");
    //Media::MediaManager::GetInstance().GetShaderManager().LoadProgram("waterTextureShader", "waterTexture.xml");
    //Media::MediaManager::GetInstance().GetShaderManager().LoadProgram("waterShader", "water.xml");

    //waterShader = Media::MediaManager::GetInstance().GetShaderManager().GetProgram("waterShader");

    camera.Set(0.0f, 1.0f, 0.0f, 50.0f, 5.0f, 22.0f, 0.0f, 1.0f, 0.0f);

    //CreateGeometry();
    //background.Initialize(64, (waterPlaneLength / 2.0f) + 0.5f);

    //waterTexture.SetShader(Media::MediaManager::GetInstance().GetShaderManager().GetProgram("waterTextureShader"));
    //unsigned int waterTextureId = waterTexture.Initialise((float) waterPlaneLength);
    //tex.setID(waterTextureId);
    //waterTexture.Resize(this->GetWindow()->GetWidth(), this->GetWindow()->GetHeight());

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    return true;
}

void BlankExample::Prepare(float dt) {
    camera.Update(dt);
}

void BlankExample::Render() {
    Rendering::Renderer::GetInstance().ClearColor(0.9f, 0.95f, 1.0f, 1.0f);
    Rendering::Renderer::GetInstance().ClearBuffers();

    Rendering::Renderer::GetInstance().GetMatrixStack().Identity();

    camera.Look();

    //background.Render();
}

void BlankExample::Shutdown() {
    Media::MediaManager::GetInstance().GetShaderManager().Deinitialise();
}

void BlankExample::OnResize(int width, int height) {
    Application::OnResize(width, height);
}

void BlankExample::OnKeyPressed(NGE::Events::KeyboardEvent& event) {
    camera.SetKeyboardInput(event);

    if (event.GetAction() == Events::PRESSED) {
        if (event.GetKeyId() == Events::KEY_Z) {
            if (camera.IsMouseLocked()) {
                window->EnableMouseCursor(true);
            } else {
                window->EnableMouseCursor(false);
            }

            camera.LockMouse(!camera.IsMouseLocked());
        }
    }
}

void BlankExample::OnMouse(NGE::Events::MouseEvent& event) {
    camera.SetMouseInfo(event.GetX(), event.GetY());
}

void BlankExample::OnMouseDrag(int x, int y) { }

void BlankExample::MouseListener(IEventDataPtr event) {
    std::shared_ptr<MouseEvent> mouseEvent = std::dynamic_pointer_cast<MouseEvent>(event);
    camera.SetMouseInfo(mouseEvent->GetX(), mouseEvent->GetY());
}
