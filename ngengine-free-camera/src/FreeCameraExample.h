/* 
 * File:   FreeCameraExampleExample.h
 * Author: tku
 *
 * Created on 27 September 2016, 16:06
 */

#ifndef FREECAMERAEXAMPLE_H
#define FREECAMERAEXAMPLE_H

#include <NGE/Windows/Application.hpp>
#include <NGE/Math/Vector2.hpp>
#include <NGE/Rendering/Camera/FreeCamera.hpp>

using namespace NGE;

namespace c = NGE::Rendering::Camera;
namespace m = NGE::Math;

class FreeCameraExample : public NGE::Windows::Application {
  public:

	FreeCameraExample() { }
	~FreeCameraExample() { }

	bool Init() override;

	void Prepare(float dt) override;
	void Render() override;
	void Shutdown() override;

	virtual void OnResize(int width, int height) override;

	void OnKeyPressed(NGE::Events::KeyboardEvent& event) override { }
	void OnMouse(NGE::Events::MouseEvent& event) override;
	void OnMouseDrag(int x, int y) override { }
    
  protected:
    c::FreeCamera camera;
    
    // Mouse filtering support variables.
    static constexpr float MOUSE_FILTER_WEIGHT = 0.75f;
    static constexpr int MOUSE_HISTORY_BUFFER_SIZE = 10;
    
    // Mouse history buffer.
    m::vec2f mouseHistory[MOUSE_HISTORY_BUFFER_SIZE];
    
    // Filtered mouse values.
    float mouseX = 0, mouseY = 0;
    
  protected:
    void FilterMouseMove(float dx, float dy);
};

#endif /* FREECAMERAEXAMPLE_H */

