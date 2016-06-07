/* 
 * File:   SubdivisionGeometryExample.h
 * Author: tku
 *
 * Created on 7 June 2016, 12:28
 */

#ifndef SUBDIVISIONGEOMETRYEXAMPLE_H
#define SUBDIVISIONGEOMETRYEXAMPLE_H

#include <NGE/Windows/Application.hpp>
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

	void OnKeyPressed(NGE::Events::KeyboardEvent& event) override { }
	void OnMouse(NGE::Events::MouseEvent& event) override { }
	void OnMouseDrag(int x, int y) override { }
	
  private:
	Media::Shaders::GLSLProgram* shader;
};

#endif /* SUBDIVISIONGEOMETRYEXAMPLE_H */

