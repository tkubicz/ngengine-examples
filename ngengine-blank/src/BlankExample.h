/* 
 * File:   BlankExample.h
 * Author: tku
 *
 * Created on 30 May 2016, 16:06
 */

#ifndef BLANKEXAMPLE_H
#define	BLANKEXAMPLE_H

#include <NGE/Windows/Application.hpp>
#include <NGE/Rendering/Camera/Camera.hpp>
#include <NGE/Geometry/Basic/Floor.hpp>
#include <NGE/Geometry/Basic/Sphere.hpp>
#include <NGE/Events/IEventData.hpp>

using namespace NGE;

class BlankExample : public NGE::Windows::Application
{
  public:
    BlankExample();
    ~BlankExample();

    bool Init();
    void Prepare(float dt);
    void Render();
    void Shutdown();
    
    virtual void OnResize(int width, int height);

    void OnKeyPressed(NGE::Events::KeyboardEvent& event);
    void OnMouse(NGE::Events::MouseEvent& event);
    void OnMouseDrag(int x, int y);
    
    void MouseListener(NGE::Events::IEventDataPtr event);

  private:

    NGE::Rendering::Camera::Camera camera;
    NGE::Geometry::Basic::Floor floor;
};

#endif	/* BLANKEXAMPLE_H */

