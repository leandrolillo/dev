/*
 * GeometryResource.h
 *
 *  Created on: 07/02/2013
 *      Author: Lean
 */

#ifndef GEOMETRYRESOURCE_H_
#define GEOMETRYRESOURCE_H_

#include "resources/Resource.h"

class GeometryResource : public Resource
{
	private:
		std::vector<vector2> vertices;
		std::vector<vector2> normals;
		std::vector<vector2> textureCoordinates;
		int textureId;
	public:
		GeometryResource(unsigned int id) : Resource(id, "video/geometry")
		{
			textureId = 0;

		}

		const std::vector<vector2>& getNormals() const {
			return normals;
		}

		void setNormals(const std::vector<vector2>& normals) {
			this->normals = normals;
		}

		const std::vector<vector2>& getTextureCoordinates() const {
			return textureCoordinates;
		}

		void setTextureCoordinates(const std::vector<vector2>& textureCoordinates) {
			this->textureCoordinates = textureCoordinates;
		}

		int getTextureId() const {
			return textureId;
		}

		void setTextureId(int textureId) {
			this->textureId = textureId;
		}

		const std::vector<vector2>& getVertices() const {
			return vertices;
		}

		void setVertices(const std::vector<vector2>& vertex) {
			this->vertices = vertex;
		}
};



#endif /* GEOMETRYRESOURCE_H_ */
