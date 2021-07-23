/*
 * ThirdPersonInputController.h
 *
 *  Created on: Jul 22, 2021
 *      Author: leandro
 */

#ifndef SRC_DEMO_INPUTCONTROLLER_THIRDPERSONINPUTCONTROLLER_H_
#define SRC_DEMO_INPUTCONTROLLER_THIRDPERSONINPUTCONTROLLER_H_

#include "FPSInputController.h"

class ThirdPersonInputController : public FPSInputController {
    real distance = (real)4;
public:
    ThirdPersonInputController(Camera &cameraReference, matriz &playerTransformReference) : FPSInputController(cameraReference, playerTransformReference) {
        //reset();
    }

    virtual void reset() override {
        FPSInputController::reset();
        this->distance = (real)4;
    }

    real getDistance() const {
        return distance;
    }

    void setDistance(real distance) {
        this->distance = distance;
        refreshViewMatrix();
    }

//    void update(real dt) override {
//        position += viewMatrix.traspuesta() * velocity;
//
//        if(terrain != null) {
//            position.y = terrain->getHeightMap()->heightAt(
//                position.x - floor(position.x / terrain->getHeightMap()->getWidth()) * terrain->getHeightMap()->getWidth(),
//                position.z - floor(position.z / terrain->getHeightMap()->getDepth()) * terrain->getHeightMap()->getDepth()
//                );
//        }
//
//        updatePlayerTransform();
//        updateCamera();
//    }

    void mouseWheel(int wheel) override {
        this->distance = this->distance + (real)wheel;
        this->distance = std::max((real)3, std::min((real)20, this->distance));
    }


protected:
    virtual void updatePlayerTransform() override {
        playerTransform = matriz_4x4::matrizTraslacion(position);
    }
    virtual void updateCamera() override {
        vector cameradelta = viewMatrix.traspuesta() * vector(0, 0, distance);
        camera.setViewMatrix((matriz_4x4)viewMatrix * matriz_4x4::matrizTraslacion(-(position + vector(0, 1, 0)) - cameradelta ));
    }
    void refreshViewMatrix() override {
        viewMatrix = matriz_3x3::matrizRotacion(radian(-getPitch()), vector(1, 0, 0)) * matriz_3x3::matrizRotacion(radian(-getYaw()), vector(0, 1, 0));
    }
};



#endif /* SRC_DEMO_INPUTCONTROLLER_THIRDPERSONINPUTCONTROLLER_H_ */
