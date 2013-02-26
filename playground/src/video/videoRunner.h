/*
 * videoRunner.h
 *
 *  Created on: 22/11/2012
 *      Author: Lean
 */

#ifndef VIDEORUNNER_H_
#define VIDEORUNNER_H_

#include <gl\gl.h>

#include "core/playground.h"
#include "math/Math3d.h"

#include "adapters/PngResourceAdapter.h"
#include "adapters/JpegResourceAdapter.h"
#include "adapters/TgaResourceAdapter.h"
#include "adapters/TextureResourceAdapter.h"
#include "adapters/GeometryResourceAdapter.h"
#include "adapters/VertexArrayAdapter.h"
#include "adapters/ShaderResourceAdapter.h"
#include "adapters/ShaderProgramResourceAdapter.h"

#define axis_length 1.0f

#define VERTEX_LOCATION 0
#define INDEX_LOCATION 1
#define NORMAL_LOCATION 2
#define TEXTURE_COORDINATE_LOCATION 3
#define COLOR_LOCATION 4


class VideoRunner: public PlaygroundRunner {
	private:
		Logger *logger;
	public:
		static const unsigned char ID = 1;
	public:
		VideoRunner() {
			logger = Logger::getLogger("video/videoRunner.h");
		}

		virtual unsigned char getId() {
			return VideoRunner::ID;
		}

		virtual boolean init() {
			this->getContainer()->getResourceManager()->addAdapter(new PngResourceAdapter());
			this->getContainer()->getResourceManager()->addAdapter(new JpegResourceAdapter());
			this->getContainer()->getResourceManager()->addAdapter(new TgaResourceAdapter());
			this->getContainer()->getResourceManager()->addAdapter(new TextureResourceAdapter());
			this->getContainer()->getResourceManager()->addAdapter(new GeometryResourceAdapter());
			this->getContainer()->getResourceManager()->addAdapter(new VertexArrayResourceAdapter());
			this->getContainer()->getResourceManager()->addAdapter(new ShaderResourceAdapter());
			this->getContainer()->getResourceManager()->addAdapter(new ShaderProgramResourceAdapter());

			glClearColor(0.0, 0.5, 0.0, 0.0);
			//glShadeModel(GL_FLAT);
			glShadeModel(GL_SMOOTH);
			glEnable(GL_DEPTH_TEST);
			//glEnable(GL_BLEND);
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glEnable(GL_TEXTURE_2D);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			return true;
		}
		virtual void beforeLoop() {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glLoadIdentity();
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

		void glDrawVertexArray(VertexArrayResource *vertexArrayResource)
		{
			if(vertexArrayResource != null) {
				if(vertexArrayResource->getTexture() != null)
					glBindTexture(GL_TEXTURE_2D, vertexArrayResource->getTexture()->getId());
				glBindVertexArray(vertexArrayResource->getId());
				glDrawElements(vertexArrayResource->getPrimitiveType(), vertexArrayResource->getAttribute(1).getCount(), GL_UNSIGNED_INT, 0);
			}

		}

		void glDrawGeometry(GeometryResource *geometry)
		{
			if(geometry != null) {
				glBegin(GL_TRIANGLES);
				for(unsigned int index = 0; index < geometry->getIndexes().size(); index++)
				{
					unsigned int currentIndex = geometry->getIndexes()[index];
					glVertex2fv((float *)geometry->getVertices()[currentIndex]);

					if(currentIndex < geometry->getColors().size())
						glColor3fv((float *)geometry->getColors()[currentIndex]);

					if(currentIndex < geometry->getTextureCoordinates().size())
						glTexCoord2fv((float *)geometry->getTextureCoordinates()[currentIndex]);

					if(currentIndex < geometry->getNormals().size())
						glNormal3f(geometry->getTextureCoordinates()[currentIndex].x, geometry->getTextureCoordinates()[currentIndex].y, 0.0);

				}
				glEnd();
			}
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
