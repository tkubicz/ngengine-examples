#include "BridgeExample.h"
#include <NGE/Tools/Tools.hpp>
#include <NGE/Media/MediaManager.hpp>
#include <NGE/Tools/Random.hpp>
#include <NGE/Math/Matrix4.hpp>
#include <NGE/Rendering/Rendering.hpp>

BridgeExample::BridgeExample()
{
    mouseLocked = false;
    particleCount = 12;
    massPos.Set(0.f, 0.f, 0.5f);
    GRAVITY.Set(0.f, -9.81f, 0.f);
    pause = false;
}

BridgeExample::~BridgeExample() { }

bool BridgeExample::Init()
{
    std::map<std::string, std::string> shaders;
    shaders.insert(std::make_pair("basicShader", "shaders.xml"));
    shaders.insert(std::make_pair("floorShader", "floorShader.xml"));
    shaders.insert(std::make_pair("bridgeShader", "shaders.xml"));
    shaders.insert(std::make_pair("colorShader", "colorShader.xml"));

    for (std::map<std::string, std::string>::iterator i = shaders.begin(); i != shaders.end(); ++i)
        NGE::Media::MediaManager::GetInstance().GetShaderManager().LoadProgram(i->first, i->second);

    shader = NGE::Media::MediaManager::GetInstance().GetShaderManager().GetProgram("basicShader");
    floorShader = NGE::Media::MediaManager::GetInstance().GetShaderManager().GetProgram("floorShader");
    bridgeShader = NGE::Media::MediaManager::GetInstance().GetShaderManager().GetProgram("bridgeShader");

    pugi::xml_document textureInfo;
    pugi::xml_parse_result textureResult = textureInfo.load_file("config/textures.xml");
    pugi::xml_node currentTexture = textureInfo.child("Texture2D");

	
	if (!Media::MediaManager::GetInstance().GetTextureManager().LoadTexture(currentTexture)) {
		log_error("Could not load wood texture");
		return false;
	}
	
    woodTexture = Media::MediaManager::GetInstance().GetTextureManager().GetTexture(currentTexture);
	if (woodTexture == nullptr) {
		log_error("Could not get wood texture");
		return false;
	}
	
    //currentTexture = currentTexture.next_sibling();

    camera.Set(-35.0f, 10.0f, 0.0f, 50.0f, 5.0f, 22.0f, 0.0f, 1.0f, 0.0f);

    GenerateFloor();

    sphere.Initialize(20, 10, shader);
    sphere.SetShader(shader);
    sphere.SetTexture(woodTexture);

    world = new Physics::MassAggregate::ParticleWorld(particleCount * 10);
    particleArray = new Physics::MassAggregate::Particle[particleCount];
    for (unsigned i = 0; i < particleCount; ++i)
    {
        world->GetParticles().push_back(particleArray + i);
    }

    Tools::Random random;

    /*for (int i = 0; i < 1000; ++i)
    {
        Physics::Particle* p1 = new Physics::Particle();
        p1->SetPosition(random.RandomFloat(-20, 20), random.RandomFloat(10, 40), random.RandomFloat(-20, 20));
        p1->SetMass(random.RandomFloat(3.0f, 6.0f));
        //p1->SetVelocity(0, 0, 0);
        p1->SetDamping(random.RandomFloat(0.5f, 2.0f));
        p1->SetAcceleration(GRAVITY);
        p1->ClearAccumulator();
        world->GetParticles().push_back(p1);
    }*/

    groundContactGenerator.Init(&world->GetParticles());
    world->GetContactGenerators().push_back(&groundContactGenerator);

    // Utworzenie mas i połączeń
    for (unsigned i = 0; i < particleCount; ++i)
    {
        unsigned x = (i % particleCount) / 2;
        particleArray[i].SetPosition(float(i / 2) * 2.0f - 5.0f, 8, float(i % 2) * 2.0f - 1.0f);
        particleArray[i].SetVelocity(0, 0, 0);
        particleArray[i].SetDamping(0.9f);
        particleArray[i].SetAcceleration(GRAVITY);
        particleArray[i].ClearAccumulator();
    }

    // Połączenie (linki)
    cables = new Physics::MassAggregate::ParticleCable[CABLE_COUNT];
    for (unsigned i = 0; i < CABLE_COUNT; ++i)
    {
        cables[i].particle[0] = &particleArray[i];
        cables[i].particle[1] = &particleArray[i + 2];
        cables[i].maxLength = 1.9f;
        cables[i].restitution = 0.3f;
        world->GetContactGenerators().push_back(&cables[i]);
    }

    supports = new Physics::MassAggregate::ParticleCableConstraint[SUPPORT_COUNT];
    for (unsigned i = 0; i < SUPPORT_COUNT; ++i)
    {
        supports[i].particle = particleArray + i;
        supports[i].anchor = Math::vec3f(float(i / 2) * 2.2f - 5.5f, 10, float(i % 2) * 1.6f - 0.8f);
        if (i < 6)
            supports[i].maxLength = float(i / 2) * 0.5f + 3.0f;
        else
            supports[i].maxLength = 5.5f - float(i / 2) * 0.5f;
        supports[i].restitution = 0.5f;
        world->GetContactGenerators().push_back(&supports[i]);
    }

    rods = new Physics::MassAggregate::ParticleRod[ROD_COUNT];
    for (unsigned i = 0; i < 6; ++i)
    {
        rods[i].particle[0] = &particleArray[i * 2];
        rods[i].particle[1] = &particleArray[i * 2 + 1];
        rods[i].length = 2;
        world->GetContactGenerators().push_back(&rods[i]);
    }

    UpdateAdditionalMass();
    GenerateBridge();

    return true;
}

void BridgeExample::Prepare(float dt)
{
    world->StartFrame();

    camera.Update(dt);

    if (!pause)
    {
        world->RunPhysics(dt);
        UpdateAdditionalMass();

        if (keyEvent.GetAction() == 1)
        {
            static float moveSpeed = 14.0f;
            // up
            if (keyEvent.GetKeyId() == 283)
            {
                massPos.z -= moveSpeed * dt;
                if (massPos.z < 0.0f)
                    massPos.z = 0.0f;
            }

            // down
            if (keyEvent.GetKeyId() == 284)
            {
                massPos.z += moveSpeed * dt;
                if (massPos.z > 1.0f)
                    massPos.z = 1.0f;
            }

            // left
            if (keyEvent.GetKeyId() == 285)
            {
                massPos.x -= moveSpeed * dt;
                if (massPos.x < 0.0f)
                    massPos.x = 0.0f;
            }

            // right
            if (keyEvent.GetKeyId() == 286)
            {
                massPos.x += moveSpeed * dt;
                if (massPos.x > 5.0f)
                    massPos.x = 5.0f;
            }
        }

        //GenerateBridge();
        UpdateBridge();
    }
}

void BridgeExample::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.9f, 0.95f, 1.0f, 1.0f);
    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    Rendering::Renderer::GetInstance().GetMatrixStack().Identity();

    camera.Look();

    RenderFloor();
    RenderBridge();

    shader->BindShader();
    woodTexture->Activate();
    shader->sendUniform("texture0", 0);

    Physics::MassAggregate::ParticleWorld::Particles& particles = world->GetParticles();
    for (Physics::MassAggregate::ParticleWorld::Particles::iterator p = particles.begin(); p != particles.end(); ++p)
    {
        Physics::MassAggregate::Particle* particle = *p;
        Math::vec3f pos = particle->GetPosition();
        Rendering::Renderer::GetInstance().GetMatrixStack().PushMatrix();
        Rendering::Renderer::GetInstance().GetMatrixStack().Translate(pos.x, pos.y, pos.z);
        sphere.Render();
        Rendering::Renderer::GetInstance().GetMatrixStack().PopMatrix();
    }

    Rendering::Renderer::GetInstance().GetMatrixStack().PushMatrix();
    {
        Rendering::Renderer::GetInstance().GetMatrixStack().Translate(massDisplayPos.x, massDisplayPos.y + 0.25f, massDisplayPos.z);
        sphere.Render();
    }
    Rendering::Renderer::GetInstance().GetMatrixStack().PopMatrix();
}

void BridgeExample::Shutdown()
{
	iceTexture.Unload();
}

void BridgeExample::OnKeyPressed(NGE::Events::KeyboardEvent& event)
{
    //int val = key - 48;
    //log_info("key: " + to_string(key) + " | action: " + to_string(action));
    int key = event.GetKeyId();
    int action = event.GetAction();

    if (key == Events::KEY_W || key == Events::KEY_A || key == Events::KEY_S || key == Events::KEY_D)
    {
        camera.SetKeyboardInput(event);
    }

    if (action == Events::PRESSED)
    {
        if (key == Events::KEY_P) 
            pause = !pause;
    }

    keyEvent = event;
}

void BridgeExample::OnMouse(NGE::Events::MouseEvent& event)
{
    camera.SetMouseInfo(event.GetX(), event.GetY());

    /*if (event.GetButtonId() == Events::BUTTON1 && event.GetAction() == Events::PRESSED)
    {
        Geometry::Ray<float> ray = Rendering::Renderer::GetCursorRay(Math::Vector2<int>(event.GetX(), event.GetY()));
        std::cout << "direction: " << ray.direction << std::endl;
        std::cout << "position: " << ray.position << std::endl;
        std::cout << "position: (" << event.GetX() << ", " << event.GetYInverse() << ")" << std::endl;
    }*/
}

void BridgeExample::OnMouseDrag(int x, int y)
{
    camera.SetMouseInfo(x, y);
}

void BridgeExample::OnResize(int width, int height)
{
    Application::OnResize(width, height);
}

void BridgeExample::GenerateFloor()
{
    static int size = 100;

    for (float i = (float) -size; i <= (float) size; i += 4.f)
    {
        floorVertices.push_back(Math::vec3f((float) -size, 0.f, i));
        floorVertices.push_back(Math::vec3f((float) size, 0.0f, i));
        floorVertices.push_back(Math::vec3f(i, 0.0f, (float) -size));
        floorVertices.push_back(Math::vec3f(i, 0.0f, (float) size));
    }

    glGenBuffers(1, &floorVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, floorVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat) * floorVertices.size() * 3, &floorVertices[0], GL_STATIC_DRAW);
}

void BridgeExample::RenderFloor()
{
    Rendering::Renderer::GetInstance().GetMatrixStack().PushMatrix();
    floorShader->BindShader();
    floorShader->AutoEnableVertexAttribArray();

    glBindBuffer(GL_ARRAY_BUFFER, floorVertexBuffer);
    glVertexAttribPointer((GLint) 0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    floorShader->sendUniform4x4("modelview_matrix", Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(MODELVIEW_MATRIX));
    floorShader->sendUniform4x4("projection_matrix", Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(PROJECTION_MATRIX));
    floorShader->sendUniform("color", Math::vec4f(0.0f, 0.6f, 0.0f));

    glDrawArrays(GL_LINES, 0, floorVertices.size());

    floorShader->AutoDisableVertexAttribArray();
    Rendering::Renderer::GetInstance().GetMatrixStack().PopMatrix();
}

void BridgeExample::UpdateAdditionalMass()
{
    for (unsigned i = 0; i < particleCount; ++i)
        particleArray[i].SetMass((float) BASE_MASS);

    int x = int(massPos.x);
    float xp = fmod(massPos.x, float(1.0f));

    if (x < 0)
    {
        x = 0;
        xp = 0;
    }

    if (x >= 5)
    {
        x = 5;
        xp = 0;
    }

    int z = int(massPos.z);
    float zp = fmod(massPos.z, float(1.0f));

    if (z < 0)
    {
        z = 0;
        zp = 0;
    }

    if (z >= 1)
    {
        z = 1;
        zp = 0;
    }

    massDisplayPos.Clear();

    particleArray[x * 2 + z].SetMass(BASE_MASS + EXTRA_MASS * (1 - xp) * (1 - zp));
    massDisplayPos += particleArray[x * 2 + z].GetPosition() * ((1 - xp) * (1 - zp));

    if (xp > 0)
    {
        particleArray[x * 2 + z + 2].SetMass(BASE_MASS + EXTRA_MASS * xp * (1 - zp));
        massDisplayPos += particleArray[x * 2 + z + 2].GetPosition() * (xp * (1 - zp));

        if (zp > 0)
        {
            particleArray[x * 2 + z + 3].SetMass(BASE_MASS + EXTRA_MASS * xp * zp);
            massDisplayPos += particleArray[x * 2 + z + 3].GetPosition() * (xp * zp);
        }
    }

    if (zp > 0)
    {
        particleArray[x * 2 + z + 1].SetMass(BASE_MASS + EXTRA_MASS * (1 - xp) * zp);
        massDisplayPos += particleArray[x * 2 + z + 1].GetPosition() * ((1 - xp) * zp);
    }
}

void BridgeExample::GenerateBridge()
{
    for (unsigned i = 0; i < ROD_COUNT; ++i)
    {
        Physics::MassAggregate::Particle** particles = rods[i].particle;
        const Math::vec3f& p0 = particles[0]->GetPosition();
        const Math::vec3f& p1 = particles[1]->GetPosition();
        bridgeVertices.push_back(Math::vec3f(p0.x, p0.y, p0.z));
        bridgeVertices.push_back(Math::vec3f(p1.x, p1.y, p1.z));
        bridgeColors.push_back(Math::vec3f(0, 0, 1));
        bridgeColors.push_back(Math::vec3f(0, 0, 1));
    }

    for (unsigned i = 0; i < CABLE_COUNT; ++i)
    {
        Physics::MassAggregate::Particle** particles = cables[i].particle;
        const Math::vec3f& p0 = particles[0]->GetPosition();
        const Math::vec3f& p1 = particles[1]->GetPosition();
        bridgeVertices.push_back(Math::vec3f(p0));
        bridgeVertices.push_back(Math::vec3f(p1));

        bridgeColors.push_back(Math::vec3f(0, 1, 0));
        bridgeColors.push_back(Math::vec3f(0, 1, 0));
    }

    for (unsigned i = 0; i < SUPPORT_COUNT; ++i)
    {
        const Math::vec3f& p0 = supports[i].particle->GetPosition();
        const Math::vec3f& p1 = supports[i].anchor;

        bridgeVertices.push_back(Math::vec3f(p0));
        bridgeVertices.push_back(Math::vec3f(p1));

        bridgeColors.push_back(Math::vec3f(0.7f, 0.7f, 0.7f));
        bridgeColors.push_back(Math::vec3f(0.7f, 0.7f, 0.7f));
    }

    glGenBuffers(1, &bridgeVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, bridgeVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat) * bridgeVertices.size() * 3, &bridgeVertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &bridgeColorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, bridgeColorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat) * bridgeColors.size() * 3, &bridgeColors[0], GL_STATIC_DRAW);
}

void BridgeExample::UpdateBridge()
{
    for (unsigned i = 0; i < ROD_COUNT; ++i)
    {
        Physics::MassAggregate::Particle** particles = rods[i].particle;
        const Math::vec3f& p0 = particles[0]->GetPosition();
        const Math::vec3f& p1 = particles[1]->GetPosition();
        bridgeVertices[2 * i] = Math::vec3f(p0.x, p0.y, p0.z);
        bridgeVertices[2 * i + 1] = Math::vec3f(p1.x, p1.y, p1.z);
    }

    for (unsigned i = 0; i < CABLE_COUNT; ++i)
    {
        Physics::MassAggregate::Particle** particles = cables[i].particle;
        const Math::vec3f& p0 = particles[0]->GetPosition();
        const Math::vec3f& p1 = particles[1]->GetPosition();
        bridgeVertices[(2 * ROD_COUNT) + (2 * i)] = Math::vec3f(p0);
        bridgeVertices[(2 * ROD_COUNT) + (2 * i + 1)] = Math::vec3f(p1);
    }

    for (unsigned i = 0; i < SUPPORT_COUNT; ++i)
    {
        const Math::vec3f& p0 = supports[i].particle->GetPosition();
        const Math::vec3f& p1 = supports[i].anchor;

        bridgeVertices[(2 * ROD_COUNT) + (2 * CABLE_COUNT) + (2 * i)] = Math::vec3f(p0);
        bridgeVertices[(2 * ROD_COUNT) + (2 * CABLE_COUNT) + (2 * i + 1)] = Math::vec3f(p1);
    }

    glBindBuffer(GL_ARRAY_BUFFER, bridgeVertexBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof (GLfloat) * bridgeVertices.size() * 3, &bridgeVertices[0]);
}

void BridgeExample::RenderBridge()
{
    bridgeShader->BindShader();
    bridgeShader->AutoEnableVertexAttribArray();

    glBindBuffer(GL_ARRAY_BUFFER, bridgeVertexBuffer);
    glVertexAttribPointer((GLint) 0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, bridgeColorBuffer);
    glVertexAttribPointer((GLint) 1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    bridgeShader->sendUniform4x4("modelview_matrix", Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(MODELVIEW_MATRIX));
    bridgeShader->sendUniform4x4("projection_matrix", Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(PROJECTION_MATRIX));

    glDrawArrays(GL_LINES, 0, bridgeVertices.size());

    bridgeShader->AutoDisableVertexAttribArray();
}
