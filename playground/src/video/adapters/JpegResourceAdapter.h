/*
 * JpegResourceAdapter.h
 *
 *  Created on: 25/01/2013
 *      Author: Lean
 */


#ifndef JPEGRESOURCEADAPTER_H_
#define JPEGRESOURCEADAPTER_H_
#include <stdio.h>
#include "../../resources/ResourceAdapter.h"

#include <jpeglib.h>

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

			ImageResource *resource = new ImageResource(0, mimeType);
			resource->setAlto(cinfo.output_height);
			resource->setAncho(cinfo.output_width);
			resource->setBpp(cinfo.output_components);
			resource->setData(new char [resource->getAncho() * resource->getAlto() * resource->getBpp()]);

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

			return(resource);
		}

		virtual void dispose(Resource *resource) {
			ImageResource *jpegResource = (ImageResource *)resource;

			if(jpegResource->getData() != null)
				delete (unsigned char *)jpegResource->getData();
		}

	};
#endif /* JPEGRESOURCEADAPTER_H_ */
