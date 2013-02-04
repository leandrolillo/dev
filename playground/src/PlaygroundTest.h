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
				logger->error("Load WAV test failed: %s", e.toString().c_str());
				testsInError++;
			}
			try {
				testLoadBuffer();
			} catch (Exception e) {
				logger->error("Load WAV test failed: %s", e.toString().c_str());
				testsInError++;
			}
			try {
				testCreateSource();
			} catch (Exception &e) {
				logger->error("Load WAV test failed: %s", e.toString().c_str());
				testsInError++;
			}
			try {
				testLoadPng();
			} catch (Exception &e) {
				logger->error("Load WAV test failed: %s", e.toString().c_str());
				testsInError++;
			}
			try {
				testLoadJpeg();
			} catch (Exception &e) {
				logger->error("Load WAV test failed: %s", e.toString().c_str());
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
			assertTrue("Load WAV test failed", resource != null && resource->getData() != null);

		}
		void testLoadOgg() {
			OggResource *resource = (OggResource *)this->getContainer()->getResourceManager()->load("tests/audio.ogg");
			assertTrue("Load OGG test failed", resource != null && resource->getData() != null);
		}

		void testLoadBuffer() {
			BufferResource *resource = (BufferResource *)this->getContainer()->getResourceManager()->load("tests/audio.ogg", "audio/buffer");
			assertTrue("Load BUFFER test failed", resource != null && resource->getId() != null);
		}

		void testCreateSource()		{
			Source *resource = (Source *)this->getContainer()->getResourceManager()->load("tests/audio.ogg", "audio/source");
			assertTrue("Load SOURCE test failed", resource != null && resource->getId() != null);
		}

		void testLoadPng() 		{
			PngResource *resource = (PngResource *)this->getContainer()->getResourceManager()->load("tests/image.png");
			assertTrue("Load PNG test failed", resource != null && resource->getData() != null);
		}

		void testLoadJpeg()		{
			JpegResource *resource = (JpegResource *)this->getContainer()->getResourceManager()->load("tests/image.jpg");

			assertTrue("Load JPEG test failed", resource != null && resource->getData() != null);
		}

		void testLoadTexture()		{
			TextureResource *resource = (TextureResource *)this->getContainer()->getResourceManager()->load("tests/image.png", "video/texture");
			assertTrue("Load TEXTURE test failed", resource != null && resource->getId() != null);
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
