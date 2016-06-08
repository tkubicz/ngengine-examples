/* 
 * File:   SubdivisionGeometryExample.h
 * Author: tku
 *
 * Created on 7 June 2016, 12:28
 */

#ifndef SUBDIVISIONGEOMETRYEXAMPLE_H
#define SUBDIVISIONGEOMETRYEXAMPLE_H

#include <NGE/Windows/Application.hpp>
#include <NGE/Math/Math.hpp>
#include <NGE/Media/Shaders/GLSLProgram.hpp>

using namespace NGE;

class SubdivisionGeometryExample : public NGE::Windows::Application {
  public:

	SubdivisionGeometryExample() { }
	~SubdivisionGeometryExample() { }

	bool Init() override;

	void Prepare(float dt) override { }
	void Render() override;
	void Shutdown() override;

	virtual void OnResize(int width, int height) override;

	void OnKeyPressed(NGE::Events::KeyboardEvent& event) override;
	void OnMouse(NGE::Events::MouseEvent& event) override;
	void OnMouseDrag(int x, int y) override { }
	
  private:
	void CreateGeometry();
	
  private:
	Media::Shaders::GLSLProgram* shader;
	
	static constexpr int WIDTH = 1280;
	static constexpr int HEIGHT = 960;
	
	// Vertex array and vertex buffer object IDs
	GLuint vaoId;
	GLuint vboVerticesId;
	GLuint vboIndicesId;
	
	// Mesh vertices and indices
	Math::vec3f vertices[4];
	GLushort indices[6];
	
	// Projection and modelview matrices
	Math::mat4f P;
	Math::mat4f MVP;
	
	// Camera transformation variables
	int state = 0, oldX = 0, oldY = 0;
	float rX = 25, rY = -40, dist = -35;
	
	int sub_divisions = 1;
};

#endif /* SUBDIVISIONGEOMETRYEXAMPLE_H */

