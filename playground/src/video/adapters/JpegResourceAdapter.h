/*
 * JpegResourceAdapter.h
 *
 *  Created on: 25/01/2013
 *      Author: Lean
 */


#ifndef JPEGRESOURCEADAPTER_H_
#define JPEGRESOURCEADAPTER_H_

 #ifndef FALSE            /* in case these macros already exist */
 #define FALSE   0        /* values of boolean */
 #endif
 #ifndef TRUE
 #define TRUE    1
 #endif

#include <stdio.h>
#include "../../resources/ResourceAdapter.h"

//#include <jpeglib.h>
#include <SDL_image.h>

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
		String toString() const {
			return logger->getBasename();
		}

		virtual const std::vector<String> getSupportedMimeTypes() {
			return supportedMimeTypes;
		}
		virtual Resource *load(FileParser &fileParser, const String &mimeType) {
			ImageResource *resource = new ImageResource(0, mimeType);

#ifndef JPEGLIB_H
			// load sample.jpg into image
			SDL_RWops *rwop = SDL_RWFromFP(fileParser.getStream(), SDL_FALSE);
			SDL_Surface *image=IMG_LoadJPG_RW(rwop);

			if(!image) {
			    logger->error("Could not load [%s] [%s]: ", mimeType.c_str(), fileParser.getFilename().c_str(), IMG_GetError());
			    // handle error
			}

			resource->setAlto(image->h);
			resource->setAncho(image->w);
			resource->setBpp(image->format->BitsPerPixel);

			unsigned int size = resource->getAncho() * resource->getAlto() * resource->getBpp();
			resource->setData(new char [size]);

			memcpy(resource->getData(), image->pixels, size);
#else
			struct jpeg_decompress_struct cinfo;
			struct jpeg_error_mgr jerr;

			cinfo.err = jpeg_std_error(&jerr);
			jpeg_create_decompress(&cinfo);
			jpeg_stdio_src(&cinfo, fileParser.getStream());
			jpeg_read_header(&cinfo, TRUE);
			jpeg_start_decompress(&cinfo);
			int row_stride = cinfo.output_width * cinfo.output_components;
			  /* Make a one-row-high sample array that will go away when done with image */
			JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

			while (cinfo.output_scanline < cinfo.output_height) {
				/* jpeg_read_scanlines expects an array of pointers to scanlines.
				 * Here the array is only one element long, but you could ask for
				 * more than one scanline at a time if that's more convenient.
				 */
				(void) jpeg_read_scanlines(&cinfo, buffer, 1);
				/* Assume put_scanline_someplace wants a pointer and sample count. */
				//put_scanline_someplace(buffer[0], row_stride);
			  }

			//TODO: Translate cinfo into a resource

			jpeg_finish_decompress(&cinfo);
			jpeg_destroy_decompress(&cinfo);
#endif

			return(resource);

//			IJLERR jerr;
//			JPEG_CORE_PROPERTIES jcprops;
//
//			if((jerr = ijlInit(&jcprops)) != IJL_OK) {
//				logger->error("Error al intentar inicializar las estructuras de ijl: [%s]", ijlErrorStr(jerr));
//				return(null);
//			}
//
//			jcprops.JPGFile = fileParser.getFilename().c_str();
//			if((jerr = ijlRead(&jcprops, IJL_JFILE_READPARAMS)) != IJL_OK) {
//				logger->error("Error al intentar leer el archivo [%s]: [%s]", fileParser.getFilename().c_str(), ijlErrorStr(jerr));
//				ijlFree(&jcprops);
//				return(null);
//			};
//
//			jcprops.JPGSizeBytes =  (jcprops.JPGWidth * 24 + 7) / 8 * jcprops.JPGHeight;
//			logger->debug("Archivo [%s]:\n	Dimensiones: [%d]x[%d]\n	Tama�o: [%d]\n", fileParser.getFilename().c_str(), jcprops.JPGWidth, jcprops.JPGHeight, jcprops.JPGSizeBytes);
//
//			JpegResource *resource = new JpegResource(0);
//
//			unsigned char *pBitmap;
//			if((pBitmap = new unsigned char[jcprops.JPGSizeBytes]) == null) {
//				logger->error("Error al intentar reservar [%d] bytes", jcprops.JPGSizeBytes);
//				ijlFree(&jcprops);
//				return(null);
//			}
//
//			resource->setData(pBitmap);
//
//			resource->setAncho(jcprops.DIBWidth = jcprops.JPGWidth);
//			resource->setAlto(jcprops.JPGHeight); //Implies a bottom-up DIB.
//			resource->setBpp(24);
//			jcprops.DIBHeight = -jcprops.JPGHeight;
//			jcprops.DIBColor = IJL_BGR;
//			jcprops.DIBPadBytes = IJL_DIB_PAD_BYTES(jcprops.JPGWidth, 3);
//			jcprops.DIBBytes = pBitmap;
//			jcprops.jquality = 90;
//
//			switch(jcprops.JPGChannels) {
//				case 1:
//					jcprops.JPGColor = IJL_G;
//				break;
//				case 3:
//					jcprops.JPGColor = IJL_YCBCR;
//				break;
//				default:
//					jcprops.DIBColor = (IJL_COLOR)IJL_OTHER;
//					jcprops.JPGColor = (IJL_COLOR)IJL_OTHER;
//				break;
//			}
//
//			if((jerr = ijlRead(&jcprops, IJL_JFILE_READWHOLEIMAGE)) != IJL_OK) {
//				logger->error("Error al intentar decodificar la imagen: [%d]\n", ijlErrorStr(jerr));
//				ijlFree(&jcprops);
//				return(null);
//			}
//
//
//			ijlFree(&jcprops);

		}
		virtual void dispose(Resource *resource) {
			ImageResource *jpegResource = (ImageResource *)resource;

			logger->debug("Disposing of jpeg");
			if(jpegResource->getData() != null)
				delete (unsigned char *)jpegResource->getData();
		}

	};
#endif /* JPEGRESOURCEADAPTER_H_ */
