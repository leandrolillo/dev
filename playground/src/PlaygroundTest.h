#ifndef _PLAYGROUND_TEST_H_
#define _PLAYGROUND_TEST_H_

#include "win32/PlaygroundWin32.h"
#include "video/videoRunner.h"
#include "audio/audioRunner.h"
#include "test/TestRunner.h"
#include <stdio.h>

#include"math/Math3d.h"

class PlaygroundTests: public TestRunner {
		Logger *logger;
	public:
		static const unsigned char ID = 100;

	public:
		PlaygroundTests() {
			logger = Logger::getLogger("PlaygroundTest.h");
		}

		virtual unsigned char getId() {

			return PlaygroundTests::ID;
		}

		virtual boolean init() {
			this->addTest("testLoadWav", static_cast<void (TestRunner::*)()>(&PlaygroundTests::testLoadWav));
			this->addTest("testLoadOgg", static_cast<void (TestRunner::*)()>(&PlaygroundTests::testLoadOgg));
			this->addTest("testLoadBuffer", static_cast<void (TestRunner::*)()>(&PlaygroundTests::testLoadBuffer));
			this->addTest("testCreateSource", static_cast<void (TestRunner::*)()>(&PlaygroundTests::testCreateSource));
			this->addTest("testLoadPng", static_cast<void (TestRunner::*)()>(&PlaygroundTests::testLoadPng));
			this->addTest("testLoadJpeg", static_cast<void (TestRunner::*)()>(&PlaygroundTests::testLoadJpeg));
			this->addTest("testLoadTexture", static_cast<void (TestRunner::*)()>(&PlaygroundTests::testLoadTexture));
			this->addTest("testLoadInvalidResource", static_cast<void (TestRunner::*)()>(&PlaygroundTests::testInvalidResource));
			this->addTest("testFileParser", static_cast<void (TestRunner::*)()>(&PlaygroundTests::testFileParser));
			this->addTest("testLoadGeometry", static_cast<void (TestRunner::*)()>(&PlaygroundTests::testLoadGeometry));
			this->addTest("testLoadVertexBuffer", static_cast<void (TestRunner::*)()>(&PlaygroundTests::testLoadVertexBuffer));

			return true;
		}

		void testInvalidResource()
		{
				Resource *resource = this->getContainer()->getResourceManager()->load("tests/fake.wav");
				assertEquals("Null resource expected", null, resource);

				resource = this->getContainer()->getResourceManager()->load("tests/fake.nomimetype");
				assertEquals("Null resource expected", null, resource);

		}
		void testFileParser()
		{
			String token;

			FileParser commentFileParser(this->getContainer()->getResourceManager()->normalize("tests/commentFileToParse.txt"));
			assertEquals("Unexpected token", eof, commentFileParser.peekToken());
			assertEquals("Unexpected token", eof, commentFileParser.takeToken());
			commentFileParser.close();

			FileParser emptyFileParser(this->getContainer()->getResourceManager()->normalize("tests/emptyFileToParse.txt"));
			assertEquals("Unexpected token", eof, emptyFileParser.peekToken());
			assertEquals("Unexpected token", eof, emptyFileParser.takeToken());
			commentFileParser.close();

			FileParser fileParser(this->getContainer()->getResourceManager()->normalize("tests/fileToParse.txt"));
			assertEquals("Unexpected token", "{", fileParser.peekToken());
			assertEquals("Unexpected token", "{", fileParser.takeToken());
			assertEquals("Unexpected token", "\"", fileParser.takeToken());
			assertEquals("Unexpected token", "property", fileParser.takeToken());
			assertEquals("Unexpected token", "\"", fileParser.takeToken());
			assertEquals("Unexpected token", ":", fileParser.takeToken());
			assertEquals("Unexpected token", "\"", fileParser.takeToken());
			assertEquals("Unexpected token", "value", fileParser.takeToken());
			assertEquals("Unexpected token", "\"", fileParser.takeToken());
			assertEquals("Unexpected token", "}", fileParser.takeToken());
			assertEquals("Unexpected token", eof, fileParser.peekToken());
			assertEquals("Unexpected token", eof, fileParser.takeToken());
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

		void testLoadGeometry()	{
			GeometryResource *resource = (GeometryResource *)this->getContainer()->getResourceManager()->load("tests/geometry.json", "video/geometry");

			assertTrue("GEOMETRY resource not loaded", resource != null);
			assertEquals("GEOMETRY mimetype invalid", "video/geometry", resource->getMimeType());
		}

		void testLoadVertexBuffer()	{
			VertexArrayResource *resource = (VertexArrayResource *)this->getContainer()->getResourceManager()->load("tests/geometry.json", "video/vertexArray");

			assertTrue("VertexBuffer resource not loaded", resource != null);
			assertEquals("VertexBuffer mimetype invalid", "video/vertexArray", resource->getMimeType());
		}

};

class PlaygroundDemo : public PlaygroundRunner {
	private:
		VideoRunner *video;
		WglRunner *wgl;
		AudioRunner *audio;

		Logger *logger;

		//Graphical stuff
		vector posicion;
		TextureResource *textureResource;
		TextureResource *anotherTextureResource;
		VertexArrayResource *vertexArrayResource;
	public:
		static const unsigned char ID = 101;

	public:
		PlaygroundDemo() {
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

			return PlaygroundDemo::ID;
		}

		virtual boolean init() {
			video = (VideoRunner *) this->getContainer()->getRunner(1);
			wgl = (WglRunner *) this->getContainer()->getRunner(0);
			audio = (AudioRunner *)this->getContainer()->getRunner(3);


			// demo stuff
			Source *backgroundSource = audio->createSource("background.ogg");
			if(backgroundSource != null)
				audio->playSource(backgroundSource);

			textureResource = (TextureResource *)this->getContainer()->getResourceManager()->load("images/ss/MB.PNG", "video/texture");
			anotherTextureResource = (TextureResource *)this->getContainer()->getResourceManager()->load("images/ss/REBECCA_ROMIJN_STAMOS0118.JPG", "video/texture");

			vertexArrayResource = (VertexArrayResource *)this->getContainer()->getResourceManager()->load("geometry/triangle.json", "video/vertexArray");
			return true;
		}

		virtual LoopResult doLoop() {
			glLoadIdentity();

			glTranslatef(posicion.x, posicion.y, posicion.z);

			if(textureResource != null)
				glBindTexture(GL_TEXTURE_2D, textureResource->getId());

			video->glPlane(posicion, vector(0, 1, 0), vector(0.0, 0.0f, 0.0f), 3, 3);

			video->glAxis();

			if(anotherTextureResource != null)
				glBindTexture(GL_TEXTURE_2D, anotherTextureResource->getId());

			if(vertexArrayResource != null) {
				glBindVertexArray(vertexArrayResource->getId());
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 1);
			}

//			glBegin(GL_TRIANGLE_FAN);
//				glTexCoord2f(0.0f, 0.0f);
//				glVertex3f(0.0f, 0.0f, 1.0f);
//
//				glTexCoord2f(0.0f, 1.0f);
//				glVertex3f(0.0f, 1.0f, 1.0f);
//
//				glTexCoord2f(1.0f, 1.0f);
//				glVertex3f(1.0f, 1.0f, 1.0f);
//
//				glTexCoord2f(1.0f, 0.0f);
//				glVertex3f(1.0f, 0.0f, 1.0f);
//			glEnd();


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

			this->addRunner(new PlaygroundDemo());
			this->addRunner(new PlaygroundTests());
		}
};
#endif
