/*
 * NormalGenerator.h
 *
 *  Created on: 18/03/2013
 *      Author: Lean
 */

#ifndef NORMALGENERATOR_H_
#define NORMALGENERATOR_H_

#include"../../resources/GeometryResource.h"
#include"log/Logger.h"
#include <gl/gl.h>

class NormalGenerator
{
	public:
		virtual const void generateNormals(GeometryResource *resource) = 0;

	protected:
	virtual const void compressVertices(std::vector<vector3> &unCompressed, std::vector<vector3> &compressed, std::vector<unsigned int> &indices)
	{
		for(std::vector<vector3>::iterator source = unCompressed.begin(); source != unCompressed.end(); source ++)
				{
					unsigned int index = 0;
					boolean preExisting = false;
					for(std::vector<vector3>::iterator destination = compressed.begin(); destination != compressed.end(); destination++)
					{
						if(*source == *destination) {
							indices.push_back(index);
							preExisting = true;
							break;
						}
						index++;
					}
					if(!preExisting) {
						compressed.push_back(*source);
						indices.push_back(compressed.size() - 1);
					}
				}

	}
};
class PerVertexGLTrianglesNormalGenerator : public NormalGenerator
{
	private:
		Logger *logger;
	public:
		PerVertexGLTrianglesNormalGenerator()
		{
			logger = Logger::getLogger("video/normalGenerator/PerVertexGLTrianglesNormalGenerator.h");
		}
		const void generateNormals(GeometryResource *resource)
		{
			if(!resource->getVertices().empty())
			{
				std::vector<vector3> normals;
				std::vector<vector3> vertices;
				std::vector<unsigned int> indices;

				this->compressVertices(resource->getVertices(), vertices, indices);

				logger->debug("Generating normals PER VERTEX / GL_TRIANGLES");

				log("Vertices original = ", resource->getVertices());
				log("Vertices compressed = ", vertices);
				log("Indices = ", indices);

				std::vector<unsigned int> divisor;
				for(std::vector<vector3>::iterator iterator = vertices.begin(); iterator != vertices.end(); iterator++)
				{
					divisor.push_back(0);
					normals.push_back(vector(0, 0 ,0));
				}

				for(unsigned int currentVertexIndex = 0; currentVertexIndex < indices.size(); currentVertexIndex+=3)
				{
					vector tangente2 = vertices[indices[currentVertexIndex + 2]] - vertices[indices[currentVertexIndex]];
					vector tangente1 = vertices[indices[currentVertexIndex + 1]] - vertices[indices[currentVertexIndex]];
					vector normal = (tangente2 ^ tangente1).Normalizado();

					logger->debug("Current Triangle: [%d], tangente1 = <%.2f, %.2f, %.2f>, tangente2=<%.2f, %.2f, %.2f>, normal=<%.2f, %.2f, %.2f>", currentVertexIndex,
							tangente1.x, tangente1.y, tangente1.z,
							tangente2.x, tangente2.y, tangente2.z,
							normal.x, normal.y, normal.z);

					normals[indices[currentVertexIndex]] += normal;
					divisor[indices[currentVertexIndex]]++;
					normals[indices[currentVertexIndex + 1]] += normal;
					divisor[indices[currentVertexIndex + 1]]++;
					normals[indices[currentVertexIndex + 2]] += normal;
					divisor[indices[currentVertexIndex + 2]]++;
				}

				log("Normals = ", normals);
				log("divisors = ", divisor);

				for(unsigned int currentNormalIndex = 0; currentNormalIndex < normals.size(); currentNormalIndex++)
					normals[currentNormalIndex] =  (normals[currentNormalIndex] * (1.0f / divisor[currentNormalIndex])).Normalizado();

				resource->getNormals().clear();
				for(std::vector<unsigned int>::iterator iterator = indices.begin(); iterator != indices.end(); iterator++)
					resource->getNormals().push_back(normals[(*iterator)]);
			}
		}

	private:
		void log(String prefix, std::vector<vector3> array)
		{
			char vectorBuffer[256];

			prefix.append(" [");

			for(std::vector<vector3>::iterator current = array.begin(); current != array.end(); current++) {
				sprintf(vectorBuffer, "<%.2f, %.2f, %.2f> ", (*current).x, (*current).y, (*current).z);
				prefix.append(vectorBuffer);
			}

			prefix.append(" ]");
			logger->debug(prefix.c_str());

		}

		void log(String prefix, std::vector<unsigned int> array)
		{
			char vectorBuffer[256];

			prefix.append(" [");

			for(std::vector<unsigned int>::iterator current = array.begin(); current != array.end(); current++) {
				sprintf(vectorBuffer, "%d, ", *current);
				prefix.append(vectorBuffer);
			}

			prefix.append(" ]");
			logger->debug(prefix.c_str());

		}
};

class NormalGeneratorFactory
{
	public:
		static NormalGenerator *getGenerator(unsigned int primitiveType)
		{
			switch(primitiveType) {
				case GL_TRIANGLES:
					static PerVertexGLTrianglesNormalGenerator _perVertexGLTrianglesNormalGeneratorInstance;
					return &_perVertexGLTrianglesNormalGeneratorInstance;
			}

			return null;
		}
};



#endif /* NORMALGENERATOR_H_ */
