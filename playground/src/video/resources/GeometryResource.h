/*
 * GeometryResource.h
 *
 *  Created on: 07/02/2013
 *      Author: Lean
 */

#ifndef GEOMETRYRESOURCE_H_
#define GEOMETRYRESOURCE_H_

#include <Resource.h>
#include <Math3d.h>
#include <vector>

/**
 * Holds "Geometric" (not true, it also has colors and texture coordinates) data. It does not send anything to the gcp.
 * TODO: Find better name specially since it causes confusion with geometry (coldet).
 */
class GeometryResource: public Resource {
private:
	Logger *logger = LoggerFactory::getLogger("video/GeometryResource");
	std::string name;

	std::vector<vector3> vertices;
	std::vector<unsigned int> indices;
	std::vector<vector3> normals;
	std::vector<vector2> textureCoordinates;
	std::vector<vector3> colors;
	String type;

	vector size {0.0, 0.0, 0.0 };
public:
	GeometryResource(unsigned int id) :
			Resource(id, "video/geometry") {
		type = 4;
	}

	std::vector<vector3>& getNormals() {
		return normals;
	}

	void setNormals(const std::vector<vector3> &normals) {
		this->normals = normals;
	}

	std::vector<vector2>& getTextureCoordinates() {
		return textureCoordinates;
	}

	void setTextureCoordinates(const std::vector<vector2> &texCoordinates) {
		this->textureCoordinates = texCoordinates;
	}

	std::vector<vector3>& getVertices() {
		return vertices;
	}

	void setVertices(const std::vector<vector3> &vertex) {
		this->vertices = vertex;
	}

	std::vector<vector3>& getColors() {
		return colors;
	}

	void setColors(const std::vector<vector3> &colors) {
		this->colors = colors;
	}

	std::vector<unsigned int>& getIndices() {
		return indices;
	}

	void setIndices(const std::vector<unsigned int> &indexes) {
		this->indices = indexes;
	}

	const String &getType() const {
		return type;
	}

	void setType(String type) {
		this->type = type;
	}

	vector getSize() {
		if(size == vector(0, 0, 0)) {
			vector max(REAL_MIN, REAL_MIN, REAL_MIN);
			vector min(REAL_MAX, REAL_MAX, REAL_MAX);
			for(auto &vertice : vertices) {
				max = vector(std::max(max.x, vertice.x), std::max(max.y, vertice.y), std::max(max.z, vertice.z));
				min = vector(std::min(min.x, vertice.x), std::min(min.y, vertice.y), std::min(min.z, vertice.z));
			}
			this->size = max - min;
			logger->info("Size is: %s", size.toString().c_str());
		}

		return size;
	}

	const std::string& getName() const {
		return name;
	}

	void setName(const std::string &name) {
		this->name = name;
	}
};

#endif /* GEOMETRYRESOURCE_H_ */
