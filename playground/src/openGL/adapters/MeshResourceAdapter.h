/*
 * MeshResourceAdapter.h
 *
 *  Created on: Oct 13, 2022
 *      Author: leandro
 */

#ifndef SRC_OPENGL_ADAPTERS_MESHRESOURCEADAPTER_H_
#define SRC_OPENGL_ADAPTERS_MESHRESOURCEADAPTER_H_

#include <adapters/OpenGLResourceAdapter.h>
#include <MeshResource.h>

class MeshResourceAdapter: public OpenGLResourceAdapter {
public:
	MeshResourceAdapter() {
		logger = LoggerFactory::getLogger("video/MeshResourceAdapter");
		this->produces(MimeTypes::MESH);
	}

	virtual Resource *load(ResourceLoadRequest &request) const override {
	    String geometryMimeType;

		GeometryCollection *geometryCollection = (GeometryCollection*) this->getResourceManager()->load(request.getFileParser(),
				MimeTypes::GEOMETRYCOLLECTION,
				std::set<String> { ResourceManager::EphemeralLabel });

		if (geometryCollection == null || geometryCollection->getObjects().empty()) {
		    logger->error("Could not load geometry from %s with mimetype %s", request.getFilePath().c_str(), geometryMimeType.c_str());
		    return null;
		}

		GeometryResource *geometry = geometryCollection->getObjects().begin()->second;
		MeshResource *resource = new MeshResource();

		resource->setVertexArray(OpenGLUtilites::generateVertexBuffer(geometry));
		if(geometry->getMaterial() != null) {
			resource->setMaterial(geometry->getMaterial());

			if(!geometry->getMaterial()->getAmbientTexture().empty()) {
				TextureResource *texture = (TextureResource *)getResourceManager()->load(geometry->getMaterial()->getDiffuseTexture(), MimeTypes::TEXTURE);
				if(texture != null) {
					resource->setTexture(texture);
				} else {
					logger->warn("Could not load texture [%s]", geometry->getMaterial()->getDiffuseTexture().c_str());
				}
			}

			if(!geometry->getMaterial()->getDiffuseTexture().empty()) {
				TextureResource *texture = (TextureResource *)getResourceManager()->load(geometry->getMaterial()->getDiffuseTexture(), MimeTypes::TEXTURE);
				if(texture != null) {
					resource->setTexture(texture);
				} else {
					logger->warn("Could not load texture [%s]", geometry->getMaterial()->getDiffuseTexture().c_str());
				}
			}

			if(!geometry->getMaterial()->getBumptTexture().empty()) {
				TextureResource *texture = (TextureResource *)getResourceManager()->load(geometry->getMaterial()->getBumptTexture(), MimeTypes::TEXTURE);
				if(texture != null) {
					resource->setNormalMap(texture);
				} else {
					logger->warn("Could not load texture [%s]", geometry->getMaterial()->getBumptTexture().c_str());
				}
			}

			if(!geometry->getMaterial()->getSpecularTexture().empty()) {
				TextureResource *texture = (TextureResource *)getResourceManager()->load(geometry->getMaterial()->getSpecularTexture(), MimeTypes::TEXTURE);
				if(texture != null) {
					resource->setSpecularMap(texture);
				} else {
					logger->warn("Could not load texture [%s]", geometry->getMaterial()->getSpecularTexture().c_str());
				}
			}
		}

		return resource;
	}
};

#endif /* SRC_OPENGL_ADAPTERS_MESHRESOURCEADAPTER_H_ */
