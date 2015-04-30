/* 
 * File:   WaterTexture.h
 * Author: tku
 *
 * Created on 20 czerwiec 2014, 16:03
 */

#ifndef WATERTEXTURE_H
#define	WATERTEXTURE_H

#include <NGE/Geometry/Mesh.hpp>
#include <NGE/Media/Shaders/GLSLProgram.hpp>

class WaterTexture : public NGE::Geometry::Mesh
{
  public:

    struct WaveParameters
    {
        float speed;
        float amplitude;
        float wavelength;
        float steepness;
    };

    struct WaveDirections
    {
        float x;
        float z;
    };

  public:

    WaterTexture() { }
    virtual ~WaterTexture() { }
    
    unsigned int Initialise(float waterPlaneLength);
    void Update(float dt);
    void Resize(int width, int height);
    void Render();
    void Terminate();
    
    void SetShader(NGE::Media::Shaders::GLSLProgram* shader);
    
  protected:
    static const unsigned int textureSize = 1024;
    
    unsigned int fboWaterTexture;
    unsigned int mirrorTexture;
    unsigned int depthMirrorTexture;
    
    NGE::Media::Shaders::GLSLProgram* shader;
    
    float passedTime, waterPlaneLength;
    int parentWidth, parentHeight;
    
};

#endif	/* WATERTEXTURE_H */

