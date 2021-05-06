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
    Logger *logger = Logger::getLogger("Camera");

    matriz_4x4 projectionMatrix;
    matriz_4x4 viewMatrix;
    matriz_4x4 projectionViewMatrix;

    real zNear = (real)0;
    real zFar = (real)0;

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

    const vector getViewPosition() const {
        return vector(viewMatrix (0, 3), viewMatrix(1, 3), viewMatrix(2, 3));
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

    matriz_4x4 perspectiveProjection(double fovy, double aspect, double zNear, double zFar) {
        double fW, fH;
        fH = tan(fovy / 360.0 * M_PI) * zNear;
        fW = fH * aspect;
        double left = -fW;
        double right = fW;
        double bottom = -fH;
        double top = fH;

        this->zNear = zNear;
        this->zFar = zFar;

        /**
         * from glFrustrum man page at https://www.lri.fr/~mbl/ENS/IG2/docs/opengl1.1/glFrustum.html
         */
        return matriz_4x4(
                2.0 * zNear / (right - left),   0.0,                            (right + left) / (right - left),    0.0,
                0.0,                            2.0 * zNear / (top - bottom),   (top + bottom) / (top - bottom),    0.0,
                0.0,                            0.0,                            -(zFar + zNear) / (zFar - zNear),   -(2.0 * zFar * zNear) / (zFar - zNear),
                0.0,                            0.0,                            -1.0,                               0.0);
    }

    vector4 getRayDirection(unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
        vector4 homogeneousClipCoordinates = vector4(
                1,
                (real)2 * (real)x / (real)width - (real)1,
                (real)1 - (real)2 * (real)y / (real)height,
                -1);

        vector4 cameraCoordinates = projectionMatrix.inversa() * homogeneousClipCoordinates;
        cameraCoordinates.z = (real)-1;
        cameraCoordinates.w = (real)1;

        vector3 worldCoordinates = ((vector3)(viewMatrix.inversa() * cameraCoordinates));
        worldCoordinates = worldCoordinates.normalizado();

        logger->info("projection\n%sprojection-1\n%s=\n%s",
                projectionMatrix.toString("%.3f").c_str(),
                projectionMatrix.inversa().toString("%.3f").c_str(),
                (projectionMatrix.inversa() * projectionMatrix).toString("%.3f").c_str());

        logger->info("view\n%sview-1\n%s=\n%s",
                        viewMatrix.toString("%.3f").c_str(),
                        viewMatrix.inversa().toString("%.3f").c_str(),
                        (viewMatrix.inversa() * viewMatrix).toString("%.3f").c_str());

        logger->info("click at <%u, %u> of <%u, %u>", x, y, width, height);
        logger->info("hcc = %s", homogeneousClipCoordinates.toString("%.2f").c_str());

        logger->info("camera coordinates = %s", cameraCoordinates.toString("%.2f").c_str());
        logger->info("world coordinates = %s", worldCoordinates.toString("%.2f").c_str());

        return worldCoordinates;
    }
};



#endif /* SRC_VIDEO_CAMERA_H_ */
