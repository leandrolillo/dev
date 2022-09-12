/*
 * GeometryRenderer.h
 *
 *  Created on: Sep 11, 2022
 *      Author: leandro
 */

#ifndef SRC_VIDEO_RENDERERS_GEOMETRY_GEOMETRYRENDERER_H_
#define SRC_VIDEO_RENDERERS_GEOMETRY_GEOMETRYRENDERER_H_

#include <DefaultRenderer.h>
#include <ParticleManager.h>
#include <Geometry.h>

class GeometryRenderer {
	DefaultRenderer &renderer;

    MaterialResource red = MaterialResource(vector(1, 0, 0), vector(1, 0, 0), vector(1, 0, 0), 1.0, 0.5);
    MaterialResource green = MaterialResource(vector(0, 1, 0), vector(0, 1, 0), vector(0, 1, 0), 1.0);
    MaterialResource blue = MaterialResource(vector(0, 0, 1), vector(0, 0, 1), vector(0, 0, 1), 1.0);
    MaterialResource black {vector(0, 0, 0), vector(0, 0, 0), vector(0, 0, 0), 1.0, 0.2 };
    MaterialResource white {vector(1, 1, 1), vector(1, 1, 1), vector(1, 1, 1), 1.0, 0.2 };


public:
	GeometryRenderer(DefaultRenderer &renderer) : renderer(renderer) {		}

	void render(const ParticleManager *particleManager) const {

		for(auto &particle : particleManager->getParticles()) {
			if(particle->getStatus()) {
				bool isColliding = false;
				for(auto &contact : particleManager->getContacts()) {
					if(contact.getParticleA() == particle || contact.getParticleB() ==  particle) {
						isColliding = true;
						break;
					}
				}

				renderer.setMaterial(isColliding ? &red : &black);
				this->render(particle->getBoundingVolume());
			}
		}

		renderer.setMaterial(&white);
		for(auto scenery : particleManager->getScenery()) {
			this->render(scenery);
		}


        renderer.setMaterial(&green);
		for(auto &contact : particleManager->getContacts()) {
			this->renderContact(contact);
		}
	}

	void render(const Geometry *geometry) const {
		if(geometry != null) {
			switch(geometry->getType()) {
				case GeometryType::SPHERE:
					render((const Sphere *)geometry);
				break;
				case GeometryType::AABB:
					render((const AABB *)geometry);
				break;
				case GeometryType::PLANE:
					render((const Plane *)geometry);
				break;
				case GeometryType::HIERARCHY:
					render((const HierarchicalGeometry *)geometry);
				break;
				case GeometryType::HEIGHTMAP:
					render((const HeightMapGeometry *)geometry);
				break;
				case GeometryType::LINE:
					render((const Line *)geometry);
				break;
			}
		}
	}

	void render(const AABB *aabb) const {
        renderer.drawBox(matriz::traslacion(aabb->getOrigin()),
                (real)2 * aabb->getHalfSizes().x,
                (real)2 * aabb->getHalfSizes().y,
                (real)2 * aabb->getHalfSizes().z);
	}

	void render(const Sphere *sphere) const {
		renderer.drawSphere(matriz::traslacion(sphere->getOrigin()), sphere->getRadius());
	}

	void render(const Plane *plane) const {
	}

	void render(const Line *line) const {

	}

	void render(const HeightMapGeometry *heightmap) const {

	}

	void render(const HierarchicalGeometry *hierarchy) const {

		this->render(&hierarchy->getBoundingVolume());
        for(auto &child : hierarchy->getChildren()) {
            this->render(child.get());
        }
	}



	void renderContact(const ParticleContact &contact) const {
        vector start = contact.getIntersection(); //contact.getParticleA()->getPosition();
        vector end = start + contact.getNormal() * contact.getPenetration();
        renderer.drawLine(matriz_4x4::identidad, start, end);
	}
};



#endif /* SRC_VIDEO_RENDERERS_GEOMETRY_GEOMETRYRENDERER_H_ */
