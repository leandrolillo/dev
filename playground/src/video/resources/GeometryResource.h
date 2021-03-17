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

class GeometryResource: public Resource {
private:
	std::vector<vector3> vertices;
	std::vector<unsigned int> indices;
	std::vector<vector3> normals;
	std::vector<vector2> textureCoordinates;
	std::vector<vector3> colors;
	String textureFile;
	unsigned int type;
public:
	GeometryResource(unsigned int id) :
			Resource(id, "video/geometry") {
		textureFile = "";
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

	const String& getTextureFile() const {
		return textureFile;
	}

	void setTextureFile(const String &textureFile) {
		this->textureFile = textureFile;
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

	unsigned int getType() const {
		return type;
	}

	void setType(unsigned int type) {
		this->type = type;
	}
};

#endif /* GEOMETRYRESOURCE_H_ */
