/*
 * OpenGLResourceAdapter.h
 *
 *  Created on: Mar 31, 2021
 *      Author: leandro
 */

#ifndef SRC_OPENGL_ADAPTERS_OPENGLRESOURCEADAPTER_H_
#define SRC_OPENGL_ADAPTERS_OPENGLRESOURCEADAPTER_H_

#include<ResourceAdapter.h>
#include <OpenGL/gl3.h>

class OpenGLResourceAdapter : public ResourceAdapter {
protected:
    String getGlError() const {
        String errorMessage;

        GLenum glError;
        while ((glError = glGetError()) != GL_NO_ERROR) {
            if (errorMessage.size() != 0) {
                errorMessage += ", ";
            }
            if (glError == GL_INVALID_ENUM) {
                errorMessage += "GL_INVALID_ENUM";
            } else if (glError == GL_INVALID_VALUE) {
                errorMessage += "GL_INVALID_VALUE";
            } else if (glError == GL_INVALID_OPERATION) {
                errorMessage += "GL_INVALID_OPERATION";
            } else if (glError == 0x0503) {
                errorMessage += "GL_STACK_OVERFLOW";
            } else if (glError == 0x0504) {
                errorMessage += "GL_STACK_UNDERFLOW";
            } else if (glError == GL_OUT_OF_MEMORY) {
                errorMessage += "GL_OUT_OF_MEMORY";
            } else if (glError == GL_INVALID_FRAMEBUFFER_OPERATION) {
                errorMessage += "GL_INVALID_FRAMEBUFFER_OPERATION";
            } else if (glError == 0x8031) {
                errorMessage += "GL_TABLE_TOO_LARGE";
            } else {
                errorMessage += std::to_string(glError);
            }
        }

        return errorMessage;
    }
};



#endif /* SRC_OPENGL_ADAPTERS_OPENGLRESOURCEADAPTER_H_ */
