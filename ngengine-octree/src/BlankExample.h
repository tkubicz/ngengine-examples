/* 
 * File:   BlankExample.h
 * Author: tku
 *
 * Created on 30 May 2016, 16:06
 */

#ifndef BLANKEXAMPLE_H
#define BLANKEXAMPLE_H

#include <NGE/Windows/Application.hpp>
#include <NGE/Rendering/Scene/Octree.hpp>
#include <NGE/Rendering/Camera/Camera.hpp>
#include <NGE/Tools/Random.hpp>
#include <NGE/Geometry/Basic/Floor.hpp>
#include <NGE/Geometry/Basic/Sphere.hpp>

using namespace NGE;

class BlankExample : public NGE::Windows::Application {
  public:

	BlankExample() { }
	~BlankExample() { }

	bool Init() override;

	void Prepare(float dt) override;
	void Render() override;
	void Shutdown() override;

	virtual void OnResize(int width, int height) override;

	void OnKeyPressed(NGE::Events::KeyboardEvent& event) override;
	void OnMouse(NGE::Events::MouseEvent& event) override;
	void OnMouseDrag(int x, int y) override { }
	
  private:
	Tools::Random random;
	Rendering::Camera::Camera camera;
	Media::Images::Texture* texture;
	Geometry::Basic::Floor floor;
	Geometry::Basic::Sphere sphere;
	std::vector<Math::vec4f> spherePositions;
	Rendering::Scene::Octree<Math::vec4f>* octree;
	
};

#endif /* BLANKEXAMPLE_H */

