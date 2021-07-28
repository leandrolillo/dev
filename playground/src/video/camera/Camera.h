/*
 * Camera.h
 *
 *  Created on: Apr 1, 2021
 *      Author: leandro
 */

#ifndef SRC_VIDEO_CAMERA_H_
#define SRC_VIDEO_CAMERA_H_

#include<Math3d.h>

class Camera {
protected:
    Logger *logger = LoggerFactory::getLogger("Camera");

    matriz_4x4 projectionMatrix;
    matriz_4x4 viewMatrix;
    matriz_4x4 projectionViewMatrix;


    //TODO: Review how to get these values from projection (if it applies)
    real zNear = (real)0.1;
    real zFar = (real)300;
public:
    static matriz_4x4 perspectiveProjection(double fovy, double aspect, double zNear, double zFar) {
        double fW, fH;
        fH = tan(fovy / 360.0 * M_PI) * zNear;
        fW = fH * aspect;
        double left = -fW;
        double right = fW;
        double bottom = -fH;
        double top = fH;

        /**
         * from glFrustrum man page at https://www.lri.fr/~mbl/ENS/IG2/docs/opengl1.1/glFrustum.html
         */
        return matriz_4x4(
                2.0 * zNear / (right - left),   0.0,                            (right + left) / (right - left),    0.0,
                0.0,                            2.0 * zNear / (top - bottom),   (top + bottom) / (top - bottom),    0.0,
                0.0,                            0.0,                            -(zFar + zNear) / (zFar - zNear),   -(2.0 * zFar * zNear) / (zFar - zNear),
                0.0,                            0.0,                            -1.0,                               0.0);
    }

public:
    /**
     * Mode, view and projection matrixes
     */
    const matriz_4x4& getProjectionMatrix() const {
        return this->projectionMatrix;
    }

    void setProjectionMatrix(const matriz_4x4 &projectionMatrix) {
            this->projectionMatrix = projectionMatrix;
            this->projectionViewMatrix = this->projectionMatrix * this->viewMatrix;
        }

    void setViewMatrix(const matriz_4x4 &viewMatrix) {
        this->viewMatrix = viewMatrix;
        this->projectionViewMatrix = this->projectionMatrix * this->viewMatrix;
    }

    const matriz_4x4 &getViewMatrix() const {
        return this->viewMatrix;
    }


    void setViewPosition(const vector &position) {
        viewMatrix(0, 3) = -position.x;
        viewMatrix(1, 3) = -position.y;
        viewMatrix(2, 3) = -position.z;

        this->projectionViewMatrix = this->projectionMatrix * this->viewMatrix;
    }

    const vector getViewPosition() const {
        return vector(-viewMatrix (0, 3), -viewMatrix(1, 3), -viewMatrix(2, 3));
    }

    const matriz_4x4 &getProjectionViewMatrix() const {
        return this->projectionViewMatrix;
    }

    real getZNear() const {
        return this->zNear;
    }

    real getZFar() const {
        return this->zFar;
    }

    vector4 getRayDirection(unsigned int x, unsigned int y, unsigned int width, unsigned int height) const {
        vector4 homogeneousClipCoordinates = vector4(
                (real)2 * (real)x / (real)width - (real)1,
                (real)1 - (real)2 * (real)y / (real)height,
                -1,
                0);

        vector4 cameraCoordinates = projectionMatrix.inversa() * homogeneousClipCoordinates;
        cameraCoordinates.z = (real)-1;
        cameraCoordinates.w = (real)0;

        vector3 worldCoordinates = ((vector3)(viewMatrix.inversa() * cameraCoordinates)).normalizado();

        return worldCoordinates;
    }
};



#endif /* SRC_VIDEO_CAMERA_H_ */
