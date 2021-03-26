/*
 * ShaderResource.h
 *
 *  Created on: 23/02/2013
 *      Author: Lean
 */

#ifndef SHADERRESOURCE_H_
#define SHADERRESOURCE_H_


#include <Resource.h>

class ShaderResource : public Resource
{
	public:
		ShaderResource(unsigned int id) : Resource(id, "video/vertexShader")
		{

		}
};
#endif /* SHADERRESOURCE_H_ */
