#include "WaterExample.h"
#include "NGE/Media/MediaManager.hpp"
#include "NGE/Media/Shaders/GLSLProgram.hpp"
#include "NGE/Events/EventManager.hpp"
#include "NGE/Core/Delegate.hpp"

WaterExample::WaterExample() {
    passedTime = 0.0f;
    angle = 0.0f;
}

WaterExample::~WaterExample() { }

bool WaterExample::Init() {
    log_debug("Register mouse listener");
    EventDelegate mouseDelegate("mouse-listener", Core::make_delegate(*this, &WaterExample::MouseListener));
    Events::EventManager::GetInstance().AddListener(mouseDelegate, Events::MouseEvent::eventType);

    log_debug("Initialising MediaManager");
    Media::MediaManager::GetInstance().initialize();

    log_debug("Loading shaders");
    Media::MediaManager::GetInstance().GetShaderManager().LoadProgram("waterTextureShader", "waterTexture.xml");
    Media::MediaManager::GetInstance().GetShaderManager().LoadProgram("waterShader", "water.xml");

    waterShader = Media::MediaManager::GetInstance().GetShaderManager().GetProgram("waterShader");

    camera.Set(0.0f, 1.0f, 0.0f, 50.0f, 5.0f, 22.0f, 0.0f, 1.0f, 0.0f);

    CreateGeometry();
    background.Initialize(64, (waterPlaneLength / 2.0f) + 0.5f);

    waterTexture.SetShader(Media::MediaManager::GetInstance().GetShaderManager().GetProgram("waterTextureShader"));
    unsigned int waterTextureId = waterTexture.Initialise((float) waterPlaneLength);
    tex.setID(waterTextureId);
    waterTexture.Resize(this->GetWindow()->GetWidth(), this->GetWindow()->GetHeight());

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    return true;
}

void WaterExample::Prepare(float dt) {
    camera.Update(dt);
    waterTexture.Update(passedTime);

    passedTime += dt;
    //angle += 2.0f * Math::MathUtils::PI / 120.0f * dt;
}

void WaterExample::Render() {
    Rendering::Renderer::GetInstance().ClearColor(0.9f, 0.95f, 1.0f, 1.0f);
    Rendering::Renderer::GetInstance().ClearBuffers();

    Rendering::Renderer::GetInstance().GetMatrixStack().Identity();

    /*Math::mat4f lookAt;
    lookAt.LookAt(xMove, 1.0f, 0.0f, 0.5f * sinf(angle), 1.0f, -0.5f * cosf(angle), 0.0f, 1.0f, 0.0f);
    Rendering::Renderer::GetInstance().GetMatrixStack().SetMatrixMode(MODELVIEW_MATRIX);
    Rendering::Renderer::GetInstance().GetMatrixStack().LoadMatrix(lookAt);*/

    camera.Look();

    background.Render();
    waterTexture.Render();
    RenderWater();
}

void WaterExample::Shutdown() {
    Media::MediaManager::GetInstance().GetShaderManager().Deinitialise();
}

void WaterExample::OnResize(int width, int height) {
    Application::OnResize(width, height);

    waterTexture.Resize(width, height);
}

void WaterExample::OnKeyPressed(NGE::Events::KeyboardEvent& event) {
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

void WaterExample::OnMouse(NGE::Events::MouseEvent& event) {
    camera.SetMouseInfo(event.GetX(), event.GetY());
}

void WaterExample::OnMouseDrag(int x, int y) { }

void WaterExample::MouseListener(IEventDataPtr event) {
    std::shared_ptr<MouseEvent> mouseEvent = std::dynamic_pointer_cast<MouseEvent>(event);
    camera.SetMouseInfo(mouseEvent->GetX(), mouseEvent->GetY());
}

void WaterExample::CreateGeometry() {

    float* points = new float[waterPlaneLength * waterPlaneLength * 4 * sizeof (float)];
    unsigned int* indices = new unsigned int[waterPlaneLength * (waterPlaneLength - 1) * 2 * sizeof (unsigned int)];

    for (int z = 0; z < waterPlaneLength; ++z) {
        for (int x = 0; x < waterPlaneLength; ++x) {
            points[(x + z * (waterPlaneLength)) * 4 + 0] = -(float) waterPlaneLength / 2 + 0.5f + (float) x;
            points[(x + z * (waterPlaneLength)) * 4 + 1] = 0.0f;
            points[(x + z * (waterPlaneLength)) * 4 + 2] = +(float) waterPlaneLength / 2 - 0.5f - (float) z;
            points[(x + z * (waterPlaneLength)) * 4 + 3] = 1.0f;
        }
    }

    for (int k = 0; k < waterPlaneLength; ++k) {
        for (int i = 0; i < waterPlaneLength; ++i) {
            if (k % 2 == 0) {
                indices[(i + k * waterPlaneLength) * 2 + 0] = i + (k + 1) * waterPlaneLength;
                indices[(i + k * waterPlaneLength) * 2 + 1] = i + k * waterPlaneLength;
            } else {
                indices[(i + k * waterPlaneLength) * 2 + 0] = waterPlaneLength - 1 - i + k * waterPlaneLength;
                indices[(i + k * waterPlaneLength) * 2 + 1] = waterPlaneLength - 1 - i + (k + 1) * waterPlaneLength;
            }
        }
    }

    glGenBuffers(1, &vb);
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glBufferData(GL_ARRAY_BUFFER, waterPlaneLength * waterPlaneLength * 4 * sizeof (float), (float*) points, GL_STATIC_DRAW);

    glGenBuffers(1, &ib);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, waterPlaneLength * (waterPlaneLength - 1) * 2 * sizeof (unsigned int), (unsigned int*) indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &va);
    glBindVertexArray(va);

    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);

    glBindVertexArray(0);

    delete points;
    delete indices;
}

void WaterExample::RenderWater() {
    static const unsigned int numberWaves = 4;
    static WaterTexture::WaveParameters waveParameters[numberWaves];
    static WaterTexture::WaveDirections waveDirections[numberWaves];

    static GLfloat overallSteppness = 0.2f;

    memset(waveParameters, 0, sizeof (waveParameters));
    memset(waveDirections, 0, sizeof (waveDirections));

    // Waves can be faded in and out.

    // Wave one.
    waveParameters[0].speed = 1.0f;
    waveParameters[0].amplitude = 0.01f;
    waveParameters[0].wavelength = 4.0f;
    waveParameters[0].steepness = overallSteppness / (waveParameters[0].wavelength * waveParameters[0].amplitude * (float) numberWaves);
    waveDirections[0].x = +1.0f;
    waveDirections[0].z = +1.0f;

    // Wave two.
    waveParameters[1].speed = 0.5f;
    waveParameters[1].amplitude = 0.02f;
    waveParameters[1].wavelength = 3.0f;
    waveParameters[1].steepness = overallSteppness / (waveParameters[1].wavelength * waveParameters[1].amplitude * (float) numberWaves);
    waveDirections[1].x = +1.0f;
    waveDirections[1].z = +0.0f;

    // Wave three.
    waveParameters[2].speed = 0.1f;
    waveParameters[2].amplitude = 0.015f;
    waveParameters[2].wavelength = 2.0f;
    waveParameters[2].steepness = overallSteppness / (waveParameters[2].wavelength * waveParameters[2].amplitude * (float) numberWaves);
    waveDirections[2].x = -0.1f;
    waveDirections[2].z = -0.2f;

    // Wave four.
    waveParameters[3].speed = 1.1f;
    waveParameters[3].amplitude = 0.008f;
    waveParameters[3].wavelength = 1.0f;
    waveParameters[3].steepness = overallSteppness / (waveParameters[3].wavelength * waveParameters[3].amplitude * (float) numberWaves);
    waveDirections[3].x = -0.2f;
    waveDirections[3].z = -0.1f;

    waterShader->BindShader();

    waterShader->sendUniform4x4("projectionMatrix", Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(PROJECTION_MATRIX));
    waterShader->sendUniform4x4("viewMatrix", Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(MODELVIEW_MATRIX));

    NGE::Math::mat4f inverseViewMatrix = Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(MODELVIEW_MATRIX);
    inverseViewMatrix.SetInverseRigidBody();
    waterShader->sendUniform3x3("inverseViewNormalMatrix", inverseViewMatrix.ExtractMatrix3());

    waterShader->sendUniform("waterPlaneLength", (float) waterPlaneLength);

    waterShader->sendUniform("passedTime", passedTime);

    waterShader->sendUniformArray4("waveParameters", 4 * numberWaves, (GLfloat*) waveParameters);
    waterShader->sendUniformArray2("waveDirections", 2 * numberWaves, (GLfloat*) waveDirections);

    Media::MediaManager::GetInstance().GetTextureManager().GetTexture("backgroundCubemap")->activate(0);
    waterShader->sendUniform("cubemap", 0);

    tex.activate(1);
    waterShader->sendUniform("waterTexture", 1);

    waterShader->sendUniform("cameraPos", camera.GetViewerPosition());

    glBindVertexArray(va);

    glFrontFace(GL_CCW);

    glDrawElements(GL_TRIANGLE_STRIP, waterPlaneLength * (waterPlaneLength - 1) * 2, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}