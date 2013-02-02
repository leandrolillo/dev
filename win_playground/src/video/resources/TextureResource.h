/*
 * TextureResource.h
 *
 *  Created on: 28/01/2013
 *      Author: Lean
 */

#ifndef TEXTURERESOURCE_H_
#define TEXTURERESOURCE_H_

#include "resources/Resource.h"

class TextureResource : public Resource
{
	public:
		TextureResource(unsigned int id) : Resource(id, "video/texture")
		{

		}
};


#endif /* TEXTURERESOURCE_H_ */
