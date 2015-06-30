/* 
 * File:   Water.h
 * Author: tku
 *
 * Created on 27 czerwiec 2014, 16:18
 */

#ifndef WATER_H
#define	WATER_H

#include <NGE/Geometry/Mesh.hpp>
#include <NGE/Media/Shaders/GLSLProgram.hpp>
#include <NGE/Media/Images/Texture.hpp>

class Water : public NGE::Geometry::Mesh
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

    /**
     * Constructor of the water plane class.
     * @param waterPlaneLength Height and width of the water plane.
     */
    Water(unsigned int waterPlaneLength);

    /**
     * Initialize the water plane. This method will create geometry, vertex buffer objects
     * and vertex array. It takes shader and texture as a parameters, but it is not necessary
     * to provide them when this method is called. 
     * @param texture Pointer to the water plane texture.
     * @param shader Pointer to the water plane shader.
     * @return true if everything went ok, otherwise false.
     */
    virtual bool Initialize(NGE::Media::Images::Texture* texture, NGE::Media::Shaders::GLSLProgram* shader);

    /**
     * Remove geometry from memory and delete VBOs and VBA. This method do not
     * removes water shader or texture.
     */
    virtual void Deinitialize();

    /**
     * Update water plane according to passed time.
     * @param dt Delta time.
     */
    virtual void Update(float dt);

    /**
     * Render the water plane.
     */
    virtual void Render();

    /**
     * Set shader used to render water plane.
     * @param shader Pointer to the shader.
     */
    virtual void SetShader(NGE::Media::Shaders::GLSLProgram* shader);

    /**
     * Set texture used for water plane.
     * @param texture Pointer to the texture.
     */
    virtual void SetTexture(NGE::Media::Images::Texture* texture);

    /**
     * Get the water plane length.
     * @return Water plane length.
     */
    virtual unsigned int GetWaterPlaneLength();

  protected:
    /**
     * Height and width of the water plane.
     */
    unsigned int waterPlaneLength;

    /**
     * Time passed from water plane intialization.
     */
    float passedTime;

    /**
     * Pointer to the water shader.
     */
    NGE::Media::Shaders::GLSLProgram* waterShader;

    /**
     * Pointer to the water surface texture.
     */
    NGE::Media::Images::Texture* waterTexture;
    
    /**
     * Pointer to the background texture.
     */
    NGE::Media::Images::Texture* backgroundTexture;

    /**
     * Array of vertices of the water plane.
     */
    float* vertices;

    /**
     * Array of indices of the water plane.
     */
    unsigned int* indices;

  protected:
    /**
     * Build and initialize water plane geometry.
     */
    virtual void InitializeGeometry();

    /**
     * Initialize vertex buffer objects.
     */
    virtual void InitializeVBO();

    /**
     * Initialize vertex array.
     */
    virtual void InitializeVBA();
};

#endif	/* WATER_H */

