/* 
 * File:   BridgeExample.h
 * Author: tku
 *
 * Created on 30 May 2016, 16:06
 */

#ifndef BRIDGEEXAMPLE_H
#define BRIDGEEXAMPLE_H

#include <NGE/Windows/Application.hpp>
#include <NGE/Media/Shaders/GLSLProgram.hpp>
#include <NGE/Rendering/Camera/Camera.hpp>
#include <NGE/Events/GUIEventListener.hpp>
#include <NGE/Media//Shaders/GLSLProgram.hpp>
#include <NGE/Physics/MassAggregate/ParticleWorld.hpp>
#include <NGE/Geometry/Basic/Sphere.hpp>
#include <NGE/Geometry/Basic/Floor.hpp>

using namespace NGE;

class BridgeExample : public Windows::Application {
  public:
	BridgeExample();
	virtual ~BridgeExample();

	bool Init();
	void Prepare(float dt);
	void Render();
	void Shutdown();

	void OnKeyPressed(Events::KeyboardEvent& event);
	void OnMouse(Events::MouseEvent& event);
	void OnMouseDrag(int x, int y);

	void OnResize(int width, int height);

  private:
	Media::Shaders::GLSLProgram* shader;
	Media::Shaders::GLSLProgram* floorShader;
	Media::Shaders::GLSLProgram* bridgeShader;

	Rendering::Camera::Camera camera;
	
	Geometry::Basic::Sphere sphere;
	Geometry::Basic::Floor floor;

	GLuint bridgeVertexBuffer, bridgeColorBuffer;
	std::vector<Math::vec3f> bridgeVertices;
	std::vector<Math::vec3f> bridgeColors;
	void GenerateBridge();
	void UpdateBridge();
	void RenderBridge();

	Physics::MassAggregate::ParticleWorld* world;
	unsigned int particleCount;
	Physics::MassAggregate::Particle* particleArray;
	Physics::MassAggregate::GroundContacts groundContactGenerator;

	Physics::MassAggregate::ParticleCableConstraint* supports;
	Physics::MassAggregate::ParticleCable *cables;
	Physics::MassAggregate::ParticleRod *rods;
	Math::vec3f massPos;
	Math::vec3f massDisplayPos;

	//int keyActive, keyNum;
	Events::KeyboardEvent keyEvent;
	bool pause;

	Math::vec3f GRAVITY;
	static const int ROD_COUNT = 6;
	static const int CABLE_COUNT = 10;
	static const int SUPPORT_COUNT = 12;
	static const int BASE_MASS = 1;
	static const int EXTRA_MASS = 10;

	void UpdateAdditionalMass();
};

#endif /* BRIDGEEXAMPLE_H */

