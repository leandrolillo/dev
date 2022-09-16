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
public:
    const MaterialResource red = MaterialResource(vector(1, 0, 0), vector(1, 0, 0), vector(1, 0, 0), 1.0, 0.5);
    const MaterialResource green = MaterialResource(vector(0, 1, 0), vector(0, 1, 0), vector(0, 1, 0), 0.5);
    const MaterialResource blue = MaterialResource(vector(0, 0, 1), vector(0, 0, 1), vector(0, 0, 1), 0.5);
    const MaterialResource black {vector(0, 0, 0), vector(0, 0, 0), vector(0, 0, 0), 1.0, 0.2 };
    const MaterialResource white {vector(1, 1, 1), vector(1, 1, 1), vector(1, 1, 1), 1.0, 0.2 };
private:
    DefaultRenderer &renderer;
    MaterialResource const *particleMaterial=&black;
    MaterialResource const *collidingParticleMaterial=&red;
    MaterialResource const *sceneryMaterial=&white;
    MaterialResource const *contactMaterial=&green;


public:
	GeometryRenderer(DefaultRenderer &renderer) : renderer(renderer) {		}

	void setParticleMaterial(MaterialResource *material) {
		this->particleMaterial = material;
	}

	void setCollidingParticleMaterial(MaterialResource *material) {
		this->collidingParticleMaterial = material;
	}

	void setSceneryMaterial(MaterialResource *material) {
		this->sceneryMaterial = material;
	}

	void setContactMaterial(MaterialResource *material) {
		this->contactMaterial = material;
	}

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

				renderer.setMaterial(isColliding ? collidingParticleMaterial : particleMaterial);
				this->render(particle->getBoundingVolume(), isColliding);
			}
		}

		renderer.setMaterial(sceneryMaterial);
		for(auto scenery : particleManager->getScenery()) {
			this->render(scenery);
		}


        renderer.setMaterial(contactMaterial);
		for(auto &contact : particleManager->getContacts()) {
			this->renderContact(contact);
		}
	}

	void render(const Geometry *geometry, bool isColliding = false) const {
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
					render((const HierarchicalGeometry *)geometry, isColliding);
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
		renderer.drawBox(matriz_4x4::base(matriz_3x3::matrizRotacion(0, plane->getNormal()), plane->getOrigin()) , 10, 0.01, 10);
	}

	void render(const Line *line) const {

	}

	void render(const HeightMapGeometry *heightmap) const {

	}

	void render(const HierarchicalGeometry *hierarchy, bool isColliding) const {
		if(isColliding) {
			for(auto &child : hierarchy->getChildren()) {
				this->render(child.get());
			}
		} else {
			this->render(&hierarchy->getBoundingVolume());
		}
	}



	void renderContact(const ParticleContact &contact) const {
        vector start = contact.getIntersection(); //contact.getParticleA()->getPosition();
        vector end = start + contact.getNormal() * contact.getPenetration();
        renderer.drawLine(matriz_4x4::identidad, start, end);
	}
};



#endif /* SRC_VIDEO_RENDERERS_GEOMETRY_GEOMETRYRENDERER_H_ */
