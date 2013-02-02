/*
 * JpegResource.h
 *
 *  Created on: 25/01/2013
 *      Author: Lean
 */

#ifndef JPEGRESOURCE_H_
#define JPEGRESOURCE_H_

#include "ImageResource.h"

class JpegResource: public ImageResource {
	public:
		JpegResource(unsigned int id) :
				ImageResource(id, "image/jpeg") {

		}
};

#endif /* JPEGRESOURCE_H_ */
