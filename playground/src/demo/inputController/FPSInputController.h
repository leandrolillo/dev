/*
 * FPSInputController.h
 *
 *  Created on: Jul 22, 2021
 *      Author: leandro
 */

#ifndef SRC_DEMO_INPUTCONTROLLER_FPSINPUTCONTROLLER_H_
#define SRC_DEMO_INPUTCONTROLLER_FPSINPUTCONTROLLER_H_

#include "InputController.h"
#include <Camera.h>

class FPSInputController : public InputController {
protected:
    real pitch;
    real yaw;
    vector position;
    matriz_3x3 viewMatrix;
    vector velocity;

public:
    FPSInputController(Camera &cameraReference, matriz &playerTransformReference) : InputController(cameraReference, playerTransformReference) {
        pitch = 0;
        yaw = 0;
        reset();
    }

    void setPitch(real pitch) {
        this->pitch = pitch;
        refreshViewMatrix();
    }

    real getPitch() const {
        return this->pitch;
    }

    void setYaw(real yaw) {
        this->yaw = yaw;
        refreshViewMatrix();
    }

    real getYaw() const {
        return this->yaw;
    }

    void setPosition(const vector &viewPosition) {
        this->position = viewPosition;
    }

    void setVelocity(const vector &viewVelocity) {
        this->velocity = viewVelocity;
    }

    virtual void reset() override {
        this->position = vector(0, 0, 0);
        this->velocity = vector(0, 0, 0);
        this->setPitch(0);
        this->setYaw(0);
    }

    virtual void update(real dt) override {
        position += viewMatrix.traspuesta() * velocity;

        this->constrainPosition();
        this->updatePlayerTransform();
        this->updateCamera();
    }

    void mouseMove(int x, int y, int dx, int dy) override {
        this->setPitch(this->getPitch() + dy);
        this->setYaw(this->getYaw() + dx);
    }

    void keyUp(unsigned int key, unsigned int keyModifier) override {
        switch (key) {
            case 'w':
            case 'W':
            case 's':
            case 'S':
                velocity.z = 0;;
                break;
            case 'a':
            case 'A':
            case 'd':
            case 'D':
                velocity.x = 0;
                break;
        }
    }

    void keyDown(unsigned int key, unsigned int keyModifier) override {
        switch (key) {
            case 'w':
            case 'W':
                velocity.z = -0.1;
                break;
            case 's':
            case 'S':
                velocity.z = 0.1;
                break;
            case 'a':
            case 'A':
                velocity.x = -0.1;
                break;
            case 'd':
            case 'D':
                velocity.x = 0.1;
                break;
        }
    }

protected:
    virtual void constrainPosition() {
    }

    virtual void updatePlayerTransform() {
        playerTransform = (matriz_4x4)viewMatrix * matriz_4x4::matrizTraslacion(-position - vector(0, 1, 0));

    }
    virtual void updateCamera() {
        camera.setViewMatrix(playerTransform);
    }

    virtual void refreshViewMatrix() {
        viewMatrix = matriz_3x3::matrizRotacion(radian(pitch), vector(1, 0, 0)) * matriz_3x3::matrizRotacion(radian(yaw), vector(0, 1, 0));
    }


};


#endif /* SRC_DEMO_INPUTCONTROLLER_FPSINPUTCONTROLLER_H_ */
