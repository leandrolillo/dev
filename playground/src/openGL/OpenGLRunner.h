/*
 * videoRunner.h
 *
 *  Created on: 22/11/2012
 *      Author: Lean
 */

#ifndef VIDEORUNNER_H_
#define VIDEORUNNER_H_

#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>

#include "../core/playground.h"

//#include "adapters/PngResourceAdapter.h"
//#include "adapters/JpegResourceAdapter.h"
//#include "adapters/TgaResourceAdapter.h"
//#include "adapters/TextureResourceAdapter.h"
//#include "adapters/GeometryResourceAdapter.h"
//#include "adapters/VertexArrayAdapter.h"
//#include "adapters/ShaderResourceAdapter.h"
//#include "adapters/ShaderProgramResourceAdapter.h"
#include "../windowing/SDLRunner.h"

#define axis_length 1.0f

#define VERTEX_LOCATION 0
#define INDEX_LOCATION 1
#define NORMAL_LOCATION 2
#define TEXTURE_COORDINATE_LOCATION 3
#define COLOR_LOCATION 4


class OpenGLRunner: public SDLRunner {
	private:
		Logger *logger;
		SDL_GLContext glcontext;

	public:
		static const unsigned char ID = 1;
	public:
		OpenGLRunner() {
			logger = Logger::getLogger("video/videoRunner.h");
			glcontext = null;
		}

		virtual unsigned char getId() {
			return ID;
		}

		virtual boolean init() {
			SDLRunner::init();

//			this->getContainer()->getResourceManager()->addAdapter(new PngResourceAdapter());
//			this->getContainer()->getResourceManager()->addAdapter(new JpegResourceAdapter());
//			this->getContainer()->getResourceManager()->addAdapter(new TgaResourceAdapter());
//			this->getContainer()->getResourceManager()->addAdapter(new TextureResourceAdapter());
//			this->getContainer()->getResourceManager()->addAdapter(new GeometryResourceAdapter());
//			this->getContainer()->getResourceManager()->addAdapter(new VertexArrayResourceAdapter());
//			this->getContainer()->getResourceManager()->addAdapter(new ShaderResourceAdapter());
//			this->getContainer()->getResourceManager()->addAdapter(new ShaderProgramResourceAdapter());

		    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
		    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		    glcontext = SDL_GL_CreateContext(window);


			return true;
		}
		virtual void beforeLoop() {
			SDLRunner::beforeLoop();
			glClearColor( rand() % 255 / 255.0f, rand() % 255 / 255.0f, rand() % 255 / 255.0f, 1.0f );
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glLoadIdentity();
		}

		virtual void afterLoop() {
			SDLRunner::afterLoop();
			SDL_GL_SwapWindow( window );
		}

		virtual unsigned char getInterests() {
			return RESIZE;
		}

		void resize(unsigned int height, unsigned int width) {
			glViewport(0, 0, (GLsizei) width, (GLsizei) height);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glPerspective(60.0, (GLfloat) width / (GLfloat) height, 1.0,
					200.0);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

		}

		void glPerspective(double fovy, double aspect, double zNear, double zFar)
		{
			GLdouble fW, fH;
			fH = tan( fovy / 360.0 * M_PI ) * zNear;
			fW = fH * aspect;
			glFrustum( -fW, fW, -fH, fH, zNear, zFar );
		}
};

#endif /* VIDEORUNNER_H_ */
