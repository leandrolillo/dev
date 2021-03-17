/*
 * videoRunner.h
 *
 *  Created on: 22/11/2012
 *      Author: Lean
 */

#ifndef VIDEORUNNER_H_
#define VIDEORUNNER_H_

#include <OpenGL/OpenGL.h>
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>

#include <Playground.h>

#include <adapters/PngResourceAdapter.h>
#include <adapters/JpegResourceAdapter.h>
#include <adapters/TgaResourceAdapter.h>
#include <adapters/TextureResourceAdapter.h>
#include <adapters/GeometryResourceAdapter.h>
#include <adapters/VertexArrayAdapter.h>
#include <adapters/ShaderResourceAdapter.h>
#include <adapters/ShaderProgramResourceAdapter.h>
#include <resources/ShaderProgramResource.h>
#include <resources/LightResource.h>

#include <SDLRunner.h>

#include <Math3d.h>

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

		ShaderProgramResource *currentShaderProgram;

		ShaderProgramResource *defaultShaderProgram;

		VertexArrayResource *sphere;
		VertexArrayResource *axis;

		matriz_4x4 projection, viewMatrix, projectionViewMatrix, modelMatrix;
		matriz_3x3 normalMatrix;
	public:
		static const unsigned char ID = 1;
	public:
		OpenGLRunner() {
			logger = Logger::getLogger("video/videoRunner.h");
			glcontext = null;
			majorVersion = 0;
			minorVersion = 0;
			currentShaderProgram = null;
			defaultShaderProgram = null;

			sphere = null;
			axis = null;
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

			sphere = (VertexArrayResource *)this->getContainer()->getResourceManager()->load("core/sphere.json", "video/vertexArray");
			axis = (VertexArrayResource *)this->getContainer()->getResourceManager()->load("core/axis.json", "video/vertexArray");
			defaultShaderProgram = (ShaderProgramResource *)this->getContainer()->getResourceManager()->load("core/simple.program.json", "video/shaderProgram");

			this->useProgramResource(defaultShaderProgram);

			return true;
		}
		virtual void beforeLoop() {
			SDLRunner::beforeLoop();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


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
			//logger->debug("glViewPort(0, 0, %d, %d", width, height);
			glPerspective(45.0, (GLfloat) width / (GLfloat) height, 0.1, 100.0);
		}

		void glPerspective(double fovy, double aspect, double zNear, double zFar)
		{
			double  fW, fH;
			fH = tan( fovy / 360.0 * M_PI ) * zNear;
			fW = fH * aspect;
			double left = -fW;
			double right = fW;
			double bottom = -fH;
			double top = fH;

			// from glFrustrum manpage
			projection(0, 0) = 2.0 * zNear / (right - left); 	projection(0, 1) = 0.0; 							projection(0, 2) = (right + left) / (right - left); 								projection(0, 3) = 0.0;
			projection(1, 0) = 0.0; 							projection(1, 1) = 2.0 * zNear / (top - bottom); 	projection(1, 2) = (top + bottom) / (top - bottom); 	projection(1, 3) = 0.0;
			projection(2, 0) = 0.0; 							projection(2, 1) = 0.0; 							projection(2, 2) = - (zFar + zNear) / (zFar - zNear); 	projection(2, 3) = - (2.0 * zFar * zNear) / (zFar - zNear);
			projection(3, 0) = 0.0; 							projection(3, 1) = 0.0; 							projection(3, 2) = -1.0; 								projection(3, 3) = 0.0;
		}

		const matriz_4x4 &getProjectionMatrix() const {
			return this->projection;
		}

		void setViewMatrix(const matriz_4x4 &viewMatrix) {
			this->viewMatrix = viewMatrix;
			this->projectionViewMatrix = this->projection * this->viewMatrix;
		}

		void setModelMatrix(const matriz_4x4 &modelMatrix) {
			this->modelMatrix = modelMatrix;
			this->normalMatrix = ((matriz_3x3)modelMatrix).inversa().traspuesta();
		}

		unsigned int getMajorVersion() const {
			return majorVersion;
		}

		unsigned int getMinorVersion() const {
			return minorVersion;
		}

		ShaderProgramResource *getDefaultShaderProgram() const {
			return this->defaultShaderProgram;
		}

		void useProgramResource(ShaderProgramResource *program) {
			if(program != null) {
				if(currentShaderProgram == null || currentShaderProgram->getId() != program->getId()) {
					glUseProgram(program->getId());
					currentShaderProgram = program;

#ifdef VERBOSE
					int maxLength = 0;
					int numberOfParams = 0;
					glGetProgramiv(program->getId(), GL_ACTIVE_ATTRIBUTES, &numberOfParams);
					glGetProgramiv(program->getId(), GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);
					char *nameBuffer = new char[maxLength + 1];

					logger->debug("Program [%s] has [%d] attributes:", program->toString().c_str(), numberOfParams);
					for(int index = 0; index < numberOfParams; index++) {
						GLsizei actualLength = 0;
						GLenum type;
						GLint size;
						glGetActiveAttrib(program->getId(), index, maxLength, &actualLength, &size, &type, nameBuffer);


						logger->debug("		Attribute: [%s] at location [%d] of type [%d] and size [%d]", nameBuffer, glGetAttribLocation(program->getId(), nameBuffer), type, size);
					}
					delete [] nameBuffer;

					glGetProgramiv(program->getId(), GL_ACTIVE_UNIFORMS, &numberOfParams);
					glGetProgramiv(program->getId(), GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength);
					nameBuffer = new char[maxLength + 1];

					logger->debug("Program [%s] has [%d] uniforms:", program->toString().c_str(), numberOfParams);
					for(int index = 0; index < numberOfParams; index++) {
						GLsizei actualLength = 0;
						GLenum type;
						GLint size;
						glGetActiveUniform(program->getId(), index, maxLength, &actualLength, &size, &type, nameBuffer);

						logger->debug("		Uniform: [%s] of type [%d] and size [%d]", nameBuffer, type, size);
					}
					delete [] nameBuffer;
#endif
				}
			} else {
				logger->debug("Using program 0");
				glUseProgram(0);
				currentShaderProgram = null;
			}
		}

		bool sendUnsignedInt(const char *name, unsigned int value) const {
			if(currentShaderProgram != null) {
				String errorMessage;
				glProgramUniform1i(currentShaderProgram->getId(), glGetUniformLocation(currentShaderProgram->getId(), name), value);
				if(!(errorMessage = getGlError()).empty()) {
					logger->error("Error sending unsigned int %s: %s", name, errorMessage.c_str());
					return false;
				}
			}
			return true;
		}

		bool sendMatrix(const char *name, const matriz_4x4 &value) const {
			if(currentShaderProgram != null) {
				String errorMessage;
				glUniformMatrix4fv(glGetUniformLocation(currentShaderProgram->getId(), name), 1, GL_TRUE, (real *)value);
				if(!(errorMessage = getGlError()).empty()) {
					logger->error("Error sending matrix %s: %s", name, errorMessage.c_str());
					return false;
				}
			}
			return true;
		}

		bool sendMatrix(const char *name, const matriz_3x3 &value) const {
			if(currentShaderProgram != null) {
				String errorMessage;
				glUniformMatrix3fv(glGetUniformLocation(currentShaderProgram->getId(), name), 1, GL_TRUE, (real *)value);
				if(!(errorMessage = getGlError()).empty()) {
					logger->error("Error sending matrix %s: %s", name, errorMessage.c_str());
					return false;
				}
			}
			return true;
		}

		bool sendVector(const char *name, const vector &value) const {
			if(currentShaderProgram != null) {
				String errorMessage;
				glUniform3fv(glGetUniformLocation(currentShaderProgram->getId(), name), 1, (real *)value);
				if(!(errorMessage = getGlError()).empty()) {
					logger->error("Error sending vector %s: %s", name, errorMessage.c_str());
					return false;
				}
			}
			return true;
		}

		bool sendReal(const char *name, const real &value) const {
			if(currentShaderProgram != null) {
				String errorMessage;
				glUniform1fv(glGetUniformLocation(currentShaderProgram->getId(), name), 1, &value);
				if(!(errorMessage = getGlError()).empty()) {
					logger->error("Error sending real %s: %s", name, errorMessage.c_str());
					return false;
				}
			}
			return true;
		}

		bool sendMatrices() const {
			return sendMatrix("matrices.model", this->modelMatrix) &&
					sendMatrix("matrices.pvm", this->projection * this->viewMatrix * this->modelMatrix) &&
					sendMatrix("matrices.normal", this->normalMatrix);
		}


		bool setMaterial(const MaterialResource &material) const {
			return sendVector("material.ambient", material.getAmbient()) &&
					sendVector("material.diffuse", material.getDiffuse()) &&
					sendVector("material.specular", material.getSpecular()) &&
					sendReal("material.shininess", material.getShininess());
		}

		bool setLight(const LightResource &light) const {
			return sendVector("light.ambient", light.getAmbient() * light.getShininess()) &&
								sendVector("light.diffuse", light.getDiffuse() * light.getShininess()) &&
								sendVector("light.specular", light.getSpecular() * light.getShininess()) &&
								sendVector("light.position", light.getPosition());
		}

		unsigned int asGlPrimitive(const String &typeString) {
			if(typeString == "points")
				return GL_POINTS;
			else if(typeString == "points")
				return GL_LINES;
			else if(typeString == "lineLoop")
				return GL_LINE_LOOP;
			else if(typeString == "lineStrip")
				return GL_LINE_STRIP;
			else if(typeString == "lines")
				return GL_LINES;
			else if(typeString == "triangles")
				return GL_TRIANGLES;
			else if(typeString == "triangleStrip")
				return GL_TRIANGLE_STRIP;
			else if(typeString == "triangleFan")
				return GL_TRIANGLE_FAN;
			else if(typeString == "quads")
				return GL_QUADS;
			else if(typeString == "triangleFan")
				return GL_TRIANGLE_FAN;
			else
				throw InvalidArgumentException("Invalid primitive type: [%s]", typeString.c_str());
		}

		/**
		 * Expects [number of index] elements in every vertex array attribute. E.g. You can't have three vertices, six indices and six texture coordinates. See http://www.opengl.org/wiki/Vertex_Buffer_Object#Vertex_Stream
		 */
		void drawVertexArray(VertexArrayResource *vertexArrayResource) const
		{
			String errorMessage;

			if(vertexArrayResource != null) {
				glEnableVertexAttribArray(0);

				if(vertexArrayResource->getTexture() != null) {
					glBindTexture(GL_TEXTURE_2D, vertexArrayResource->getTexture()->getId());
					if(!(errorMessage = getGlError()).empty()) {
						logger->error("Error binding texture: %s", errorMessage.c_str());
					}
				}

				glBindVertexArray(vertexArrayResource->getId());
				if(!(errorMessage = getGlError()).empty()) {
					logger->error("Error binding vertex array: %s", errorMessage.c_str());
				}

				VertexAttribPointer *indices = vertexArrayResource->getAttribute(INDEX_LOCATION);
				if(indices != null) {
					glDrawElements(vertexArrayResource->getPrimitiveType(), indices->getCount(), GL_UNSIGNED_INT, (GLvoid *)0);
					if(!(errorMessage = getGlError()).empty()) {
						logger->error("Error drawing elements: %s", errorMessage.c_str());
					}
				} else {
					glDrawArrays(vertexArrayResource->getPrimitiveType(), 0, vertexArrayResource->getAttribute(VERTEX_LOCATION)->getCount());
					if(!(errorMessage = getGlError()).empty()) {
						logger->error("Error drawing arrays: %s", errorMessage.c_str());
					}
				}

				glDisableVertexAttribArray(0);
			}
		}

		String getGlError() const {
			String errorMessage;

			GLenum glError;
			while((glError = glGetError()) != GL_NO_ERROR)
			{
				if(errorMessage.size() != 0) {
					errorMessage += ", ";
				}
				if(glError == GL_INVALID_ENUM) {
					errorMessage += "GL_INVALID_ENUM";
				} else if(glError == GL_INVALID_VALUE) {
					errorMessage += "GL_INVALID_VALUE";
				} else if(glError == GL_INVALID_OPERATION) {
					errorMessage += "GL_INVALID_OPERATION";
				} else if(glError == 0x0503) {
					errorMessage += "GL_STACK_OVERFLOW";
				} else if(glError == 0x0504) {
					errorMessage += "GL_STACK_UNDERFLOW";
				} else if(glError == GL_OUT_OF_MEMORY) {
					errorMessage += "GL_OUT_OF_MEMORY";
				} else if(glError == GL_INVALID_FRAMEBUFFER_OPERATION) {
					errorMessage += "GL_INVALID_FRAMEBUFFER_OPERATION";
				} else if(glError == 0x8031) {
					errorMessage += "GL_TABLE_TOO_LARGE";
				} else {
					errorMessage += std::to_string(glError);
				}
			}

			return errorMessage;
		}

		//TODO: Review if required to migrate this method to VBOs
		void glDrawGeometry(GeometryResource *geometry)
		{
//			if(geometry != null) {
//				glBegin(geometry->getType());
//				for(unsigned int index = 0; index < geometry->getIndices().size(); index++)
//				{
//					unsigned int currentIndex = geometry->getIndices()[index];
//
//					if(currentIndex < geometry->getColors().size())
//						glColor3fv((float *)geometry->getColors()[currentIndex]);
//
//					if(currentIndex < geometry->getTextureCoordinates().size())
//						glTexCoord2fv((float *)geometry->getTextureCoordinates()[currentIndex]);
//
//					if(currentIndex < geometry->getNormals().size())
//						glNormal3fv((float *)geometry->getNormals()[currentIndex]);
//
//					glVertex3fv((float *)geometry->getVertices()[currentIndex]);
//				}
//				glEnd();
//	//				glDisable(GL_LIGHTING);
//	//				glColor3f(0.0, 0.0, 0.0);
//	//				glBegin(GL_LINES);
//	//				for(unsigned int index = 0; index < geometry->getIndices().size(); index++)
//	//				{
//	//					unsigned int currentIndex = geometry->getIndices()[index];
//	//					glVertex3fv((float *)geometry->getVertices()[currentIndex]);
//	//					glVertex3fv((float *)(geometry->getVertices()[currentIndex] + geometry->getNormals()[currentIndex]));
//	//				}
//	//				glEnd();
//	//				glEnable(GL_LIGHTING);
//			}
		}

		void glSphere(real radius)
		{
			matriz_4x4 newModel = this->modelMatrix * matriz_4x4::matrizZoom(radius, radius, radius);
			sendMatrix("matrices.model", newModel);
			sendMatrix("matrices.pvm", this->projection * this->viewMatrix * newModel);
			drawVertexArray(sphere);
			sendMatrix("matrices.model", this->modelMatrix);
			sendMatrix("matrices.pvm", this->projection * this->viewMatrix * this->modelMatrix);
		}

		void glPlane(vector posicion, vector normal, vector origen,
						float nro_grids, float ancho) {
//	//			int pos_x, pos_z;
//	//			pos_x = (int) (((int) (posicion.x / ancho + 0.5f)) * ancho);
//	//			pos_z = (int) (((int) (posicion.z / ancho + 0.5f)) * ancho);
//
//			float pos_x = posicion.x;
//			float pos_z = posicion.z;
//
//			real ancho_plano = nro_grids * ancho;
//			real D = -(normal * origen);
//			real oneOverB = 1.0 / normal.y;
//
//			glBegin(GL_TRIANGLE_FAN);
//				real x = (real)(pos_x - ancho_plano);
//				real z = (real) (pos_z - ancho_plano);
//				real y = D - (normal.x * (x - ancho_plano) + normal.z * z) * oneOverB;
//
//				glTexCoord2f(0.0f, 0.0f);
//				glVertex3f(x, y, z);
//
//
//				x = (real)(pos_x - ancho_plano);
//				z = (real) (pos_z + ancho_plano);
//				y = D - (normal.x * (x - ancho_plano) + normal.z * z) * oneOverB;
//
//				glTexCoord2f(ancho_plano, 0.0f);
//				glVertex3f(x, y, z);
//
//				x = (real)(pos_x + ancho_plano);
//				z = (real) (pos_z + ancho_plano);
//				y = D - (normal.x * (x - ancho_plano) + normal.z * z) * oneOverB;
//
//				glTexCoord2f(ancho_plano, ancho_plano);
//				glVertex3f(x, y, z);
//
//				x = (real)(pos_x + ancho_plano);
//				z = (real) (pos_z - ancho_plano);
//				y = D - (normal.x * (x - ancho_plano) + normal.z * z) * oneOverB;
//
//				glTexCoord2f(0.0f, ancho_plano);
//				glVertex3f(x, y, z);
//			glEnd();
		}
		void glWirePlane(vector posicion, vector normal, vector origen,
				float nro_grids, float ancho) {
//			int pos_x, pos_z;
//			pos_x = (int) (((int) (posicion.x / ancho + 0.5f)) * ancho);
//			pos_z = (int) (((int) (posicion.z / ancho + 0.5f)) * ancho);
//
//			float ancho_plano = nro_grids * ancho;
//			real D = -(normal * origen);
//			real Cz = normal.z * (pos_z + ancho_plano), Cx = normal.x
//					* (pos_x + ancho_plano);
//			real menosCz = normal.z * (pos_z - ancho_plano), menosCx = normal.x
//					* (pos_x - ancho_plano);
//			real y;
//
//			glBegin(GL_LINES);
//	//			glColor3f(0.0f, 0.0f, 0.0f);
//	//			glVertex3f(0.0f, 0.0f, 0.0f);
//	//			glVertex3f(normal.x, normal.y, normal.z);
//	//
//	//			glColor3f(0.3f, 0.3f, 0.3f);
//			for (real x = (real) (pos_x - ancho_plano);
//					x < (real) (pos_x + ancho_plano + ancho); x += ancho) {
//				y = -(normal.x * x + menosCz + D) / normal.y;
//				glVertex3f(x, y, pos_z - ancho_plano);
//
//				y = -(normal.x * x + Cz + D) / normal.y;
//				glVertex3f(x, y, pos_z + ancho_plano);
//			}
//
//			//glColor3f(0.9f, 0.3f, 0.9f);
//
//			for (real z = (real) (pos_z - ancho_plano);
//					z < (real) (pos_z + ancho_plano + ancho); z += ancho) {
//				y = -(normal.z * z + menosCx + D) / normal.y;
//				glVertex3f(pos_x - ancho_plano, y, z);
//
//				y = -(normal.z * z + Cx + D) / normal.y;
//				glVertex3f(pos_x + ancho_plano, y, z);
//			}
//			glEnd();
		}

		void glAxis(real length = 1.0f) {
			matriz_4x4 newModel = this->modelMatrix * matriz_4x4::matrizZoom(length, length, length);
			sendMatrix("matrices.model", newModel);
			sendMatrix("matrices.pvm", this->projection * this->viewMatrix * newModel);
			drawVertexArray(axis);
			sendMatrix("matrices.model", this->modelMatrix);
			sendMatrix("matrices.pvm", this->projection * this->viewMatrix * this->modelMatrix);
		}
};

#endif /* VIDEORUNNER_H_ */
