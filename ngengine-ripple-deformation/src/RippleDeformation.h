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

using namespace NGE;

class RippleDeformation : public NGE::Windows::Application {
  public:

	RippleDeformation() { }

	~RippleDeformation() { }

	bool Init() override;

	void Prepare(float dt) override { }
	void Render() override;
	void Shutdown() override;

	virtual void OnResize(int width, int height) override;

	void OnKeyPressed(NGE::Events::KeyboardEvent& event) override { }

	void OnMouse(NGE::Events::MouseEvent& event) override { }

	void OnMouseDrag(int x, int y) override { }

  private:
	void CreateTopology();

  private:
	Media::Shaders::GLSLProgram* shader;

	GLuint vba;
	GLuint vbo;
};

#endif /* RIPPLEDEFORMATION_H */

