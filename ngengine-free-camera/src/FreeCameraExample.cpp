#include "FreeCameraExample.h"
#include <NGE/Core/Core.hpp>
#include <NGE/Rendering/Rendering.hpp>

bool FreeCameraExample::Init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    return true;
}

void FreeCameraExample::Prepare(float dt) { }

void FreeCameraExample::Render() {
    Rendering::Renderer::GetInstance().ClearColor(0.9f, 0.95f, 1.0f, 1.0f);
    Rendering::Renderer::GetInstance().ClearBuffers();
    Rendering::Renderer::GetInstance().GetMatrixStack().Identity();
}

void FreeCameraExample::Shutdown() { }

void FreeCameraExample::OnResize(int width, int height) {
    Application::OnResize(width, height);
}

void FreeCameraExample::OnMouse(NGE::Events::MouseEvent& event) { 
    
}

void FreeCameraExample::FilterMouseMove(float dx, float dy) {
    for (int i = MOUSE_HISTORY_BUFFER_SIZE - 1; i > 0; --i) {
        mouseHistory[i] = mouseHistory[i - 1];
    }

    // Store current mouse entry at front of array.
    mouseHistory[0] = m::vec2f(dx, dy);

    float averageX = 0.f;
    float averageY = 0.f;
    float averageTotal = 0.f;
    float currentWeight = 1.0f;

    // Filter the mouse.
    for (int i = 0; i < MOUSE_HISTORY_BUFFER_SIZE; ++i) {
        m::vec2f tmp = mouseHistory[i];
        averageX += tmp.x * currentWeight;
        averageY += tmp.y * currentWeight;
        averageTotal += 1.f * currentWeight;
        currentWeight *= MOUSE_FILTER_WEIGHT;
    }

    mouseX = averageX / averageTotal;
    mouseY = averageY / averageTotal;
}
