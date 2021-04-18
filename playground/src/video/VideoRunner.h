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
#include <adapters/ObjResourceAdapter.h>

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
			this->getContainer()->getResourceManager()->addAdapter(new ObjResourceAdapter());

			return true;
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

	/**
	 * Drawing methods
	 */
	virtual void setClearColor(real r, real g, real b, real a) const = 0;
	virtual void setAttribute(unsigned int attributeCode, unsigned int param) const = 0;

	virtual void setTexture(unsigned int location, const TextureResource *texture, unsigned int type = 0x0DE1) const = 0;
	virtual void setTexture(unsigned int location, const String &samplerName, const TextureResource *texture, unsigned int type = 0x0DE1) const = 0;
	virtual void drawVertexArray(const VertexArrayResource *vertexArrayResource) const = 0;

	//TODO: get rid of this method
	virtual TextureResource *getDefaultTexture() const = 0;
};



#endif /* SRC_VIDEO_VIDEORUNNER_H_ */
