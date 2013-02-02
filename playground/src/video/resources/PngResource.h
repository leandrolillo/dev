/*
 * PngResource.h
 *
 *  Created on: 25/01/2013
 *      Author: Lean
 */

#ifndef PNGRESOURCE_H_
#define PNGRESOURCE_H_

#include "ImageResource.h"

class PngResource : public ImageResource
{
	public:
		PngResource(unsigned int id) : ImageResource(id, "image/png") {

		}
};



#endif /* PNGRESOURCE_H_ */
