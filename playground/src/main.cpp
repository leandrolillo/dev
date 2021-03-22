#include <iostream>
#include <stdio.h>

#define GL_SILENCE_DEPRECATION
#include <OpenGLRunner.h>
#include <AudioRunner.h>

#include "PlaygroundGeneralDemo.h"

#include<ParticleManager.h>

#define numberOfParticles 256

class PhysicsPlaygroundRunner: public PlaygroundRunner {
	OpenGLRunner *openGl = null;
	AudioRunner *audio = null;

	matriz_4x4 view;
	vector viewPosition;

	ParticleManager particleManager;
	Particle particles[numberOfParticles];

	unsigned long to = 0;
	real invPerformanceFreq = 1.0f;

	/**
	 * stats
	 */
	real elapsedTime = 0.0f;
	unsigned long frames = 0;
public:
	PhysicsPlaygroundRunner() {
		reset();
	}

	unsigned char getId() {
		return 200;
	}

	virtual unsigned char getInterests() {
			return KEY_DOWN | KEY_UP | MOUSE_MOVE | MOUSE_WHEEL;
	}

	void reset() {
		for(Particle *particle = particles; particle < particles + numberOfParticles; particle++) {
			particle->setStatus(false);
		}

		viewPosition = vector(0.0f, 0.0f, -5.0f);

	}

	bool init() {
		openGl = (OpenGLRunner*) this->getContainer()->getRunner(1);
		audio = (AudioRunner*) this->getContainer()->getRunner(3);

		to = openGl->getPerformanceCounter();
		invPerformanceFreq = (real)1 / (real)openGl->getPerformanceFreq();

		glClearColor(0.0, 0.5, 0.0, 0.0);
		glEnable(GL_DEPTH_TEST);

		reset();

		return true;
	}

	LoopResult doLoop() {
		unsigned long tf = openGl->getPerformanceCounter();
		real dt = (real)(tf - to) * invPerformanceFreq;
		to = tf;

		this->particleManager.step(dt);

		openGl->setViewMatrix(matriz_4x4::matrizTraslacion(this->viewPosition));
		openGl->setModelMatrix(matriz_4x4::Identidad);
		openGl->sendMatrices();
		openGl->glAxis(1.0);

		int count = 0;
		for(Particle *particle = particles; particle < particles + numberOfParticles; particle++) {
			if(particle->getStatus()) {
				count++;
				openGl->setModelMatrix(matriz_4x4::matrizTraslacion(particle->getPosition()));

				openGl->sendMatrices();
				openGl->glSphere(0.1);
			}
		}

		/**
		 * stats
		 */
		elapsedTime += dt;
		frames++;
		real fps = (real)frames / elapsedTime;
		printf("Elapsed time: %.2f, frames: %u, fps:%.2f. View position: %s - count = %d\r",
				elapsedTime,
				frames,
				fps,
				viewPosition.toString().c_str(),
				count);

		return CONTINUE;
	}

	void mouseWheel(int wheel) {
		viewPosition += vector(0.0f, 0.0f, wheel);
	}

	virtual void mouseMove(int dx, int dy) {
		viewPosition += vector(0.1f * dx, 0.1f * dy, 0);
	}

	void mouseButtonDown(unsigned char button ) {
		Particle *bullet = null;
		for(Particle *particle = particles; particle < particles + numberOfParticles; particle++) {
			if(!particle->getStatus()) {
				bullet = particle;
				break;
			}
		}

		if(bullet) {
			bullet->setPosition((vector)(view.columna(3)));
			bullet->setVelocity(((vector)view.columna(2)).normalizado() * 35);
			bullet->setMass(2.0f);
			bullet->setDamping(0.99f);
			bullet->setStatus(true);
		}
	}

	virtual void keyDown(unsigned int key) {
		switch (key) {
		case SDLK_ESCAPE:
			this->getContainer()->stop();
			break;
		case SDLK_SPACE:
			reset();
			break;
		case 'f':
		case 'F':
			this->openGl->setFullscreen(!this->openGl->getFullscreen());
			break;
		}
	}

};

class PhysicsPlayground: public Playground {
public:
	PhysicsPlayground(const String &resourcesBasePath) :
			Playground(resourcesBasePath) {
	}
	void init() {
		Playground::init();
		this->addRunner(new OpenGLRunner());
		this->addRunner(new AudioRunner());
		this->addRunner(new PhysicsPlaygroundRunner());
	}
};



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

int main(int, char**){
//	PlaygroundGeneralDemo playground("/Users/leandro/huevadas/projects/dev/media/");
//    playground.run();

	PhysicsPlayground playground("/Users/leandro/huevadas/projects/dev/media/");
    playground.run();

    return 0;
}

