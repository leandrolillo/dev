#ifndef _PLAYGROUND_TEST_H_
#define _PLAYGROUND_TEST_H_

#include "win32/PlaygroundWin32.h"
#include "video/videoRunner.h"
#include "audio/audioRunner.h"
#include <stdio.h>

#include"math/Math3d.h"

class TestLogic: public PlaygroundRunner {
	private:
		VideoRunner *video;
		WglRunner *wgl;
		AudioRunner *audio;

		Logger *logger;

		//Graphical stuff
		vector posicion;
		TextureResource *textureResource;
		TextureResource *anotherTextureResource;
	public:
		static const unsigned char ID = 100;

	public:
		TestLogic() {
			video = null;
			wgl = null;
			audio = null;

			logger = Logger::getLogger("PlaygroundTest.h");

			posicion = vector(0.0, -1.0f, -5.0);
			textureResource = null;
			anotherTextureResource = null;
		}

		virtual unsigned char getInterests() {
			return KEY_DOWN | MOUSE_MOVE;
		}

		virtual unsigned char getId() {
			return TestLogic::ID;
		}

		virtual boolean init() {
			video = (VideoRunner *) this->getContainer()->getRunner(1);
			wgl = (WglRunner *) this->getContainer()->getRunner(0);
			audio = (AudioRunner *)this->getContainer()->getRunner(3);


			//tests
			doTests();

			// demo stuff
			Source *backgroundSource = audio->createSource("background.wav");
			if(backgroundSource != null)
				audio->playSource(backgroundSource);

			textureResource = (TextureResource *)this->getContainer()->getResourceManager()->load("imagenes/ss/MB.PNG", "video/texture");
			anotherTextureResource = (TextureResource *)this->getContainer()->getResourceManager()->load("imagenes/ss/REBECCA_ROMIJN_STAMOS0118.JPG", "video/texture");

			return true;
		}

		void doTests() {
			unsigned int testsInError = 0;
			logger->debug("\n\nRunning Test...\n\n");
			try {
				testLoadWav();
			} catch (Exception &e) {
				logger->error("Load WAV test failed: %s", e.toString().c_str());
				testsInError++;
			}

			try {
				testLoadOgg();
			} catch (Exception &e) {
				logger->error("Load OGG test failed: %s", e.toString().c_str());
				testsInError++;
			}
			try {
				testLoadBuffer();
			} catch (Exception e) {
				logger->error("Load BUFFER test failed: %s", e.toString().c_str());
				testsInError++;
			}
			try {
				testCreateSource();
			} catch (Exception &e) {
				logger->error("Load SOURCE test failed: %s", e.toString().c_str());
				testsInError++;
			}
			try {
				testLoadPng();
			} catch (Exception &e) {
				logger->error("Load PNG test failed: %s", e.toString().c_str());
				testsInError++;
			}
			try {
				testLoadJpeg();
			} catch (Exception &e) {
				logger->error("Load JPEG test failed: %s", e.toString().c_str());
				testsInError++;
			}
			try {
				testLoadTexture();
			} catch (Exception &e) {
				logger->error("Load WAV test failed: %s", e.toString().c_str());
				testsInError++;
			}

			logger->debug("\n\nTESTS: %d tests in error\n\n", testsInError);
		}

		void assertTrue(boolean condition)
		{
			assertTrue("", condition);
		}
		void assertTrue(String message, boolean condition)
		{
			if(!condition)
			assertFail(message);
		}

		void assertEquals(String message, const String &expected, const String &actual)
		{
			assertTrue(message + ". Expected: [" + expected + "]. Actual: [" + actual + "]", expected == actual);
		}

		void assertEquals(String message, unsigned int expected, unsigned int actual)
		{
			char buffer[256];
			sprintf(buffer, "%s. Expected: [%d]. Actual: [%d]", message.c_str(), expected, actual);
			assertTrue(buffer, expected == actual);
		}

		void assertFalse(String message, boolean condition)
		{
			if(condition)
			assertFail(message);
		}
		void assertFalse(boolean condition)
		{
			assertFalse("", condition);
		}

		void assertFail(String message) {
			throw Exception(message.c_str());
		}

		void testLoadWav()
		{
			AudioResource *resource = (AudioResource *)this->getContainer()->getResourceManager()->load("tests/audio.wav");
			assertTrue("WAV resource not loaded", resource != null);
			assertTrue("WAV resource data not loaded", resource->getData() != null);
			assertEquals("WAV mimetype invalid", "audio/wav", resource->getMimeType());

		}
		void testLoadOgg() {
			OggResource *resource = (OggResource *)this->getContainer()->getResourceManager()->load("tests/audio.ogg");
			assertTrue("OGG resource not loaded", resource != null);
			assertTrue("OGG resource data not loaded", resource->getData() != null);
			assertEquals("OGG mimetype invalid", "audio/ogg", resource->getMimeType());
		}

		void testLoadBuffer() {
			BufferResource *resource = (BufferResource *)this->getContainer()->getResourceManager()->load("tests/audio.ogg", "audio/buffer");
			assertTrue("BUFFER resource not loaded", resource != null);
			assertTrue("BUFFER resource id not set correctly", resource->getId() != 0);
			assertEquals("Buffer mimetype invalid", "audio/buffer", resource->getMimeType());
		}

		void testCreateSource()		{
			Source *resource = (Source *)this->getContainer()->getResourceManager()->load("tests/audio.ogg", "audio/source");
			assertTrue("Source resource not loaded", resource != null);
			assertTrue("Source resource id not set correctly", resource->getId() != 0);
			assertEquals("Source mimetype invalid", "audio/source", resource->getMimeType());

		}

		void testLoadPng() 		{
			PngResource *resource = (PngResource *)this->getContainer()->getResourceManager()->load("tests/image.png");
			assertTrue("PNG resource not loaded", resource != null);
			assertTrue("PNG data not set properly", resource->getData() != null);
			assertEquals("PNG Invalid width", 512, resource->getAncho());
			assertEquals("PNG Invalid height", 512, resource->getAlto());
			assertEquals("PNG mimetype invalid", "image/png", resource->getMimeType());
		}

		void testLoadJpeg()		{
			JpegResource *resource = (JpegResource *)this->getContainer()->getResourceManager()->load("tests/image.jpg");

			assertTrue("JPEG resource not loaded", resource != null);
			assertTrue("JPEG data not set properly", resource->getData() != null);
			assertEquals("JPEG Invalid width", 756, resource->getAncho());
			assertEquals("JPEG Invalid height", 512, resource->getAlto());
			assertEquals("JPEG mimetype invalid", "image/jpeg", resource->getMimeType());
		}

		void testLoadTexture()		{
			TextureResource *resource = (TextureResource *)this->getContainer()->getResourceManager()->load("tests/image.png", "video/texture");

			assertTrue("TEXTURE resource not loaded", resource != null);
			assertTrue("TEXTURE id not set properly", resource->getId() != 0);
			assertEquals("TEXTURE mimetype invalid", "video/texture", resource->getMimeType());
		}

		virtual LoopResult doLoop() {
			glLoadIdentity();

			glTranslatef(posicion.x, posicion.y, posicion.z);
			glBindTexture(GL_TEXTURE_2D, textureResource->getId());
			video->glPlane(posicion, vector(0, 1, 0), vector(0.0, 0.0f, 0.0f), 3, 3);

			video->glAxis();
			glBindTexture(GL_TEXTURE_2D, anotherTextureResource->getId());
			glBegin(GL_TRIANGLE_FAN);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(0.0f, 0.0f, 1.0f);

				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(0.0f, 1.0f, 1.0f);

				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(1.0f, 1.0f, 1.0f);

				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(1.0f, 0.0f, 1.0f);
			glEnd();


			return CONTINUE;
		}
		virtual void mouseMove(int dx, int dy) {
			posicion += vector(0.1f * dx, 0.1f * dy, 0);
		}
		virtual void keyDown(unsigned int key) {
			switch (key) {
				case VK_SPACE:
					posicion = vector(0.0, -1.0f, -5.0);
					break;
				case VK_ESCAPE:
					this->getContainer()->stop();
					break;
				case VK_RETURN:
					this->wgl->setFullscreen(!this->wgl->getFullscreen());
					break;
			}
		}

};
class PlaygroundTest: public PlaygroundWin32 {
	public:
		void init() {
			PlaygroundWin32::init();
			this->addRunner(new TestLogic());

		}
};
#endif
