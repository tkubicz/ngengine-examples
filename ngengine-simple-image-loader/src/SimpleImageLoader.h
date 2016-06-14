/* 
 * File:   SimpleImageLoader.h
 * Author: tku
 *
 * Created on 30 May 2016, 16:06
 */

#ifndef SIMPLEIMAGELOADER_H
#define SIMPLEIMAGELOADER_H

#include <NGE/Windows/Application.hpp>
#include <NGE/Media/Images/Texture.hpp>
#include <NGE/Media/Shaders/GLSLProgram.hpp>
#include <NGE/Math/Math.hpp>

using namespace NGE;

class SimpleImageLoader : public NGE::Windows::Application {
  public:

	SimpleImageLoader() { }

	~SimpleImageLoader() { }

	bool Init() override;

	void Prepare(float dt) override { }
	void Render() override;
	void Shutdown() override;

	virtual void OnResize(int width, int height) override;

	void OnKeyPressed(NGE::Events::KeyboardEvent& event) override { }

	void OnMouse(NGE::Events::MouseEvent& event) override { }

	void OnMouseDrag(int x, int y) override { }

  private:
	void CreateGeometry();

  private:
	GLuint vaoId;
	GLuint vboVerticesId;
	GLuint vboIndicesId;

	Media::Shaders::GLSLProgram* shader;
	Media::Images::Texture* texture;

	Math::vec2f vertices[4];
	GLushort indices[6];
};

#endif /* SIMPLEIMAGELOADER_H */

