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
		std::vector<unsigned int> indexes;
		std::vector<vector2> normals;
		std::vector<vector2> textureCoordinates;
		std::vector<vector3> colors;
		String textureFile;
		unsigned int type;
	public:
		GeometryResource(unsigned int id) : Resource(id, "video/geometry")
		{
			textureFile = "";
			type = 4;
		}

		std::vector<vector2>& getNormals() {
			return normals;
		}

		void setNormals(const std::vector<vector2>& normals) {
			this->normals = normals;
		}

		std::vector<vector2>& getTextureCoordinates() {
			return textureCoordinates;
		}

		void setTextureCoordinates(const std::vector<vector2>& texCoordinates) {
			this->textureCoordinates = texCoordinates;
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

		std::vector<vector3>& getColors()
		{
			return colors;
		}

		void setColors(const std::vector<vector3>& colors)
		{
			this->colors = colors;
		}

		std::vector<unsigned int>& getIndexes()
		{
			return indexes;
		}

		void setIndexes(const std::vector<unsigned int>& indexes)
		{
			this->indexes = indexes;
		}

		unsigned int getType() const
		{
			return type;
		}

		void setType(unsigned int type)
		{
			this->type = type;
		}
	};



#endif /* GEOMETRYRESOURCE_H_ */
