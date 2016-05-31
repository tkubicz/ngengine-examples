/* 
 * File:   ColoredTriangleExample.h
 * Author: tku
 *
 * Created on 30 May 2016, 16:06
 */

#ifndef COLOREDTRIANGLEEXAMPLE_H
#define COLOREDTRIANGLEEXAMPLE_H

#include <NGE/Windows/Application.hpp>
#include <NGE/Math/Math.hpp>
#include <NGE/Rendering/Camera/Camera.hpp>
#include <NGE/Geometry/Basic/Floor.hpp>
#include <NGE/Geometry/Basic/Sphere.hpp>
#include <NGE/Events/IEventData.hpp>

using namespace NGE;

class ColoredTriangleExample : public NGE::Windows::Application {

	struct Vertex {
		Math::vec3f position;
		Math::vec3f color;
	};

  public:
	ColoredTriangleExample();
	~ColoredTriangleExample();

	bool Init() override;
	void Prepare(float dt) override {}
	void Render() override;
	void Shutdown() override;

	virtual void OnResize(int width, int height) override;

	void OnKeyPressed(NGE::Events::KeyboardEvent& event) override {}
	void OnMouse(NGE::Events::MouseEvent& event) override {}
	void OnMouseDrag(int x, int y) override {}

  private:

	Media::Shaders::GLSLProgram* shader;

	Math::mat4f modelViewMatrix;
	Math::mat4f projectionMatrix;

	Vertex vertices[3];
	unsigned short indices[3];

	GLuint vaoID;
	GLuint vboVerticesID;
	GLuint vboIndicesID;
};

#endif /* COLOREDTRIANGLEEXAMPLE_H */

