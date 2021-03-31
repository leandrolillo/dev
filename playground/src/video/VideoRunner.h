/*
 * VideoRunner.h
 *
 *  Created on: Mar 26, 2021
 *      Author: leandro
 */

#ifndef SRC_VIDEO_VIDEORUNNER_H_
#define SRC_VIDEO_VIDEORUNNER_H_

#include<Playground.h>
#include <adapters/PngResourceAdapter.h>
#include <adapters/JpegResourceAdapter.h>
#include <adapters/TgaResourceAdapter.h>
#include <adapters/GeometryResourceAdapter.h>
#include <adapters/TerrainResourceAdapter.h>

#include <resources/LightResource.h>
#include <resources/ShaderProgramResource.h>
#include <resources/VertexArrayResource.h>
#include <Math3d.h>


class VideoRunner : public PlaygroundRunner {
public:
	static const unsigned char ID = 0;
protected:
	bool fullScreen;
	unsigned short height;
	unsigned short width;

	matriz_4x4 projection;
	matriz_4x4 viewMatrix;
	matriz_4x4 projectionViewMatrix;
	matriz_4x4 modelMatrix;
	matriz_3x3 normalMatrix;


public:
	virtual unsigned char getId() {
		return ID;
	}

	unsigned short getScreenHeight() {
		return height;
	}

	unsigned short getScreenWidth() {
		return width;
	}

	bool getFullscreen() {
		return this->fullScreen;
	}

	virtual void resize(unsigned int height, unsigned int width) {
		this->height = height;
		this->width = width;
	}

	virtual bool move(unsigned short x, unsigned short y) {
		return true;
	}

	virtual bool setFullscreen(bool fullScreen) {
		this->fullScreen = fullScreen;

		return this->fullScreen;
	}

	virtual bool init() {
			this->getContainer()->getResourceManager()->addAdapter(new PngResourceAdapter());
			this->getContainer()->getResourceManager()->addAdapter(new JpegResourceAdapter());
			this->getContainer()->getResourceManager()->addAdapter(new TgaResourceAdapter());
			this->getContainer()->getResourceManager()->addAdapter(new GeometryResourceAdapter());
			this->getContainer()->getResourceManager()->addAdapter(new TerrainResourceAdapter());

			return true;
	}

	/**
	 * Mode, view and projection matrixes
	 */
	const matriz_4x4& getProjectionMatrix() const {
		return this->projection;
	}

	void setProjectionMatrix(const matriz_4x4 &projectionMatrix) {
			this->projection = projectionMatrix;
			this->projectionViewMatrix = this->projection * this->viewMatrix;
		}

	void setViewMatrix(const matriz_4x4 &viewMatrix) {
		this->viewMatrix = viewMatrix;
		this->projectionViewMatrix = this->projection * this->viewMatrix;
	}

	const matriz_4x4 &getViewMatrix() const {
		return this->viewMatrix;
	}

	void setModelMatrix(const matriz_4x4 &modelMatrix) {
		this->modelMatrix = modelMatrix;
		this->normalMatrix = ((matriz_3x3) modelMatrix).inversa().traspuesta();
	}

	const matriz_4x4 &getModelMatrix() const {
		return this->modelMatrix;
	}

	const matriz_4x4 &getProjectionViewMatrix() const {
		return this->projectionViewMatrix;
	}

	matriz_4x4 perspectiveProjection(double fovy, double aspect, double zNear, double zFar) {
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
				2.0 * zNear / (right - left), 	0.0, 							(right + left) / (right - left), 	0.0,
				0.0, 							2.0 * zNear / (top - bottom), 	(top + bottom) / (top - bottom), 	0.0,
				0.0, 							0.0, 							-(zFar + zNear) / (zFar - zNear), 	-(2.0 * zFar * zNear) / (zFar - zNear),
				0.0, 							0.0, 							-1.0, 								0.0);
	}

	/**
	 * Performance counters
	 */
	virtual unsigned long getPerformanceCounter() const = 0;
	virtual unsigned long getPerformanceFreq() const = 0;

	/**
	 * Shader methods - should this go to a shader class?
	 */

	virtual void useProgramResource(const ShaderProgramResource *program) = 0;
	virtual bool sendUnsignedInt(const String &name, unsigned int value) const = 0;
	virtual bool sendReal(const String &name, const real &value) const = 0;
	virtual bool sendVector(const String &name, const vector &value) const = 0;
	virtual bool sendMatrix(const String &name, const matriz_4x4 &value) const = 0;
	virtual bool sendMatrix(const String &name, const matriz_3x3 &value) const = 0;
	virtual bool sendMatrices() const = 0;
	virtual bool setMaterial(const MaterialResource &material) const = 0;
	virtual bool setLight(const LightResource &light) const = 0;

	/**
	 * Drawing methods
	 */
	virtual void setClearColor(real r, real g, real b, real a) const = 0;
	virtual void setAttribute(unsigned int attributeCode, unsigned int param) const = 0;

	virtual void setTexture(unsigned int location, const TextureResource *texture, unsigned int type = 0x0DE1) const = 0;
	virtual void setTexture(unsigned int location, const String &samplerName, const TextureResource *texture, unsigned int type = 0x0DE1) const = 0;
	virtual void drawVertexArray(const VertexArrayResource *vertexArrayResource) const = 0;
	virtual void drawSphere(real radius) const = 0;
	virtual void drawBox(real height, real width, real depth) const = 0;
	virtual void drawPlane(vector posicion, vector normal, vector origen, float nro_grids, float ancho) const = 0;
	virtual void drawAxis(real length = 1.0f) const = 0;

};



#endif /* SRC_VIDEO_VIDEORUNNER_H_ */
