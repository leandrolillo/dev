/*
 * TgaResourceAdapter.h
 *
 *  Created on: 19/02/2013
 *      Author: Lean
 */

#ifndef TGARESOURCEADAPTER_H_
#define TGARESOURCEADAPTER_H_

#include <stdio.h>
#include "../resources/ResourceAdapter.h"
#include "../video/resources/ImageResource.h"


class TgaResourceAdapter: public ResourceAdapter {
	private:
		std::vector<String> supportedMimeTypes;
		Logger *logger;
	private:
		short le_short(unsigned char *bytes)
		{
			return bytes[0] | ((char)bytes[1] << 8);
		}
	public:
		TgaResourceAdapter() {
			supportedMimeTypes.push_back("image/tga");

			logger = Logger::getLogger("video/TgaResourceAdapter.h");
		}
		String toString() const {
			return logger->getBasename();
		}
		virtual const std::vector<String> getSupportedMimeTypes() {
			return supportedMimeTypes;
		}
		virtual Resource *load(FileParser &fileParser, const String &mimeType) {
			ImageResource *resource = new ImageResource(0, "image/tga");

			struct tgaHeader {
			   char  id_length;
			   char  color_map_type;
			   char  data_type_code;
			   unsigned char  color_map_origin[2];
			   unsigned char  color_map_length[2];
			   char  color_map_depth;
			   unsigned char  x_origin[2];
			   unsigned char  y_origin[2];
			   unsigned char  width[2];
			   unsigned char  height[2];
			   char  bits_per_pixel;
			   char  image_descriptor;
			} header;

			if (fileParser.read(&header, sizeof(header), 1) != 1) {
				logger->error("%s has incomplete tga header\n", fileParser.getFilename().c_str());
				return null;
			}
			if (header.data_type_code != 2) {
				logger->error("[%s] compressed tga not supported\n", fileParser.getFilename().c_str());
				return null;
			}
			if (header.bits_per_pixel != 24) {
				logger->error("%s is not a 24-bit uncompressed RGB tga file\n", fileParser.getFilename().c_str());
				return null;
			}

			for (int i = 0; i < header.id_length; ++i)
				if (fileParser.takeByte() == EOF) {
					logger->error("%s has incomplete id string\n", fileParser.getFilename().c_str());
					return null;
				}

			unsigned int color_map_size = le_short(header.color_map_length) * (header.color_map_depth/8);
			for (unsigned int i = 0; i < color_map_size; ++i)
				if (fileParser.takeByte() == EOF) {
					logger->error("%s has incomplete color map\n", fileParser.getFilename().c_str());
					return null;
				}

			resource->setAncho(le_short(header.width));
			resource->setAlto(le_short(header.height));
			resource->setBpp(header.bits_per_pixel>>3);

			unsigned int size = resource->getAncho() * resource->getAlto() * resource->getBpp();
			resource->setData(new char [size]);

			if(fileParser.read(resource->getData(), 1, size) != size) {
				logger->error("%s has incomplete image\n", fileParser.getFilename().c_str());
				dispose(resource);
				return null;
			}

			return(resource);
		}
		virtual void dispose(Resource *resource) {
			if(resource->getMimeType() == "image/tga") {
				ImageResource *tgaResource = (ImageResource *)resource;

				logger->debug("Disposing of tga");
				if(tgaResource->getData() != null) {
					delete (unsigned char *)tgaResource->getData();
					tgaResource->setData(null);
				}
			}
		}
	};
#endif /* TGARESOURCEADAPTER_H_ */
