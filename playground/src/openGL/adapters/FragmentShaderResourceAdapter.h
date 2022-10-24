/*
 * FragmentShaderResourceAdapter.h
 *
 *  Created on: Oct 24, 2022
 *      Author: leandro
 */

#ifndef SRC_OPENGL_ADAPTERS_FRAGMENTSHADERRESOURCEADAPTER_H_
#define SRC_OPENGL_ADAPTERS_FRAGMENTSHADERRESOURCEADAPTER_H_


#include "ShaderResourceAdapter.h"

class FragmentShaderResourceAdapter : public ShaderResourceAdapter {
public:
	FragmentShaderResourceAdapter() {
		logger = LoggerFactory::getLogger("video/FragmentShaderResourceAdapter");
		this->produces(MimeTypes::FRAGMENTSHADER);
		this->accepts(MimeTypes::GLSL);
	}
};


#endif /* SRC_OPENGL_ADAPTERS_FRAGMENTSHADERRESOURCEADAPTER_H_ */
