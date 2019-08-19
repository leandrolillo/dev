/*
 * TgaResource.h
 *
 *  Created on: 19/02/2013
 *      Author: Lean
 */

#ifndef TGARESOURCE_H_
#define TGARESOURCE_H_

#include "ImageResource.h"

class TgaResource: public ImageResource {
	public:
		TgaResource(unsigned int id) :
				ImageResource(id, "image/tga") {

		}
};



#endif /* TGARESOURCE_H_ */
