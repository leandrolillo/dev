#ifndef _PLAYGROUND_TEST_H_
#define _PLAYGROUND_TEST_H_

//#include "win32/PlaygroundWin32.h"
//#include "video/videoRunner.h"
//#include "audio/audioRunner.h"

#include "TestRunner.h"
#include "../math/Math3d.h"
#include <stdio.h>


//#define SKIP_DEMO
#define SKIP_TESTS


#ifndef SKIP_TESTS
class PlaygroundTestsRunner: public TestRunner {
		Logger *logger;
	public:
		static const unsigned char ID = 100;

	public:
		PlaygroundTestsRunner() {
			logger = Logger::getLogger("PlaygroundTest.h");
		}

		virtual unsigned char getId() {

			return PlaygroundTests::ID;
		}

		virtual bool init() {
			this->addTest("testLoadWav", static_cast<void (TestRunner::*)()>(&PlaygroundTests::testLoadWav));
			this->addTest("testLoadOgg", static_cast<void (TestRunner::*)()>(&PlaygroundTests::testLoadOgg));
			this->addTest("testLoadBuffer", static_cast<void (TestRunner::*)()>(&PlaygroundTests::testLoadBuffer));
			this->addTest("testCreateSource", static_cast<void (TestRunner::*)()>(&PlaygroundTests::testCreateSource));
			this->addTest("testLoadPng", static_cast<void (TestRunner::*)()>(&PlaygroundTests::testLoadPng));
			this->addTest("testLoadJpeg", static_cast<void (TestRunner::*)()>(&PlaygroundTests::testLoadJpeg));
			this->addTest("testLoadTga", static_cast<void (TestRunner::*)()>(&PlaygroundTests::testLoadTga));
			this->addTest("testLoadTexture", static_cast<void (TestRunner::*)()>(&PlaygroundTests::testLoadTexture));
			this->addTest("testLoadInvalidResource", static_cast<void (TestRunner::*)()>(&PlaygroundTests::testInvalidResource));
			this->addTest("testFileParser", static_cast<void (TestRunner::*)()>(&PlaygroundTests::testFileParser));
			this->addTest("testLoadGeometry", static_cast<void (TestRunner::*)()>(&PlaygroundTests::testLoadGeometry));
			this->addTest("testLoadVertexArray", static_cast<void (TestRunner::*)()>(&PlaygroundTests::testLoadVertexBuffer));
			this->addTest("testLoadShader", static_cast<void (TestRunner::*)()>(&PlaygroundTests::testLoadShaders));
			this->addTest("testLoadShaderProgram", static_cast<void (TestRunner::*)()>(&PlaygroundTests::testLoadShaderProgram));
			this->addTest("testLoadShaderProgramByVersion", static_cast<void (TestRunner::*)()>(&PlaygroundTests::testLoadShaderProgramByVersion));
			this->addTest("testMath", static_cast<void (TestRunner::*)()>(&PlaygroundTests::testMath));

			return true;
		}

		void testMath()
		{
			//TODO: Test vector and matrix math operations, factories and utilities. Test functions.
			assertFail("Implement the tests you lazy ass!");
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

		void testLoadTga()		{
			TgaResource *resource = (TgaResource *)this->getContainer()->getResourceManager()->load("tests/image.tga");

			assertTrue("TGA resource not loaded", resource != null);
			assertTrue("TGA data not set properly", resource->getData() != null);
			assertEquals("TGA Invalid width", 400, resource->getAncho());
			assertEquals("TGA Invalid height", 300, resource->getAlto());
			assertEquals("TGA mimetype invalid", "image/tga", resource->getMimeType());
		}

		void testLoadTexture()		{
			TextureResource *resource = (TextureResource *)this->getContainer()->getResourceManager()->load("tests/image.png", "video/texture");

			assertTrue("TEXTURE resource not loaded", resource != null);
			assertTrue("TEXTURE id not set properly", resource->getId() != 0);
			assertEquals("TEXTURE mimetype invalid", "video/texture", resource->getMimeType());

			resource = (TextureResource *)this->getContainer()->getResourceManager()->load("tests/image.jpg", "video/texture");

			assertTrue("TEXTURE resource not loaded", resource != null);
			assertTrue("TEXTURE id not set properly", resource->getId() != 0);
			assertEquals("TEXTURE mimetype invalid", "video/texture", resource->getMimeType());

			resource = (TextureResource *)this->getContainer()->getResourceManager()->load("tests/image.tga", "video/texture");

			assertTrue("TEXTURE resource not loaded", resource != null);
			assertTrue("TEXTURE id not set properly", resource->getId() != 0);
			assertEquals("TEXTURE mimetype invalid", "video/texture", resource->getMimeType());


		}

		void testLoadGeometry()	{
			GeometryResource *resource = (GeometryResource *)this->getContainer()->getResourceManager()->load("tests/geometry.json", "video/geometry");

			assertTrue("GEOMETRY resource not loaded", resource != null);
			assertEquals("GEOMETRY mimetype invalid", "video/geometry", resource->getMimeType());
			assertEquals("Incorrect number of vertices", 10, resource->getVertices().size());
			assertEquals("Incorrect number of colors", 3, resource->getColors().size());
			assertEquals("Incorrect number of texture coordinates", 3, resource->getTextureCoordinates().size());
			assertEquals("Incorrect number of normals", 3, resource->getNormals().size());


		}

		void testLoadVertexBuffer()	{
			VertexArrayResource *resource = (VertexArrayResource *)this->getContainer()->getResourceManager()->load("tests/geometry.json", "video/vertexArray");

			assertTrue("VertexArray resource not loaded", resource != null);
			assertEquals("VertexArray mimetype invalid", "video/vertexArray", resource->getMimeType());
		}

		void testLoadShaders()	{
			ShaderResource *resource = (ShaderResource *)this->getContainer()->getResourceManager()->load("tests/vertexShader.glsl", "video/vertexShader");

			assertTrue("Shader resource not loaded", resource != null);
			assertEquals("Shader mimetype invalid", "video/vertexShader", resource->getMimeType());
		}

		void testLoadShaderProgram()	{
			ShaderProgramResource *resource = (ShaderProgramResource *)this->getContainer()->getResourceManager()->load("tests/shaderProgram.json", "video/shaderProgram");

			assertTrue("Shader Program resource not loaded", resource != null);
			assertEquals("Shader Program mimetype invalid", "video/shaderProgram", resource->getMimeType());
		}

		void testLoadShaderProgramByVersion()
		{
			WglRunner *wgl = (WglRunner *) this->getContainer()->getRunner(0);

			ShaderProgramResource *shaderProgramResource = null;

			if(wgl->getMajorVersion() >= 3) {
				shaderProgramResource = (ShaderProgramResource *)this->getContainer()->getResourceManager()->load("shaders/lighting.140.program.json", "video/shaderProgram");
			} else {
				shaderProgramResource = (ShaderProgramResource *)this->getContainer()->getResourceManager()->load("shaders/lighting.120.program.json", "video/shaderProgram");
			}
			assertTrue("Shader Program resource not loaded", shaderProgramResource != null);
			assertEquals("Shader Program mimetype invalid", "video/shaderProgram", shaderProgramResource->getMimeType());

			if(wgl->getMajorVersion() >= 3) {
				shaderProgramResource = (ShaderProgramResource *)this->getContainer()->getResourceManager()->load("shaders/toon.140.program.json", "video/shaderProgram");
			} else {
				shaderProgramResource = (ShaderProgramResource *)this->getContainer()->getResourceManager()->load("shaders/toon.120.program.json", "video/shaderProgram");
			}
			assertTrue("Shader Program resource not loaded", shaderProgramResource != null);
			assertEquals("Shader Program mimetype invalid", "video/shaderProgram", shaderProgramResource->getMimeType());

		}

};

#endif
#endif
