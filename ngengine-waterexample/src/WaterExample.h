/* 
 * File:   WaterExample.h
 * Author: tku
 *
 * Created on 16 czerwiec 2014, 17:37
 */

#ifndef WATEREXAMPLE_H
#define	WATEREXAMPLE_H

#include <NGE/Windows/Application.hpp>
#include <NGE/Rendering/Camera/Camera.hpp>
#include <NGE/Geometry/Basic/Floor.hpp>
#include "NGE/Geometry/Basic/Sphere.hpp"
#include "WaterTexture.h"
#include "Background.h"

using namespace NGE;

class WaterExample : public NGE::Windows::Application
{
  public:
    WaterExample();
    ~WaterExample();

    bool Init();
    void Prepare(float dt);
    void Render();
    void Shutdown();
    
    virtual void OnResize(int width, int height);

    void OnKeyPressed(NGE::Events::KeyboardEvent& event);
    void OnMouse(NGE::Events::MouseEvent& event);
    void OnMouseDrag(int x, int y);
    
    void MouseListener(IEventDataPtr event);

  private:
    static const int waterPlaneLength = 256;

    GLuint vb, ib, va;
    NGE::Media::Shaders::GLSLProgram* waterShader;
    WaterTexture waterTexture;
    NGE::Media::Images::Texture tex;

    NGE::Rendering::Camera::Camera camera;
    NGE::Geometry::Basic::Floor floor;
    Background background;
   
    void CreateGeometry();
    void CreateWaterCubemap();
    
    void RenderWater();
    
    float passedTime;
    float angle;
};

#endif	/* WATEREXAMPLE_H */

