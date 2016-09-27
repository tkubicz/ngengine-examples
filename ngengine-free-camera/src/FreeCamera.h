/* 
 * File:   FreeCamera.h
 * Author: tku
 *
 * Created on 27 September 2016, 16:06
 */

#ifndef FREECAMERA_H
#define FREECAMERA_H

#include <NGE/Windows/Application.hpp>

using namespace NGE;

class FreeCamera : public NGE::Windows::Application {
  public:

	FreeCamera() { }
	~FreeCamera() { }

	bool Init() override;

	void Prepare(float dt) override { }
	void Render() override;
	void Shutdown() override;

	virtual void OnResize(int width, int height) override;

	void OnKeyPressed(NGE::Events::KeyboardEvent& event) override { }
	void OnMouse(NGE::Events::MouseEvent& event) override { }
	void OnMouseDrag(int x, int y) override { }
};

#endif /* FREECAMERA_H */

