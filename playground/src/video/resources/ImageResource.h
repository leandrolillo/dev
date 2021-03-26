/*
 * ImageResource.h
 *
 *  Created on: 25/01/2013
 *      Author: Lean
 */

#ifndef IMAGERESOURCE_H_
#define IMAGERESOURCE_H_

#include <Resource.h>

class ImageResource : public Resource
{
	private:
		unsigned int alto;
		unsigned int ancho;
		unsigned char bpp;
		void *data;

	public:
		ImageResource(unsigned int id, String mimeType) : Resource(id, mimeType)
		{
			alto = 0;
			ancho = 0;
			bpp = 0;
			data = null;
		}

		unsigned int getAlto() const
		{
			return alto;
		}

		void setAlto(unsigned int alto)
		{
			this->alto = alto;
		}

		unsigned int getAncho() const
		{
			return ancho;
		}

		void setAncho(unsigned int ancho)
		{
			this->ancho = ancho;
		}

		unsigned char getBpp() const
		{
			return bpp;
		}

		void setBpp(unsigned char bpp)
		{
			this->bpp = bpp;
		}

		void* getData() const
		{
			return data;
		}

		void setData(void* data)
		{
			this->data = data;
		}
	};



#endif /* IMAGERESOURCE_H_ */
