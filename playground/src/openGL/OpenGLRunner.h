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

#include "../core/Playground.h"

#include "../video/adapters/PngResourceAdapter.h"
#include "../video/adapters/JpegResourceAdapter.h"
#include "../video/adapters/TgaResourceAdapter.h"
#include "adapters/TextureResourceAdapter.h"
#include "adapters/GeometryResourceAdapter.h"
#include "adapters/VertexArrayAdapter.h"
#include "adapters/ShaderResourceAdapter.h"
#include "adapters/ShaderProgramResourceAdapter.h"
#include "../windowing/SDLRunner.h"

#define axis_length 1.0f

#define VERTEX_LOCATION 0
#define INDEX_LOCATION 1
#define NORMAL_LOCATION 2
#define TEXTURE_COORDINATE_LOCATION 3
#define COLOR_LOCATION 4

#ifndef GL_MAJOR_VERSION
#define GL_MAJOR_VERSION 0x821B
#endif

#ifndef GL_MINOR_VERSION
#define GL_MINOR_VERSION 0x821C
#endif

class OpenGLRunner: public SDLRunner {
	private:
		Logger *logger;
		SDL_GLContext glcontext;
		unsigned int majorVersion;
		unsigned int minorVersion;


	public:
		static const unsigned char ID = 1;
	public:
		OpenGLRunner() {
			logger = Logger::getLogger("video/videoRunner.h");
			glcontext = null;
			majorVersion = 0;
			minorVersion = 0;
		}

		virtual unsigned char getId() {
			return ID;
		}

		virtual bool init() {
			SDLRunner::init();

			this->getContainer()->getResourceManager()->addAdapter(new PngResourceAdapter());
			this->getContainer()->getResourceManager()->addAdapter(new JpegResourceAdapter());
			this->getContainer()->getResourceManager()->addAdapter(new TgaResourceAdapter());
			this->getContainer()->getResourceManager()->addAdapter(new TextureResourceAdapter());
			this->getContainer()->getResourceManager()->addAdapter(new GeometryResourceAdapter());
			this->getContainer()->getResourceManager()->addAdapter(new VertexArrayResourceAdapter());
			this->getContainer()->getResourceManager()->addAdapter(new ShaderResourceAdapter());
			this->getContainer()->getResourceManager()->addAdapter(new ShaderProgramResourceAdapter());

		    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
		    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		    glcontext = SDL_GL_CreateContext(window);

			int majorVersion;
			int minorVersion;

			glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
			glGetIntegerv(GL_MINOR_VERSION, &minorVersion);
			this->majorVersion = (unsigned int)majorVersion;
			this->minorVersion = (unsigned int)minorVersion;

			logger->info("OpenGL [%d].[%d] initialized\nVersion: [%s]\nGLSL Version: [%s]\nGLEW Version [%s]\nVendor: [%s]\nRenderer: [%s]",
					this->majorVersion,
					this->minorVersion,
					glGetString(GL_VERSION),
					glGetString(GL_SHADING_LANGUAGE_VERSION),
					"0",//glewGetString(GLEW_VERSION),
					glGetString(GL_VENDOR),
					glGetString(GL_RENDERER));



			return true;
		}
		virtual void beforeLoop() {
			SDLRunner::beforeLoop();
			//glClearColor( rand() % 255 / 255.0f, rand() % 255 / 255.0f, rand() % 255 / 255.0f, 1.0f );
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

		unsigned int getMajorVersion() const {
			return majorVersion;
		}

		unsigned int getMinorVersion() const {
			return minorVersion;
		}

		/**
		 * Expects [number of index] elements in every vertex array attribute. E.g. You can't have three vertices, six indices and six texture coordinates. See http://www.opengl.org/wiki/Vertex_Buffer_Object#Vertex_Stream
		 */
		void glDrawVertexArray(VertexArrayResource *vertexArrayResource)
		{
			if(vertexArrayResource != null) {
				if(vertexArrayResource->getTexture() != null)
					glBindTexture(GL_TEXTURE_2D, vertexArrayResource->getTexture()->getId());
//				glBindVertexArray(vertexArrayResource->getId());
				glDrawElements(vertexArrayResource->getPrimitiveType(), vertexArrayResource->getAttribute(INDEX_LOCATION).getCount(), GL_UNSIGNED_INT, (GLvoid *)0);
			}

		}

		void glDrawGeometry(GeometryResource *geometry)
		{
			if(geometry != null) {
				glBegin(geometry->getType());
				for(unsigned int index = 0; index < geometry->getIndices().size(); index++)
				{
					unsigned int currentIndex = geometry->getIndices()[index];

					if(currentIndex < geometry->getColors().size())
						glColor3fv((float *)geometry->getColors()[currentIndex]);

					if(currentIndex < geometry->getTextureCoordinates().size())
						glTexCoord2fv((float *)geometry->getTextureCoordinates()[currentIndex]);

					if(currentIndex < geometry->getNormals().size())
						glNormal3fv((float *)geometry->getNormals()[currentIndex]);

					glVertex3fv((float *)geometry->getVertices()[currentIndex]);
				}
				glEnd();
	//				glDisable(GL_LIGHTING);
	//				glColor3f(0.0, 0.0, 0.0);
	//				glBegin(GL_LINES);
	//				for(unsigned int index = 0; index < geometry->getIndices().size(); index++)
	//				{
	//					unsigned int currentIndex = geometry->getIndices()[index];
	//					glVertex3fv((float *)geometry->getVertices()[currentIndex]);
	//					glVertex3fv((float *)(geometry->getVertices()[currentIndex] + geometry->getNormals()[currentIndex]));
	//				}
	//				glEnd();
	//				glEnable(GL_LIGHTING);
			}
		}

		void glSphere(real radius, real dFi = radian(10), real dTita = radian(10))
		{
			real oneOverTwoPi = 1.0f / (2.0f * M_PI);
			real oneOverPi = 1.0f / M_PI;

			glBegin(GL_TRIANGLES);
			for(real fi = 0; fi < radian(360) - dFi; fi += dFi)
			{
				for(real tita = 0; tita < radian(180) - dTita; tita+= dTita)
				{
					vector2 texel1 = vector2(fi * oneOverTwoPi,   tita * oneOverPi);
					vector2 texel2 = vector2(fi * oneOverTwoPi,   (tita + dTita) * oneOverPi);
					vector2 texel3 = vector2((fi + dFi) * oneOverTwoPi,   tita * oneOverPi);
					vector2 texel4 = vector2((fi + dFi) * oneOverTwoPi,   (tita + dTita) * oneOverPi);

					vector3 vertex1 = radius * vector3(radius * sin(tita) * sin(fi), radius * cos(tita), radius * sin(tita) * cos(fi));
					vector3 vertex2 = radius * vector3(radius * sin(tita + dTita) * sin(fi), radius * cos(tita + dTita), radius * sin(tita + dTita) * cos(fi));
					vector3 vertex3 = radius * vector3(radius * sin(tita) * sin(fi + dFi), radius * cos(tita), radius * sin(tita) * cos(fi + dFi));
					vector3 vertex4 = radius * vector3(radius * sin(tita + dTita) * sin(fi + dFi), radius * cos(tita + dTita), radius * sin(tita + dTita) * cos(fi + dFi));

					glNormal3fv((float *)VectorUtilities::normalizar(vertex1));
					glTexCoord2fv((float *)texel1);
					glVertex3fv((float *)vertex1);

					glNormal3fv((float *)VectorUtilities::normalizar(vertex2));
					glTexCoord2fv((float *)texel2);
					glVertex3fv((float *)vertex2);

					glNormal3fv((float *)VectorUtilities::normalizar(vertex3));
					glTexCoord2fv((float *)texel3);
					glVertex3fv((float *)vertex3);

					glNormal3fv((float *)VectorUtilities::normalizar(vertex3));
					glTexCoord2fv((float *)texel3);
					glVertex3fv((float *)vertex3);

					glNormal3fv((float *)VectorUtilities::normalizar(vertex2));
					glTexCoord2fv((float *)texel2);
					glVertex3fv((float *)vertex2);

					glNormal3fv((float *)VectorUtilities::normalizar(vertex4));
					glTexCoord2fv((float *)texel4);
					glVertex3fv((float *)vertex4);
				}
			}
			glEnd();

		}

		void glPlane(vector posicion, vector normal, vector origen,
						float nro_grids, float ancho) {
	//			int pos_x, pos_z;
	//			pos_x = (int) (((int) (posicion.x / ancho + 0.5f)) * ancho);
	//			pos_z = (int) (((int) (posicion.z / ancho + 0.5f)) * ancho);

			float pos_x = posicion.x;
			float pos_z = posicion.z;

			real ancho_plano = nro_grids * ancho;
			real D = -(normal * origen);
			real oneOverB = 1.0 / normal.y;

			glBegin(GL_TRIANGLE_FAN);
				real x = (real)(pos_x - ancho_plano);
				real z = (real) (pos_z - ancho_plano);
				real y = D - (normal.x * (x - ancho_plano) + normal.z * z) * oneOverB;

				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(x, y, z);


				x = (real)(pos_x - ancho_plano);
				z = (real) (pos_z + ancho_plano);
				y = D - (normal.x * (x - ancho_plano) + normal.z * z) * oneOverB;

				glTexCoord2f(ancho_plano, 0.0f);
				glVertex3f(x, y, z);

				x = (real)(pos_x + ancho_plano);
				z = (real) (pos_z + ancho_plano);
				y = D - (normal.x * (x - ancho_plano) + normal.z * z) * oneOverB;

				glTexCoord2f(ancho_plano, ancho_plano);
				glVertex3f(x, y, z);

				x = (real)(pos_x + ancho_plano);
				z = (real) (pos_z - ancho_plano);
				y = D - (normal.x * (x - ancho_plano) + normal.z * z) * oneOverB;

				glTexCoord2f(0.0f, ancho_plano);
				glVertex3f(x, y, z);
			glEnd();
		}
		void glWirePlane(vector posicion, vector normal, vector origen,
				float nro_grids, float ancho) {
			int pos_x, pos_z;
			pos_x = (int) (((int) (posicion.x / ancho + 0.5f)) * ancho);
			pos_z = (int) (((int) (posicion.z / ancho + 0.5f)) * ancho);

			float ancho_plano = nro_grids * ancho;
			real D = -(normal * origen);
			real Cz = normal.z * (pos_z + ancho_plano), Cx = normal.x
					* (pos_x + ancho_plano);
			real menosCz = normal.z * (pos_z - ancho_plano), menosCx = normal.x
					* (pos_x - ancho_plano);
			real y;

			glBegin(GL_LINES);
	//			glColor3f(0.0f, 0.0f, 0.0f);
	//			glVertex3f(0.0f, 0.0f, 0.0f);
	//			glVertex3f(normal.x, normal.y, normal.z);
	//
	//			glColor3f(0.3f, 0.3f, 0.3f);
			for (real x = (real) (pos_x - ancho_plano);
					x < (real) (pos_x + ancho_plano + ancho); x += ancho) {
				y = -(normal.x * x + menosCz + D) / normal.y;
				glVertex3f(x, y, pos_z - ancho_plano);

				y = -(normal.x * x + Cz + D) / normal.y;
				glVertex3f(x, y, pos_z + ancho_plano);
			}

			//glColor3f(0.9f, 0.3f, 0.9f);

			for (real z = (real) (pos_z - ancho_plano);
					z < (real) (pos_z + ancho_plano + ancho); z += ancho) {
				y = -(normal.z * z + menosCx + D) / normal.y;
				glVertex3f(pos_x - ancho_plano, y, z);

				y = -(normal.z * z + Cx + D) / normal.y;
				glVertex3f(pos_x + ancho_plano, y, z);
			}
			glEnd();
		}

		void glAxis() {
			glBegin(GL_LINES);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(axis_length, 0.0f, 0.0f);
			glVertex3f(-axis_length, 0.0f, 0.0f);
			glVertex3f(axis_length - 0.2f, 0.2f, 0.0f);
			glVertex3f(axis_length - 0.2f, -0.2f, 0.0f);

			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f(0.0f, axis_length, 0.0f);
			glVertex3f(0.0f, -axis_length, 0.0f);
			glVertex3f(0.2f, axis_length - 0.2f, 0.0f);
			glVertex3f(-0.2f, axis_length - 0.2f, 0.0f);

			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(0.0f, 0.0f, axis_length);
			glVertex3f(0.0f, 0.0f, -axis_length);
			glVertex3f(0.2f, 0.0f, axis_length - 0.2f);
			glVertex3f(-0.2f, 0.0f, axis_length - 0.2f);
			glEnd();
		}
};

#endif /* VIDEORUNNER_H_ */
