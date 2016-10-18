#include "BlankExample.h"
#include <NGE/Core/Core.hpp>
#include <NGE/Rendering/Rendering.hpp>

bool BlankExample::Init() {

	log_debug("Initialising MediaManager");
	Media::MediaManager::GetInstance().initialize();

	log_debug("Loading shaders");
	Media::MediaManager::GetInstance().GetShaderManager().LoadProgram("floorShader", "shaders.xml");
	Media::MediaManager::GetInstance().GetShaderManager().LoadProgram("basicShader", "shaders.xml");
	Media::MediaManager::GetInstance().GetShaderManager().LoadProgram("solidColorShader", "shaders.xml");

	log_debug("Loading textures");
	pugi::xml_document textureInfo;
	pugi::xml_parse_result textureResult = textureInfo.load_file("textures/textures.xml");
	pugi::xml_node currentTexture = textureInfo.child("Texture2D");
	Media::MediaManager::GetInstance().GetTextureManager().LoadTexture(currentTexture);

	camera.Set(-35.0f, 10.0f, 0.0f, 50.0f, 10.0f, 22.0f, 0.0f, 1.0f, 0.0f);
	floor.Initialize(100.f, 4.f, Media::MediaManager::GetInstance().GetShaderManager().GetProgram("floorShader"));

	sphere.Initialize(20, 2.0, Media::MediaManager::GetInstance().GetShaderManager().GetProgram("basicShader"));
	sphere.SetTexture(Media::MediaManager::GetInstance().GetTextureManager().GetTexture(currentTexture));

	octree = new Rendering::Scene::Octree<Math::vec4f>(Math::vec3f(0, 0, 0), 100.0f);

	for (unsigned i = 0; i < 100; ++i) {
		spherePositions.push_back(Math::vec4f(
				random.RandomFloat(-100.0f, 100.0f),
				random.RandomFloat(1.0f, 20.0f),
				random.RandomFloat(-100.0f, 100.0f),
				random.RandomFloat(0.2f, 1.0f)
				));
	}

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
	floor.Render();
	
	static int rendered;
	rendered = 0;
	for (unsigned i = 0; i < spherePositions.size(); ++i) {
		Rendering::Renderer::GetInstance().GetMatrixStack().PushMatrix();
        Rendering::Renderer::GetInstance().GetMatrixStack().Translate(spherePositions[i].x, spherePositions[i].y, spherePositions[i].z);
        Rendering::Renderer::GetInstance().GetMatrixStack().Scale(spherePositions[i].w, spherePositions[i].w, spherePositions[i].w);
        sphere.Render();
        Rendering::Renderer::GetInstance().GetMatrixStack().PopMatrix();
        rendered += 1;
	}
	
	//log_debug("Rendered: {}/{}", rendered, spherePositions.size());
}

void BlankExample::Shutdown() {
	delete octree;
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

void BlankExample::OnResize(int width, int height) {
	Application::OnResize(width, height);
}
