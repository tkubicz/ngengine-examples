/* 
 * File:   Background.h
 * Author: tku
 *
 * Created on 25 czerwiec 2014, 20:00
 */

#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "NGE/Geometry/Basic/Sphere.hpp"
#include <NGE/Media/Images/LoadTextureEvent.hpp>

class Background : public NGE::Geometry::Basic::Sphere {
  public:
    /**
     * Initialize the background sphere. This method loads necessary data and
     * assets.
     * @param slices Number of sphere slices.
     * @param radius Radius of the sphere.
     * @return True if everything went ok, otherwise false.
     */
    virtual bool Initialize(int slices, float radius);

    /**
     * Render the background (sphere).
     */
    virtual void Render();
};

#endif /* BACKGROUND_H */

