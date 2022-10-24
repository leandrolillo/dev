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
        Logger *logger = LoggerFactory::getLogger("video/image");
		unsigned int alto = 0;
		unsigned int ancho = 0;
		unsigned char bpp = 0;
		unsigned char format = 0; //future storage of rgba, rgb, bgr, bgra, etc.
		void *data = null;

	public:
		ImageResource(unsigned int id, String mimeType) : Resource(id, mimeType)
		{
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
