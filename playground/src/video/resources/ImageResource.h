/*
 * ImageResource.h
 *
 *  Created on: 25/01/2013
 *      Author: Lean
 */

#ifndef IMAGERESOURCE_H_
#define IMAGERESOURCE_H_

#include <Resource.h>
#include <Math3d.h>

class ImageResource : public Resource
{
	private:
        Logger *logger = Logger::getLogger("video/ImageResource");
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

		unsigned char getBytespp() const
        {
		    /**
		     * bits divided by 8
		     */
            return bpp * 0.125;
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

		//TODO: Should be a vector3 of integers instead of reals.
		vector getPixel(unsigned int x, unsigned int y) {
		    if(x < this->getAlto() && y <= this->getAncho()) {
                unsigned int position = (y * this->getAncho() + x) * this->getBytespp();

                return vector( ((unsigned char *)this->data)[position],
                        ((unsigned char *)this->data)[position + 1],
                        ((unsigned char *)this->data)[position + 2]
                        );
		    }

		    return vector(0, 0, 0);
		}

        virtual String toString() const {
            return "Image(id:" + std::to_string(this->getId()) + ") [" + this->getMimeType() + "] [" + this->getFileName() + "]: ["
                    + std::to_string(this->getAlto()) + "x" + std::to_string(this->getAlto()) + "x" + std::to_string(this->getBpp()) + "]";
        }
	};



#endif /* IMAGERESOURCE_H_ */
