/*
 * JpegResourceAdapter.h
 *
 *  Created on: 25/01/2013
 *      Author: Lean
 */

#ifndef JPEGRESOURCEADAPTER_H_
#define JPEGRESOURCEADAPTER_H_

#include <ijl.h>
#include <stdio.h>
#include "resources/ResourceAdapter.h"
#include "../resources/JpegResource.h"

class JpegResourceAdapter: public ResourceAdapter {
	private:
		std::vector<String> supportedMimeTypes;
		Logger *logger;
		public:
		JpegResourceAdapter() {
			supportedMimeTypes.push_back("image/jpeg");
			supportedMimeTypes.push_back("image/jpg");

			logger = Logger::getLogger("video/JpegResourceAdapter.h");
		}
		virtual const std::vector<String> getSupportedMimeTypes() {
			return supportedMimeTypes;
		}
		virtual Resource *load(FileParser &fileParser) {
			IJLERR jerr;
			JPEG_CORE_PROPERTIES jcprops;

			if((jerr = ijlInit(&jcprops)) != IJL_OK) {
				logger->error("Error al intentar inicializar las estructuras de ijl");
				return(null);
			}

			jcprops.JPGFile = fileParser.getFilename().c_str();
			if((jerr = ijlRead(&jcprops, IJL_JFILE_READPARAMS)) != IJL_OK) {
				logger->error("Error al intentar leer el archivo [%s]", fileParser.getFilename().c_str());
				ijlFree(&jcprops);
				return(null);
			};

			jcprops.JPGSizeBytes =  (jcprops.JPGWidth * 24 + 7) / 8 * jcprops.JPGHeight;
			logger->debug("Archivo [%s]:\n	Dimensiones: [%d]x[%d]\n	Tamaño: [%d]\n", fileParser.getFilename().c_str(), jcprops.JPGWidth, jcprops.JPGHeight, jcprops.JPGSizeBytes);

			JpegResource *resource = new JpegResource(0);

			unsigned char *pBitmap;
			if((pBitmap = new unsigned char[jcprops.JPGSizeBytes]) == null) {
				logger->error("Error al intentar reservar [%d] bytes", jcprops.JPGSizeBytes);
				ijlFree(&jcprops);
				return(null);
			}

			resource->setData(pBitmap);

			resource->setAncho(jcprops.DIBWidth = jcprops.JPGWidth);
			resource->setAlto(jcprops.JPGHeight); //Implies a bottom-up DIB.
			resource->setBpp(24);
			jcprops.DIBHeight = -jcprops.JPGHeight;
			jcprops.DIBColor = IJL_BGR;
			jcprops.DIBPadBytes = IJL_DIB_PAD_BYTES(jcprops.JPGWidth, 3);
			jcprops.DIBBytes = pBitmap;
			jcprops.jquality = 90;

			switch(jcprops.JPGChannels) {
				case 1:
					jcprops.JPGColor = IJL_G;
				break;
				case 3:
					jcprops.JPGColor = IJL_YCBCR;
				break;
				default:
					jcprops.DIBColor = (IJL_COLOR)IJL_OTHER;
					jcprops.JPGColor = (IJL_COLOR)IJL_OTHER;
				break;
			}

			if((jerr = ijlRead(&jcprops, IJL_JFILE_READWHOLEIMAGE)) != IJL_OK) {
				logger->error("Error al intentar decodificar la imagen: [%d]\n", jerr);
				ijlFree(&jcprops);
				return(null);
			}


			ijlFree(&jcprops);

			return(resource);
		}
		virtual void dispose(Resource *resource) {
			JpegResource *jpegResource = (JpegResource *)resource;

			logger->debug("Disposing of jpeg");
			if(jpegResource->getData() != null)
				delete (unsigned char *)jpegResource->getData();
		}

	};

#endif /* JPEGRESOURCEADAPTER_H_ */
