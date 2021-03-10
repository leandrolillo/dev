#include <iostream>
#include <stdio.h>

#include "openGL/OpenGLRunner.h"

class PhysicsPlayground: public Playground {
private:
	PlaygroundRunner *windowsRunner;
protected:
	PlaygroundRunner *getWindowRunner() {
		if (windowsRunner == null) {
			windowsRunner = new OpenGLRunner();
		}
		return windowsRunner;
	}
public:
	PhysicsPlayground() {
		this->windowsRunner = null;
	}
	void init() {
		Playground::init();

		this->addRunner(this->getWindowRunner());
	}

};

class PlaygroundDemoRunner : public PlaygroundRunner {
	private:
		OpenGLRunner *openGl;
		//AudioRunner *audio;

		Logger *logger;

		//Graphical stuff
		vector viewPosition;
		vector lightPosition;
		vector *currentPosition;
		real rotation;
		TextureResource *textureResource;
		TextureResource *anotherTextureResource;
		VertexArrayResource *vertexArrayResource;
		ShaderProgramResource *shaderProgramResource;
		ShaderProgramResource *anotherShaderProgramResource;
		GeometryResource *geometryResource;
		VertexArrayResource *sphereArrayResource;
		GeometryResource *sphereGeometryResource;
	public:
		static const unsigned char ID = 101;

	public:
		PlaygroundDemoRunner() /*: sphereGeometryResource(0) */{
			openGl = null;
			//audio = null;

			logger = Logger::getLogger("Main.cpp");

			viewPosition = vector(0.0, 0.0f, -6.0);
			lightPosition = vector(0.0, 0.0f, 4.0);
			currentPosition = &viewPosition;
			rotation = 0;
			textureResource = null;
			anotherTextureResource = null;
			vertexArrayResource = null;
			shaderProgramResource = null;
			anotherShaderProgramResource = null;
			geometryResource = null;
			sphereArrayResource = null;
			sphereGeometryResource = null;
		}

		virtual unsigned char getInterests() {
			return KEY_DOWN | KEY_UP | MOUSE_MOVE;
		}

		virtual unsigned char getId() {
			return PlaygroundDemoRunner::ID;
		}

		virtual bool init() {
			//TODO: Review why can´t use public static IDs properties from each class
			openGl = (OpenGLRunner *) this->getContainer()->getRunner(1);
			//audio = (AudioRunner *)this->getContainer()->getRunner(3);

			if(!openGl) {
				logger->error("Could not find openGl runner");
				return false;
			}

			ResourceManager *resourceManager = this->getContainer()->getResourceManager();

			// demo stuff
//			Source *backgroundSource = audio->createSource("background.ogg");
//			audio->playSource(backgroundSource);

			textureResource = (TextureResource *)resourceManager->load("images/TEXTURA.PNG", "video/texture");
			anotherTextureResource = (TextureResource *)resourceManager->load("images/irs.JPG", "video/texture");
			geometryResource = (GeometryResource *)resourceManager->load("geometry/triangle.json", "video/geometry");
			sphereArrayResource = (VertexArrayResource *)resourceManager->load("geometry/sphere.json", "video/vertexArray");
			sphereGeometryResource = (GeometryResource *)resourceManager->load("geometry/sphere.json", "video/geometry");
			vertexArrayResource = (VertexArrayResource *)resourceManager->load("geometry/triangle.json", "video/vertexArray");

//			if(openGl->getMajorVersion() >= 3) {
//				shaderProgramResource = (ShaderProgramResource *)resourceManager->load("shaders/lighting.140.program.json", "video/shaderProgram");
//				anotherShaderProgramResource = (ShaderProgramResource *)resourceManager->load("shaders/toon.140.program.json", "video/shaderProgram");
//			} else {
//				shaderProgramResource = (ShaderProgramResource *)resourceManager->load("shaders/lighting.120.program.json", "video/shaderProgram");
//				anotherShaderProgramResource = (ShaderProgramResource *)resourceManager->load("shaders/toon.120.program.json", "video/shaderProgram");
//
//			}

			glClearColor(0.0, 0.5, 0.0, 0.0);
			glShadeModel(GL_SMOOTH);
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);

//			glEnable(GL_BLEND);
//			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glEnable(GL_TEXTURE_2D);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

			glEnable(GL_LIGHTING);
			//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, (float *)vector4(0.2, 0.2, 0.2));

			glEnable(GL_LIGHT0);
			//glLightfv(GL_LIGHT0, GL_POSITION, (float *)vector4(0.0, 0.0, 0.0, 1.0));
			glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, (float *)vector3(1.0, 1.0, 1.0));
			glLightfv(GL_LIGHT0, GL_SPECULAR, (float *)vector3(0.0, 0.0, 0.0));


			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, (float *)vector3(1.0, 1.0, 1.0));
			glMaterialfv(GL_FRONT, GL_EMISSION, (float *)vector3(0.0, 0.0, 0.0));
			glMaterialfv(GL_FRONT, GL_SPECULAR, (float *)vector3(0.0, 0.0, 0.0));
			glMaterialf(GL_FRONT, GL_SHININESS, 128.0);

			return true;
		}

		GeometryResource buildSphereGeometry()
		{
			GeometryResource resource(0);

			real dFi = radian(10);
			real dTita = radian(10);
			real radius = 1;

			real oneOverTwoPi = 1.0f / (2.0f * M_PI);
			real oneOverPi = 1.0f / M_PI;

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

					resource.getVertices().push_back(vertex1);
					resource.getNormals().push_back(VectorUtilities::normalizar(vertex1));
					resource.getTextureCoordinates().push_back(texel1);
					resource.getIndices().push_back(resource.getIndices().size());
					resource.getColors().push_back(vector3(1.0, 1.0, 1.0));

					resource.getVertices().push_back(vertex2);
					resource.getNormals().push_back(VectorUtilities::normalizar(vertex2));
					resource.getTextureCoordinates().push_back(texel2);
					resource.getIndices().push_back(resource.getIndices().size());
					resource.getColors().push_back(vector3(1.0, 1.0, 1.0));

					resource.getVertices().push_back(vertex3);
					resource.getNormals().push_back(VectorUtilities::normalizar(vertex3));
					resource.getTextureCoordinates().push_back(texel3);
					resource.getIndices().push_back(resource.getIndices().size());
					resource.getColors().push_back(vector3(1.0, 1.0, 1.0));

					resource.getVertices().push_back(vertex3);
					resource.getNormals().push_back(VectorUtilities::normalizar(vertex3));
					resource.getTextureCoordinates().push_back(texel3);
					resource.getIndices().push_back(resource.getIndices().size());
					resource.getColors().push_back(vector3(1.0, 1.0, 1.0));


					resource.getVertices().push_back(vertex2);
					resource.getNormals().push_back(VectorUtilities::normalizar(vertex2));
					resource.getTextureCoordinates().push_back(texel2);
					resource.getIndices().push_back(resource.getIndices().size());
					resource.getColors().push_back(vector3(1.0, 1.0, 1.0));

					resource.getVertices().push_back(vertex4);
					resource.getNormals().push_back(VectorUtilities::normalizar(vertex4));
					resource.getTextureCoordinates().push_back(texel4);
					resource.getIndices().push_back(resource.getIndices().size());
					resource.getColors().push_back(vector3(1.0, 1.0, 1.0));
				}
			}

			return resource;
		}

		virtual LoopResult doLoop() {
//			glUseProgram(0);
//
//			glLoadIdentity();
//			glTranslatef(viewPosition.x, viewPosition.y, viewPosition.z);
//
//			openGl->glAxis();
//
//			glBindTexture(GL_TEXTURE_2D, 0);
//
//			glDisable(GL_LIGHTING);
//			glColor3f(1.0, 1.0, 0.0);
//			glPushMatrix();
//			//light w = 1: spotlight; w = 0 directional light
//			glLightfv(GL_LIGHT0, GL_POSITION, (float *)vector4(lightPosition.x, lightPosition.y, lightPosition.z, 0));
//			glTranslatef(lightPosition.x, lightPosition.y, lightPosition.z);
//			openGl->glSphere(.2);
//			glPopMatrix();
//
//			glEnable(GL_LIGHTING);
//
//			if(anotherTextureResource != null)
//				glBindTexture(GL_TEXTURE_2D, anotherTextureResource->getId());
//
//			glPushMatrix();
//			glTranslatef(-1.5, 2.0, 0.0);
//			glRotatef(rotation, 0.0, 1.0, 0.0);
//			openGl->glDrawGeometry(geometryResource);
//			glPopMatrix();
//
//			glPushMatrix();
//			glTranslatef(1.5, 2.0, 0.0);
//			glRotatef(rotation, 0.0, 1.0, 0.0);
//			openGl->glDrawGeometry(sphereGeometryResource);
//			glPopMatrix();
//
//			//if(anotherShaderProgramResource != null)
//				//glUseProgram(anotherShaderProgramResource->getId());
//
//			glPushMatrix();
//			glTranslatef(-1.5, 0.0, 0.0);
//			glRotatef(rotation, 0.0, 1.0, 0.0);
//			openGl->glDrawGeometry(geometryResource);
//			glPopMatrix();
//
//			glPushMatrix();
//			glTranslatef(1.5, 0.0, 0.0);
//			glRotatef(rotation, 0.0, 1.0, 0.0);
//			openGl->glDrawGeometry(sphereGeometryResource);
//			glPopMatrix();
//
//
//			if(shaderProgramResource != null)
//				glUseProgram(shaderProgramResource->getId());

			glPushMatrix();
			glTranslatef(-1.5, -2.0, 0.0);
			glRotatef(rotation, 0.0, 1.0, 0.0);
			openGl->glDrawVertexArray(vertexArrayResource);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(1.5, -2.0, 0.0);
			glRotatef(rotation, 0.0, 1.0, 0.0);
			openGl->glDrawVertexArray(sphereArrayResource);
			glPopMatrix();

			rotation+=0.1;

			return CONTINUE;
		}
		virtual void mouseMove(int dx, int dy) {
			*currentPosition += vector(0.1f * dx, 0.1f * dy, 0);
		}
		virtual void keyUp(unsigned int key) {
			switch (key) {
			case SDLK_RCTRL:
			case SDLK_LCTRL:
					currentPosition = &viewPosition;
					break;
			}
		}
		virtual void keyDown(unsigned int key) {
			switch (key) {
				case SDLK_RCTRL:
				case SDLK_LCTRL:
					currentPosition = &lightPosition;
					break;
				case SDLK_SPACE:
					viewPosition = vector(0.0, 0.0f, -6.0);
					lightPosition = vector(0.0, 0.0, 4.0);
					break;
				case SDLK_ESCAPE:
					this->getContainer()->stop();
					break;
				case 'f':
				case 'F':
					this->openGl->setFullscreen(!this->openGl->getFullscreen());
				break;
			}
		}

};


class PlaygroundTest: public Playground {
	public:
	PlaygroundTest(const String &resourcesBasePath) : Playground(resourcesBasePath) {
	}

		void init() {
			Playground::init();
			this->addRunner(new PlaygroundDemoRunner());
			this->addRunner(new OpenGLRunner());

		}
};

int main(int, char**){
	PlaygroundTest playground("/Users/leandro/huevadas/projects/dev/media");
    //PhysicsPlayground playground;
    playground.run();
    
    return 0;
}
