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
	public:
		GeometryResource(unsigned int id) : Resource(id, "video/geometry")
		{

		}
};



#endif /* GEOMETRYRESOURCE_H_ */
