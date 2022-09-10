/*
 * PhysicsDemo.h
 *
 *  Created on: Mar 22, 2021
 *      Author: leandro
 */

#ifndef SRC_COLDETDEMO_H_
#define SRC_COLDETDEMO_H_

#include <iostream>
#include <stdio.h>

#define GL_SILENCE_DEPRECATION
#include <OpenGLRunner.h>
#include <AudioRunner.h>
#include<renderers/DefaultRenderer.h>
#include<renderers/SkyboxRenderer.h>
#include<renderers/GridRenderer.h>

#include<Math3d.h>
#include<forces/Gravity.h>

#include<Geometry.h>
#include<geometry/DrawableSphere.h>
#include<geometry/DrawableAABB.h>
#include<geometry/DrawableHierarchy.h>
#include"../geometry/intersection/CollisionTester.h"

#include<InvalidArgumentException.h>
#include<Camera.h>

class CollisionDetectionDemoRunner;

class CollidingParticle: public Particle {
private:
    CollisionDetectionDemoRunner *runner = null;
    bool _isColliding = false;
    bool _isSelected = false;
    MaterialResource material;

public:
    CollidingParticle(Geometry * geometry) : Particle(geometry), material(vector(0, 0, 0), vector(0, 0, 0), vector(0, 0, 0), 2.0) {
    }

    const MaterialResource &getMaterial() const {
        return this->material;
    }

    void setMaterial(const MaterialResource &material) {
        this->material = material;
    }

    void setIsColliding(bool isColliding) {
        this->_isColliding = isColliding;
    }

    bool isColliding() const {
        return this->_isColliding;
    }

    bool isSelected() const {
        return this->_isSelected;
    }

    void setSelected(bool selected) {
        this->_isSelected = selected;
    }

    void setRunner(CollisionDetectionDemoRunner *runner);
    void afterIntegrate(real dt);
    void onCollision(const ParticleContact &contact);
};

class CollisionDetectionDemoRunner: public PlaygroundRunner {
    Logger *logger = LoggerFactory::getLogger("CollisionDetectionDemoRunner");
    VideoRunner *video = null;
    AudioRunner *audio = null;

    CollisionDetector collisionDetector;
    CollisionTester intersectionTester;
    ContactResolver contactSolver;
    std::vector<Particle*> allParticles; // collect all particles for sending to collision detector.
    std::vector<std::unique_ptr<CollidingParticle>> collidingParticles;
    Particle ground;
    //Plane plane = Plane(vector(0, 0, 0), vector(0, 1, 0));

    vector2 startPosition;
    vector2 endPosition;

    Camera camera;
    DefaultRenderer defaultRenderer;
    SkyboxRenderer skyboxRenderer;
    GridRenderer gridRenderer;

    MaterialResource red = MaterialResource(vector(1, 0, 0), vector(1, 0, 0), vector(1, 0, 0), 1.0, 0.5);
    MaterialResource green = MaterialResource(vector(0, 1, 0), vector(0, 1, 0), vector(0, 1, 0), 1.0);
    MaterialResource blue = MaterialResource(vector(0, 0, 1), vector(0, 0, 1), vector(0, 0, 1), 1.0);

public:
    CollisionDetectionDemoRunner() : ground(new Plane(vector(0, 0, 0), vector(0, 1, 0))){
        logger->addAppender(LoggerFactory::getAppender("stdout"));
    }

    unsigned char getId() {
        return 200;
    }

    virtual unsigned char getInterests() {
        return KEY_DOWN | KEY_UP | MOUSE_MOVE | MOUSE_WHEEL | MOUSE_BUTTON_DOWN | MOUSE_BUTTON_UP | RESIZE;
    }

    void resize(unsigned int height, unsigned int width) {
        camera.setProjectionMatrix(Camera::perspectiveProjection(45.0, (GLfloat) width / (GLfloat) height, 0.1, 300.0));
    }

    void reset() {
//		video->setMousePosition(video->getScreenWidth() >> 1, video->getScreenHeight() >> 1);
        camera.setViewMatrix(matriz_4x4::traslacion(vector(0.0f, -0.5f, -5.0f)));

        real radius = (real) 0.5;
        if(collidingParticles.size() > 0) {
            collidingParticles[0]->setPosition(vector(-1, 0, 0));
            ((Sphere*) collidingParticles[0]->getGeometry())->setRadius(radius);
            collidingParticles[0]->setMass(M_PI * radius * radius);
            collidingParticles[0]->setVelocity(vector(1, 1, 0));
            collidingParticles[0]->setMaterial(blue);
        }

        if(collidingParticles.size() > 1) {
            collidingParticles[1]->setPosition(vector(1, 0, 0));
            ((Sphere*) collidingParticles[1]->getGeometry())->setRadius(radius);
            collidingParticles[1]->setMass(M_PI * radius * radius);
            collidingParticles[1]->setVelocity(vector(-1, -1, 0));
            collidingParticles[1]->setMaterial(green);
        }

        if(collidingParticles.size() > 2) {
            collidingParticles[2]->setPosition(vector(1, 0, 0));
            ((AABB*) collidingParticles[2]->getGeometry())->setHalfSizes(vector(0.5, 0.5, 0.5));
            collidingParticles[2]->setMass(1);
            collidingParticles[2]->setVelocity(vector(-1, -1, 0));
            collidingParticles[2]->setMaterial(green);
        }

        if(collidingParticles.size() > 3) {
            collidingParticles[3]->setPosition(vector(-2, 1, 0));
            collidingParticles[3]->setMass(1);
            collidingParticles[3]->setVelocity(vector(-1, -1, 0));
            collidingParticles[3]->setMaterial(green);
        }

        ground.setInverseMass((real)0);
    }

    bool init() {
        video = (VideoRunner*) this->getContainer()->getRequiredRunner(VideoRunner::ID);
        audio = (AudioRunner*) this->getContainer()->getRequiredRunner(AudioRunner::ID);

        logger->debug("Initializing renderers");
        defaultRenderer.setVideoRunner(video);
        gridRenderer.setVideoRunner(video);
//	    skyboxRenderer.setVideoRunner(video);
//	    skyboxRenderer.setSize(200);

        logger->debug("Setting up video %d", video);
        video->setClearColor(0.0, 0.5, 0.0, 0.0);
        video->enable(DEPTH_TEST, true);
        video->enable(CULL_FACE, CULL_FACE_BACK);
        video->enable(BLEND, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        collidingParticles.push_back(std::unique_ptr<CollidingParticle>(new CollidingParticle(new DrawableSphere(vector(0, 0, 0), (real) 0.5))));
        collidingParticles.back()->setRunner(this);
        allParticles.push_back(collidingParticles.back().get());

        collidingParticles.push_back(std::unique_ptr<CollidingParticle>(new CollidingParticle(new DrawableSphere(vector(0, 0, 0), (real) 0.5))));
        collidingParticles.back()->setRunner(this);
        allParticles.push_back(collidingParticles.back().get());

        collidingParticles.push_back(std::unique_ptr<CollidingParticle>(new CollidingParticle(new DrawableAABB(vector(2, 2, 0), vector(0.5, 0.5, 0.5)))));
        collidingParticles.back()->setRunner(this);
        allParticles.push_back(collidingParticles.back().get());

        HierarchicalGeometry *hierarchicalGeometry = new DrawableHierarchy(new AABB(vector(0, 0, 0), vector(1, 0.5, 0.5)));
        hierarchicalGeometry->addChildren(new Sphere(vector(-0.5, 0, 0), 0.5));
        hierarchicalGeometry->addChildren(new Sphere(vector(0.5, 0, 0), 0.5));

        collidingParticles.push_back(std::unique_ptr<CollidingParticle>(new CollidingParticle(hierarchicalGeometry)));
        collidingParticles.back()->setRunner(this);
        allParticles.push_back(collidingParticles.back().get());

        //collisionDetector.addScenery(&plane);
        allParticles.push_back(&ground);

        reset();

        logger->debug("Completed initialization");
        return true;
    }

    void drawContact(ParticleContact &contact) {
        vector start = contact.getIntersection(); //contact.getParticleA()->getPosition();
        vector end = start + contact.getNormal() * contact.getPenetration();

        defaultRenderer.setMaterial(&green);
        defaultRenderer.drawLine(matriz_4x4::identidad, start, end);

//        if (contact.getParticleB() != null) {
//            start = contact.getIntersection();
//            end = start - contact.getNormal() * contact.getPenetration();
//
//            defaultRenderer.setMaterial(&green);
//            defaultRenderer.drawLine(matriz_4x4::identidad, start, end);
//        }
    }

    LoopResult doLoop() {
        defaultRenderer.clear();
        defaultRenderer.drawAxes(matriz_4x4::identidad);
        defaultRenderer.drawLine(matriz_4x4::identidad, vector(-1, 0, 0), vector(1, 0, 0));
        defaultRenderer.drawLine(matriz_4x4::identidad, vector(0, -1, 0), vector(0, 1, 0));
        defaultRenderer.drawLine(matriz_4x4::identidad, vector(0, 0, -1), vector(0, 0, 1));

        std::vector<ParticleContact> contacts = collisionDetector.detectCollisions(allParticles);

        //defaultRenderer.setMaterial(&blue);
        for (auto contact : contacts) {
            drawContact(contact);
            //Draw contacts
        }

        //logger->info("Found %d contacts.", contacts.size());

        for (auto &particle : collidingParticles) {
            defaultRenderer.setMaterial(particle->isColliding() ? &red : &particle->getMaterial());

            const Drawable* drawableGeometry = dynamic_cast<const Drawable*>(particle->getGeometry());
            drawableGeometry->draw(defaultRenderer);

            defaultRenderer.setMaterial(&blue);
            defaultRenderer.drawLine(matriz_4x4::identidad, particle->getPosition(), particle->getPosition() + particle->getVelocity());

//	        defaultRenderer.setMaterial(&blue);
//          defaultRenderer.drawLine(matriz_4x4::identidad, vector(0, 0, 0), sphereParticle->getPosition());
//	        defaultRenderer.drawLine(matriz_4x4::identidad, vector(0, 0, 0), sphereParticle->getPosition() + vector(0, 0, sphere->getRadius()));

            particle->setIsColliding(false);
        }

        defaultRenderer.render(camera);
        //skyboxRenderer.render(camera);
        gridRenderer.render(camera);

        return LoopResult::CONTINUE;
    }

    void onCollision(CollidingParticle *sphereParticle) {
        sphereParticle->setIsColliding(true);
    }

    void afterIntegrate(CollidingParticle *particle) {
        if (particle->getPosition().modulo() > 100) {
            particle->setStatus(false);
        }

        particle->setIsColliding(false);
    }

    void mouseWheel(int wheel) {
//        camera.setViewMatrix(matriz_4x4::matrizTraslacion(camera.getPosition() + vector(0.0f, 0.0f, wheel)));
//        audio->updateListener(camera.getPosition() * -1);
        //logger->info("camera: %s", camera.getPosition().toString("%.2f").c_str());
    }

    virtual void mouseMove(int x, int y, int dx, int dy) {
            Line line(-camera.getPosition(),
                    camera.getRayDirection((unsigned int) x, (unsigned int) y, video->getScreenWidth(), video->getScreenHeight()));

            if (!equalsZero(line.getDirection().z)) {
                for(auto &particle : collidingParticles) {

                if (particle->isSelected()) {
                    vector origin = particle->getGeometry()->getOrigin();

                    real t = (origin.z - line.getOrigin().z) / line.getDirection().z;
                    particle->setPosition(line.getOrigin() + t * line.getDirection());
                }
            }
        }
    }

    void mouseButtonUp(unsigned char button, int x, int y) {
        if (button == SDL_BUTTON_LEFT) {
            for (auto &particle : collidingParticles) {
                particle->setSelected(false);
            }
        }
    }

    void mouseButtonDown(unsigned char button, int x, int y) {
        if (button == SDL_BUTTON_LEFT) {
            this->startPosition = vector2(x, y);

            Line line(-camera.getPosition(),
                    camera.getRayDirection((unsigned int) x, (unsigned int) y, video->getScreenWidth(), video->getScreenHeight()));

            for(auto &particle : collidingParticles) {
                if (intersectionTester.intersects(*particle->getGeometry(), (Geometry &)line)) {
                    particle->setSelected(true);
                }
            }
        }
    }

    virtual void keyDown(unsigned int key, unsigned int keyModifier) {
        switch (key) {
            case SDLK_BACKSPACE:
                reset();
                break;
            case SDLK_SPACE:
                contactSolver.resolve(collisionDetector.detectCollisions(allParticles), 0.1);
                break;
        }
    }

};

void CollidingParticle::setRunner(CollisionDetectionDemoRunner *runner) {
    this->runner = runner;
}

void CollidingParticle::afterIntegrate(real dt) {
    if (runner != null) {
        runner->afterIntegrate(this);
    }
}

void CollidingParticle::onCollision(const ParticleContact &contact) {
    if (runner != null) {
        runner->onCollision(this);
    }

}

class CollisionDetectionPlayground: public Playground {
public:
    CollisionDetectionPlayground(const String &resourcesBasePath) : Playground(resourcesBasePath) {
    }
    void init() {
        Playground::init();
        this->addRunner(new OpenGLRunner());
        this->addRunner(new AudioRunner());
        this->addRunner(new CollisionDetectionDemoRunner());
    }
};

#endif
