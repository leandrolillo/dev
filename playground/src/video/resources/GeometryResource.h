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
		std::vector<vector3> colors;
		String textureFile;
	public:
		GeometryResource(unsigned int id) : Resource(id, "video/geometry")
		{
			textureFile = "";
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

		std::vector<vector2>& getVertices() {
			return vertices;
		}

		void setVertices(const std::vector<vector2>& vertex) {
			this->vertices = vertex;
		}

		const String& getTextureFile() const
		{
			return textureFile;
		}

		void setTextureFile(const String & textureFile)
		{
			this->textureFile = textureFile;
		}

		const std::vector<vector3>& getColors() const
		{
			return colors;
		}

		void setColors(const std::vector<vector3>& colors)
		{
			this->colors = colors;
		}
	};



#endif /* GEOMETRYRESOURCE_H_ */
