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
};
class PerVertexGLTrianglesNormalGenerator : public NormalGenerator
{
	private:
		Logger *logger;
	public:
		PerVertexGLTrianglesNormalGenerator()
		{
			logger = Logger::getLogger("video/GeometryResourceAdapter.h");
		}
		const void generateNormals(GeometryResource *resource)
		{
			if(!resource->getIndices().empty() && !resource->getVertices().empty())
			{
				logger->debug("Generating normals PER VERTEX / GL_TRIANGLES");

				if(!resource->getNormals().empty())
					resource->getNormals().clear();

				std::vector<unsigned int> divisor;
				for(std::vector<vector3>::iterator iterator = resource->getVertices().begin(); iterator != resource->getVertices().end(); iterator++)
				{
					divisor.push_back(0);
					resource->getNormals().push_back(vector(0, 0 ,0));
				}

				for(unsigned int currentVertexIndex = 0; currentVertexIndex < resource->getIndices().size(); currentVertexIndex+=3)
				{
					vector tangente2 = resource->getVertices()[resource->getIndices()[currentVertexIndex + 2]] - resource->getVertices()[resource->getIndices()[currentVertexIndex]];
					vector tangente1 = resource->getVertices()[resource->getIndices()[currentVertexIndex + 1]] - resource->getVertices()[resource->getIndices()[currentVertexIndex]];
					vector normal = (tangente2 ^ tangente1).Normalizado();

					logger->debug("Current Triangle: [%d], tangente1 = <%.2f, %.2f, %.2f>, tangente2=<%.2f, %.2f, %.2f>, normal=<%.2f, %.2f, %.2f>", currentVertexIndex,
							tangente1.x, tangente1.y, tangente1.z,
							tangente2.x, tangente2.y, tangente2.z,
							normal.x, normal.y, normal.z);

					resource->getNormals()[resource->getIndices()[currentVertexIndex]] += normal;
					divisor[resource->getIndices()[currentVertexIndex]]++;
					resource->getNormals()[resource->getIndices()[currentVertexIndex + 1]] += normal;
					divisor[resource->getIndices()[currentVertexIndex + 1]]++;
					resource->getNormals()[resource->getIndices()[currentVertexIndex + 2]] += normal;
					divisor[resource->getIndices()[currentVertexIndex + 2]]++;
				}

				log("Normals = ", resource->getNormals());
				log("divisors = ", divisor);

				for(unsigned int currentNormalIndex = 0; currentNormalIndex < resource->getNormals().size(); currentNormalIndex++)
				{
					resource->getNormals()[currentNormalIndex] = resource->getNormals()[currentNormalIndex] * (1.0f / divisor[currentNormalIndex]);
				}
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
