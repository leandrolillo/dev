/*
 * videoRunner.h
 *
 *  Created on: 22/11/2012
 *      Author: Lean
 */

#ifndef OPENGLVIDEORUNNER_H_
#define OPENGLVIDEORUNNER_H_

#include <SDL2/SDL.h>
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>

#include <Playground.h>
#include <VideoRunner.h>
#include <adapters/TextureResourceAdapter.h>
#include <adapters/VertexArrayAdapter.h>
#include <adapters/ShaderResourceAdapter.h>
#include <adapters/ShaderProgramResourceAdapter.h>

#define DEPTH_TEST GL_DEPTH_TEST
#define CULL_FACE GL_CULL_FACE
#define CULL_FACE_BACK GL_BACK

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

int playgroundEventFilter(void *context, SDL_Event *event);

class OpenGLRunner: public VideoRunner {
private:
	Logger *logger = Logger::getLogger("video/openGlRunner");

	SDL_Window *window = null;
	SDL_GLContext glcontext = null;
	unsigned int majorVersion = 0;
	unsigned int minorVersion = 0;

	const ShaderProgramResource *currentShaderProgram = null;
	const ShaderProgramResource *defaultShaderProgram = null;

	VertexArrayResource *axis = null;
	VertexArrayResource *sphere = null;
	VertexArrayResource *box = null;


public:
	static const unsigned char ID = 1;
public:
	virtual unsigned char getId() {
		return ID;
	}

	virtual unsigned char getInterests() {
		return RESIZE;
	}

	virtual ~OpenGLRunner() {
		SDL_DestroyWindow(window);
		logger->debug("OpenGL/SDL Window destroyed");
		SDL_Quit();
		logger->debug("SDL shutdown");
	}

	virtual bool init() {
		VideoRunner::init();
		this->getContainer()->getResourceManager()->addAdapter(
				new TextureResourceAdapter());
		this->getContainer()->getResourceManager()->addAdapter(
				new VertexArrayResourceAdapter());
		this->getContainer()->getResourceManager()->addAdapter(
				new ShaderResourceAdapter());
		this->getContainer()->getResourceManager()->addAdapter(
				new ShaderProgramResourceAdapter());

		if (SDL_Init(SDL_INIT_VIDEO) != 0) {
			logger->error("SDL_Init Error: %s", SDL_GetError());
			return false;
		}

		logger->debug("SDL initialized");

		SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);

		this->window = SDL_CreateWindow("SDL2/OpenGL Demo", 0, 0, 640, 480,
				SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

		logger->debug("SDL Window created");

		SDL_AddEventWatch(playgroundEventFilter, this);

		logger->debug("SDL event watch registered");

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
				SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		glcontext = SDL_GL_CreateContext(window);

		int majorVersion;
		int minorVersion;

		glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
		glGetIntegerv(GL_MINOR_VERSION, &minorVersion);
		this->majorVersion = (unsigned int) majorVersion;
		this->minorVersion = (unsigned int) minorVersion;

		logger->info(
				"OpenGL [%d].[%d] initialized\nVersion: [%s]\nGLSL Version: [%s]\nGLEW Version [%s]\nVendor: [%s]\nRenderer: [%s]",
				this->majorVersion, this->minorVersion, glGetString(GL_VERSION),
				glGetString(GL_SHADING_LANGUAGE_VERSION), "0", //glewGetString(GLEW_VERSION),
				glGetString(GL_VENDOR), glGetString(GL_RENDERER));

		axis =
				(VertexArrayResource*) this->getContainer()->getResourceManager()->load(
						"core/axis.json", "video/vertexArray");

		box =
				(VertexArrayResource*) this->getContainer()->getResourceManager()->load(
						"core/box.json", "video/vertexArray");


		sphere =
				(VertexArrayResource*) this->getContainer()->getResourceManager()->load(
						"core/sphere.json", "video/vertexArray");

		defaultShaderProgram =
				(ShaderProgramResource*) this->getContainer()->getResourceManager()->load(
						"core/simple.program.json", "video/shaderProgram");

		/**
		 * OpenGL defaults so that something is rendered with minimum configuration.
		 */
		this->useProgramResource(defaultShaderProgram);
		this->generateDefaultTexture();

		return true;
	}

	virtual bool afterInit() {
		int height = 0;
		int width = 0;
		SDL_GetWindowSize(window, &width, &height);

		this->getContainer()->resize(height, width);

		return true;
	}

	virtual void beforeLoop() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	LoopResult doLoop() {
		SDL_Event windowEvent;

		if (SDL_PollEvent(&windowEvent)) {
			switch (windowEvent.type) {
			case SDL_QUIT:
				return STOP;
			}
		}

		return CONTINUE;
	}

	virtual void afterLoop() {
		SDL_GL_SwapWindow(window);
	}

	virtual int processEvent(SDL_Event *event) {
		switch (event->type) {
		case SDL_WINDOWEVENT:
			switch (event->window.event) {
			case SDL_WINDOWEVENT_RESIZED:
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				logger->debug("WINDOW RESIZED to [%d, %d]", event->window.data2,
						event->window.data1);
				this->getContainer()->resize(event->window.data2,
						event->window.data1);
				return 0;
			}
			break;
		case SDL_KEYDOWN:
			//SDL_Log("SDL_KEYDOWN %d", event->key.keysym.sym);
			logger->verbose("KEYDOWN: %d", event->key.keysym.sym);
			this->getContainer()->keyDown(event->key.keysym.sym);
			return 0;
		case SDL_KEYUP:
			//SDL_Log("SDL_KEYUP %d", event->key.keysym.sym);
			logger->verbose("KEYUP: %d", event->key.keysym.sym);
			this->getContainer()->keyUp(event->key.keysym.sym);
			return 0;
		case SDL_MOUSEMOTION:
			//SDL_Log("SDL_MOUSEMOTION (%d,%d) delta=(%d,%d)", event->motion.x, event->motion.y, event->motion.xrel, event->motion.yrel);
			this->getContainer()->mouseMove(event->motion.xrel,
					event->motion.yrel);
			logger->verbose("MOUSEMOVE: (%d, %d)", event->motion.xrel,
					event->motion.yrel);
			return 0;
		case SDL_MOUSEBUTTONDOWN:
			//SDL_Log("SDL_MOUSEBUTTONDOWN %d", event->button.button);
			logger->verbose("MOUSEBUTTONDOWN: %d at <%d, %d>", event->button.button, event->button.x, event->button.y);
			this->getContainer()->mouseButtonDown(event->button.button, event->button.x, event->button.y);
			return 0;
		case SDL_MOUSEBUTTONUP:
			//SDL_Log("SDL_MOUSEBUTTONUP %d", event->button.button);
			logger->verbose("MOUSEBUTTONUP: %d at <%d, %d>", event->button.button, event->button.x, event->button.y);
			this->getContainer()->mouseButtonUp(event->button.button, event->button.x, event->button.y);
			return 0;
		case SDL_MOUSEWHEEL:
			//SDL_Log("SDL_MOUSEWHEEL %d %d", event->wheel.direction, event->wheel.y);
			logger->verbose("MOUSEWHEEL: %d", event->wheel.y);
			this->getContainer()->mouseWheel(event->wheel.y);
			return 0;
		}

		return 1;
	}

	unsigned long getPerformanceCounter() const {
		return SDL_GetPerformanceCounter();
	}

	unsigned long getPerformanceFreq() const {
		return SDL_GetPerformanceFrequency();
	}

	void resize(unsigned int height, unsigned int width) {
		VideoRunner::resize(height, width);
		glViewport(0, 0, (GLsizei) width, (GLsizei) height);
	}

	bool setFullscreen(bool fullScreen) {
		logger->info("Setting fullscreen");
		if(VideoRunner::setFullscreen(fullScreen)) {
			logger->info("Calling sdl setwindowfullscreen");
			SDL_SetWindowFullscreen(this->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		} else {
			SDL_SetWindowFullscreen(this->window, 0);
		}

		int height = 0;
		int width = 0;
		SDL_GetWindowSize(window, &width, &height);

		this->getContainer()->resize(height, width);

		return this->getFullscreen();
	}

	unsigned int getMajorVersion() const {
		return majorVersion;
	}

	unsigned int getMinorVersion() const {
		return minorVersion;
	}

	const ShaderProgramResource* getDefaultShaderProgram() const {
		return this->defaultShaderProgram;
	}

	void useProgramResource(const ShaderProgramResource *program) {
		if (program != null) {
			if (currentShaderProgram == null
					|| currentShaderProgram->getId() != program->getId()) {
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
		if (currentShaderProgram != null) {
			String errorMessage;
			glProgramUniform1i(currentShaderProgram->getId(),
					glGetUniformLocation(currentShaderProgram->getId(), name),
					value);
			if (!(errorMessage = getGlError()).empty()) {
				logger->error("Error sending unsigned int %s: %s", name,
						errorMessage.c_str());
				return false;
			}
		}
		return true;
	}

	bool sendMatrix(const char *name, const matriz_4x4 &value) const {
		if (currentShaderProgram != null) {
			String errorMessage;
			glUniformMatrix4fv(
					glGetUniformLocation(currentShaderProgram->getId(), name),
					1, GL_TRUE, (real*) value);
			if (!(errorMessage = getGlError()).empty()) {
				logger->error("Error sending matrix %s: %s", name,
						errorMessage.c_str());
				return false;
			}
		}
		return true;
	}

	bool sendMatrix(const char *name, const matriz_3x3 &value) const {
		if (currentShaderProgram != null) {
			String errorMessage;
			glUniformMatrix3fv(
					glGetUniformLocation(currentShaderProgram->getId(), name),
					1, GL_TRUE, (real*) value);
			if (!(errorMessage = getGlError()).empty()) {
				logger->error("Error sending matrix %s: %s", name,
						errorMessage.c_str());
				return false;
			}
		}
		return true;
	}

	bool sendVector(const char *name, const vector &value) const {
		if (currentShaderProgram != null) {
			String errorMessage;
			glUniform3fv(
					glGetUniformLocation(currentShaderProgram->getId(), name),
					1, (real*) value);
			if (!(errorMessage = getGlError()).empty()) {
				logger->error("Error sending vector %s: %s", name,
						errorMessage.c_str());
				return false;
			}
		}
		return true;
	}

	bool sendReal(const char *name, const real &value) const {
		if (currentShaderProgram != null) {
			String errorMessage;
			glUniform1fv(
					glGetUniformLocation(currentShaderProgram->getId(), name),
					1, &value);
			if (!(errorMessage = getGlError()).empty()) {
				logger->error("Error sending real %s: %s", name,
						errorMessage.c_str());
				return false;
			}
		}
		return true;
	}

	bool sendMatrices() const {
		return sendMatrix("matrices.model", this->modelMatrix)
				&& sendMatrix("matrices.pvm",
						this->projection * this->viewMatrix * this->modelMatrix)
				&& sendMatrix("matrices.normal", this->normalMatrix);
	}

	bool setMaterial(const MaterialResource &material) const {
		return sendVector("material.ambient", material.getAmbient())
				&& sendVector("material.diffuse", material.getDiffuse())
				&& sendVector("material.specular", material.getSpecular())
				&& sendReal("material.shininess", material.getShininess());
	}

	bool setLight(const LightResource &light) const {
		return sendVector("light.ambient",
				light.getAmbient() * light.getShininess())
				&& sendVector("light.diffuse",
						light.getDiffuse() * light.getShininess())
				&& sendVector("light.specular",
						light.getSpecular() * light.getShininess())
				&& sendVector("light.position", light.getPosition());
	}

	void setTexture(unsigned int location, const TextureResource *texture) const {
		if (texture != null) {
			glActiveTexture(GL_TEXTURE0 + location);
			glBindTexture(GL_TEXTURE_2D, texture->getId());
		}
	}

	void setClearColor(real r, real g, real b, real a) const {
		glClearColor(r, g, b, a);
	}

	void setAttribute(unsigned int attributeCode, unsigned int param) const {
		switch (attributeCode) {
		case (DEPTH_TEST):
			if ((bool) param) {
				glEnable(GL_DEPTH_TEST);
			} else {
				glDisable(GL_DEPTH_TEST);
			}
			break;
		case (CULL_FACE):
			glEnable(GL_CULL_FACE);
			glCullFace(param);

			break;
		}
	}

	/**
	 * Expects [number of index] elements in every vertex array attribute. E.g. You can't have three vertices, six indices and six texture coordinates. See http://www.opengl.org/wiki/Vertex_Buffer_Object#Vertex_Stream
	 */
	void drawVertexArray(const VertexArrayResource *vertexArrayResource) const {
		String errorMessage;

		if (vertexArrayResource != null) {
			glEnableVertexAttribArray(0);

			if (vertexArrayResource->getTexture() != null) {
				glBindTexture(GL_TEXTURE_2D,
						vertexArrayResource->getTexture()->getId());
				if (!(errorMessage = getGlError()).empty()) {
					logger->error("Error binding texture: %s",
							errorMessage.c_str());
				}
			}
//			else {
//				glBindTexture(GL_TEXTURE_2D, 0);
//			}


			glBindVertexArray(vertexArrayResource->getId());
			if (!(errorMessage = getGlError()).empty()) {
				logger->error("Error binding vertex array: %s",
						errorMessage.c_str());
			}

			const VertexAttribPointer *indices = vertexArrayResource->getAttribute(INDEX_LOCATION);
			if (indices != null) {
				glDrawElements(vertexArrayResource->getPrimitiveType(),indices->getCount(), GL_UNSIGNED_INT, (GLvoid*) 0);
				if (!(errorMessage = getGlError()).empty()) {
					logger->error("Error drawing elements: %s",
							errorMessage.c_str());
				}
			} else {
				glDrawArrays(vertexArrayResource->getPrimitiveType(), 0, vertexArrayResource->getAttribute(VERTEX_LOCATION)->getCount());
				if (!(errorMessage = getGlError()).empty()) {
					logger->error("Error drawing arrays: %s",
							errorMessage.c_str());
				}
			}

			glDisableVertexAttribArray(0);
		}
	}

	void drawSphere(real radius) const {
		matriz_4x4 newModel = this->modelMatrix
				* matriz_4x4::matrizZoom(radius, radius, radius);
		sendMatrix("matrices.model", newModel);
		sendMatrix("matrices.pvm",
				this->projection * this->viewMatrix * newModel);
		drawVertexArray(sphere);
		sendMatrix("matrices.model", this->modelMatrix);
		sendMatrix("matrices.pvm",
				this->projection * this->viewMatrix * this->modelMatrix);
	}

	void drawBox(real height, real width, real depth) const {
		matriz_4x4 newModel = this->modelMatrix
				* matriz_4x4::matrizZoom(height, width, depth);
		sendMatrix("matrices.model", newModel);
		sendMatrix("matrices.pvm",
				this->projection * this->viewMatrix * newModel);
		drawVertexArray(box);
		sendMatrix("matrices.model", this->modelMatrix);
		sendMatrix("matrices.pvm",
				this->projection * this->viewMatrix * this->modelMatrix);
	}


	void drawPlane(vector posicion, vector normal, vector origen,
			float nro_grids, float ancho) const {
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

	void drawAxis(real length = 1.0f) const {
		matriz_4x4 newModel = this->modelMatrix
				* matriz_4x4::matrizZoom(length, length, length);
		sendMatrix("matrices.model", newModel);
		sendMatrix("matrices.pvm",
				this->projection * this->viewMatrix * newModel);
		drawVertexArray(axis);
		sendMatrix("matrices.model", this->modelMatrix);
		sendMatrix("matrices.pvm",
				this->projection * this->viewMatrix * this->modelMatrix);
	}

protected:
	void generateDefaultTexture() {
		unsigned int textureHandler = 0;
		glGenTextures(1, &textureHandler);

		GLubyte data[] = { 255, 255, 255, 255 };

		glBindTexture(GL_TEXTURE_2D, ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, data);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureHandler);
	}

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
	unsigned int asGlPrimitive(const String &typeString) {
		if (typeString == "points")
			return GL_POINTS;
		else if (typeString == "points")
			return GL_LINES;
		else if (typeString == "lineLoop")
			return GL_LINE_LOOP;
		else if (typeString == "lineStrip")
			return GL_LINE_STRIP;
		else if (typeString == "lines")
			return GL_LINES;
		else if (typeString == "triangles")
			return GL_TRIANGLES;
		else if (typeString == "triangleStrip")
			return GL_TRIANGLE_STRIP;
		else if (typeString == "triangleFan")
			return GL_TRIANGLE_FAN;
		else if (typeString == "quads")
			return GL_QUADS;
		else if (typeString == "triangleFan")
			return GL_TRIANGLE_FAN;
		else
			throw InvalidArgumentException("Invalid primitive type: [%s]",
					typeString.c_str());
	}
};

int playgroundEventFilter(void *context, SDL_Event *event) {
	OpenGLRunner *runner = (OpenGLRunner*) context;
	return runner->processEvent(event);
}

#endif /* VIDEORUNNER_H_ */
