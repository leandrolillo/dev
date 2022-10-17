/*
 * AssetsResource.h
 *
 *  Created on: Oct 12, 2022
 *      Author: leandro
 */

#ifndef SRC_VIDEO_RESOURCES_GEOMETRYCOLLECTION_H_
#define SRC_VIDEO_RESOURCES_GEOMETRYCOLLECTION_H_

#include <GeometryResource.h>
#include <MaterialResource.h>
#include <unordered_map>

class GeometryCollection : public Resource {
private:
	std::unordered_map<String, GeometryResource *>objects;
public:
	GeometryCollection() : Resource(0, "model/objs") {

	}
	void addObject(GeometryResource *object) {
		if(object != null && !object->getName().empty()) {
			objects[object->getName()] = object;
		}
	}

	GeometryResource *getObject(const String &name) const {
		auto iterator = objects.find(name);
		if(iterator == objects.end()) {
			return null;
		}

		return iterator->second;
	}

	const std::unordered_map<String, GeometryResource*> &getObjects() const {
		return objects;
	}
};


#endif /* SRC_VIDEO_RESOURCES_GEOMETRYCOLLECTION_H_ */
