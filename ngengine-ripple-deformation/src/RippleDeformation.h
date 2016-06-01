/* 
 * File:   RippleDeformation.h
 * Author: tku
 *
 * Created on 30 May 2016, 16:06
 */

#ifndef RIPPLEDEFORMATION_H
#define RIPPLEDEFORMATION_H

#include <NGE/Windows/Application.hpp>
#include <NGE/Media/Shaders/GLSLProgram.hpp>
#include <NGE/Math/Math.hpp>

using namespace NGE;

class RippleDeformation : public NGE::Windows::Application {
  public:

	RippleDeformation() { }

	~RippleDeformation() { }

	bool Init() override;

	void Prepare(float dt) override;
	void Render() override;
	void Shutdown() override;

	virtual void OnResize(int width, int height) override;

	void OnKeyPressed(NGE::Events::KeyboardEvent& event) override { }

	void OnMouse(NGE::Events::MouseEvent& event) override;

	void OnMouseDrag(int x, int y) override { }

  private:
	void CreateTopology();
	void CreateBufferObjects();

  private:
	/**
	 * Total quads on x axis.
	 */
	static constexpr int NUM_X = 40;
	/**
	 * Total quads on z axis.
	 */
	static constexpr int NUM_Z = 40;

	/**
	 * Size of the plane in world space on x axis.
	 */
	static constexpr float SIZE_X = 4;

	/**
	 * Size of the plane in world space on z axis.
	 */
	static constexpr float SIZE_Z = 4;
	static constexpr float HALF_SIZE_X = SIZE_X / 2.0f;
	static constexpr float HALF_SIZE_Z = SIZE_Z / 2.0f;

	/**
	 * Ripple displacement speed
	 */
	static constexpr float SPEED = 2;

	Math::vec3f vertices[(NUM_X + 1) * (NUM_Z + 1)];

	static constexpr int TOTAL_INDICES = NUM_X * NUM_Z * 2 * 3;

	GLushort indices[TOTAL_INDICES];

	// Camera transformation variables.
	int state = 0, oldX = 0, oldY = 0;
	float rX = 25, rY = 40, dist = -7;

	// Projection and modelview matrix.
	Math::mat4f P;
	Math::mat4f MV;

	float time = 0;

	Media::Shaders::GLSLProgram* shader;

	GLuint vaoID;
	GLuint vboVerticesID, vboIndicesID;
};

#endif /* RIPPLEDEFORMATION_H */

