#ifndef _PLAYGROUND_TEST_H_
#define _PLAYGROUND_TEST_H_

#include "win32/PlaygroundWin32.h"
#include "video/videoRunner.h"
#include "audio/audioRunner.h"
#include "test/TestRunner.h"
#include "math/Math3d.h"
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

class PlaygroundDemo : public PlaygroundRunner {
	private:
		VideoRunner *video;
		WglRunner *wgl;
		AudioRunner *audio;

		Logger *logger;

		//Graphical stuff
		vector viewPosition;
		vector lightPosition;
		vector *currentPosition;
		real rotation;
		TextureResource *textureResource;
		TextureResource *anotherTextureResource;
		VertexArrayResource *vertexArrayResource;
		ShaderProgramResource *shaderProgramResource;
		ShaderProgramResource *anotherShaderProgramResource;
		GeometryResource *geometryResource;
		VertexArrayResource *sphereArrayResource;
		GeometryResource *sphereGeometryResource;
	public:
		static const unsigned char ID = 101;

	public:
		PlaygroundDemo() /*: sphereGeometryResource(0) */{
			video = null;
			wgl = null;
			audio = null;

			logger = Logger::getLogger("PlaygroundTest.h");

			viewPosition = vector(0.0, 0.0f, -6.0);
			lightPosition = vector(0.0, 0.0f, 4.0);
			currentPosition = &viewPosition;
			rotation = 0;
			textureResource = null;
			anotherTextureResource = null;
			vertexArrayResource = null;
			shaderProgramResource = null;
			anotherShaderProgramResource = null;
			geometryResource = null;
			sphereArrayResource = null;
			sphereGeometryResource = null;
		}

		virtual unsigned char getInterests() {
			return KEY_DOWN | KEY_UP | MOUSE_MOVE;
		}

		virtual unsigned char getId() {
			return PlaygroundDemo::ID;
		}

		virtual boolean init() {
			video = (VideoRunner *) this->getContainer()->getRunner(1);
			wgl = (WglRunner *) this->getContainer()->getRunner(0);
			audio = (AudioRunner *)this->getContainer()->getRunner(3);


			// demo stuff
//			Source *backgroundSource = audio->createSource("background.ogg");
//			audio->playSource(backgroundSource);

			textureResource = (TextureResource *)this->getContainer()->getResourceManager()->load("images/ss/MB.PNG", "video/texture");
			anotherTextureResource = (TextureResource *)this->getContainer()->getResourceManager()->load("images/irs.JPG", "video/texture");
			geometryResource = (GeometryResource *)this->getContainer()->getResourceManager()->load("geometry/triangle.json", "video/geometry");
			sphereArrayResource = (VertexArrayResource *)this->getContainer()->getResourceManager()->load("geometry/sphere.json", "video/vertexArray");

			sphereGeometryResource = (GeometryResource *)this->getContainer()->getResourceManager()->load("geometry/sphere.json", "video/geometry");

			if(wgl->getMajorVersion() >= 3) {
				shaderProgramResource = (ShaderProgramResource *)this->getContainer()->getResourceManager()->load("shaders/lighting.140.program.json", "video/shaderProgram");
				anotherShaderProgramResource = (ShaderProgramResource *)this->getContainer()->getResourceManager()->load("shaders/toon.140.program.json", "video/shaderProgram");
			} else {
				shaderProgramResource = (ShaderProgramResource *)this->getContainer()->getResourceManager()->load("shaders/lighting.120.program.json", "video/shaderProgram");
				anotherShaderProgramResource = (ShaderProgramResource *)this->getContainer()->getResourceManager()->load("shaders/toon.120.program.json", "video/shaderProgram");

			}
			vertexArrayResource = (VertexArrayResource *)this->getContainer()->getResourceManager()->load("geometry/triangle.json", "video/vertexArray");

			glClearColor(0.0, 0.5, 0.0, 0.0);
			glShadeModel(GL_SMOOTH);
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);

//			glEnable(GL_BLEND);
//			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glEnable(GL_TEXTURE_2D);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

			glEnable(GL_LIGHTING);
			//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, (float *)vector4(0.2, 0.2, 0.2));

			glEnable(GL_LIGHT0);
			//glLightfv(GL_LIGHT0, GL_POSITION, (float *)vector4(0.0, 0.0, 0.0, 1.0));
			glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, (float *)vector3(1.0, 1.0, 1.0));
			glLightfv(GL_LIGHT0, GL_SPECULAR, (float *)vector3(0.0, 0.0, 0.0));


			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, (float *)vector3(1.0, 1.0, 1.0));
			glMaterialfv(GL_FRONT, GL_EMISSION, (float *)vector3(0.0, 0.0, 0.0));
			glMaterialfv(GL_FRONT, GL_SPECULAR, (float *)vector3(0.0, 0.0, 0.0));
			glMaterialf(GL_FRONT, GL_SHININESS, 128.0);

			logger->info("Demo Initialization finished");
			return true;
		}

		GeometryResource buildSphereGeometry()
		{
			GeometryResource resource(0);

			real dFi = radian(10);
			real dTita = radian(10);
			real radius = 1;

			real oneOverTwoPi = 1.0f / (2.0f * M_PI);
			real oneOverPi = 1.0f / M_PI;

			for(real fi = 0; fi < radian(360) - dFi; fi += dFi)
			{
				for(real tita = 0; tita < radian(180) - dTita; tita+= dTita)
				{
					vector2 texel1 = vector2(fi * oneOverTwoPi,   tita * oneOverPi);
					vector2 texel2 = vector2(fi * oneOverTwoPi,   (tita + dTita) * oneOverPi);
					vector2 texel3 = vector2((fi + dFi) * oneOverTwoPi,   tita * oneOverPi);
					vector2 texel4 = vector2((fi + dFi) * oneOverTwoPi,   (tita + dTita) * oneOverPi);

					vector3 vertex1 = radius * vector3(radius * sin(tita) * sin(fi), radius * cos(tita), radius * sin(tita) * cos(fi));
					vector3 vertex2 = radius * vector3(radius * sin(tita + dTita) * sin(fi), radius * cos(tita + dTita), radius * sin(tita + dTita) * cos(fi));
					vector3 vertex3 = radius * vector3(radius * sin(tita) * sin(fi + dFi), radius * cos(tita), radius * sin(tita) * cos(fi + dFi));
					vector3 vertex4 = radius * vector3(radius * sin(tita + dTita) * sin(fi + dFi), radius * cos(tita + dTita), radius * sin(tita + dTita) * cos(fi + dFi));

					resource.getVertices().push_back(vertex1);
					resource.getNormals().push_back(VectorUtilities::normalizar(vertex1));
					resource.getTextureCoordinates().push_back(texel1);
					resource.getIndices().push_back(resource.getIndices().size());
					resource.getColors().push_back(vector3(1.0, 1.0, 1.0));

					resource.getVertices().push_back(vertex2);
					resource.getNormals().push_back(VectorUtilities::normalizar(vertex2));
					resource.getTextureCoordinates().push_back(texel2);
					resource.getIndices().push_back(resource.getIndices().size());
					resource.getColors().push_back(vector3(1.0, 1.0, 1.0));

					resource.getVertices().push_back(vertex3);
					resource.getNormals().push_back(VectorUtilities::normalizar(vertex3));
					resource.getTextureCoordinates().push_back(texel3);
					resource.getIndices().push_back(resource.getIndices().size());
					resource.getColors().push_back(vector3(1.0, 1.0, 1.0));

					resource.getVertices().push_back(vertex3);
					resource.getNormals().push_back(VectorUtilities::normalizar(vertex3));
					resource.getTextureCoordinates().push_back(texel3);
					resource.getIndices().push_back(resource.getIndices().size());
					resource.getColors().push_back(vector3(1.0, 1.0, 1.0));


					resource.getVertices().push_back(vertex2);
					resource.getNormals().push_back(VectorUtilities::normalizar(vertex2));
					resource.getTextureCoordinates().push_back(texel2);
					resource.getIndices().push_back(resource.getIndices().size());
					resource.getColors().push_back(vector3(1.0, 1.0, 1.0));

					resource.getVertices().push_back(vertex4);
					resource.getNormals().push_back(VectorUtilities::normalizar(vertex4));
					resource.getTextureCoordinates().push_back(texel4);
					resource.getIndices().push_back(resource.getIndices().size());
					resource.getColors().push_back(vector3(1.0, 1.0, 1.0));
				}
			}

			return resource;
		}

		virtual LoopResult doLoop() {
			glUseProgram(0);

			glLoadIdentity();
			glTranslatef(viewPosition.x, viewPosition.y, viewPosition.z);

			video->glAxis();

			glBindTexture(GL_TEXTURE_2D, 0);

			glDisable(GL_LIGHTING);
			glColor3f(1.0, 1.0, 0.0);
			glPushMatrix();
			//light w = 1: spotlight; w = 0 directional light
			glLightfv(GL_LIGHT0, GL_POSITION, (float *)vector4(lightPosition.x, lightPosition.y, lightPosition.z, 0));
			glTranslatef(lightPosition.x, lightPosition.y, lightPosition.z);
			video->glSphere(.2);
			glPopMatrix();

			glEnable(GL_LIGHTING);

			if(anotherTextureResource != null)
				glBindTexture(GL_TEXTURE_2D, anotherTextureResource->getId());

			glPushMatrix();
			glTranslatef(-1.5, 2.0, 0.0);
			glRotatef(rotation, 0.0, 1.0, 0.0);
			video->glDrawGeometry(geometryResource);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(1.5, 2.0, 0.0);
			glRotatef(rotation, 0.0, 1.0, 0.0);
			video->glDrawGeometry(sphereGeometryResource);
			glPopMatrix();

			if(anotherShaderProgramResource != null)
				glUseProgram(anotherShaderProgramResource->getId());

			glPushMatrix();
			glTranslatef(-1.5, 0.0, 0.0);
			glRotatef(rotation, 0.0, 1.0, 0.0);
			video->glDrawGeometry(geometryResource);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(1.5, 0.0, 0.0);
			glRotatef(rotation, 0.0, 1.0, 0.0);
			video->glDrawGeometry(sphereGeometryResource);
			glPopMatrix();


			if(shaderProgramResource != null)
				glUseProgram(shaderProgramResource->getId());

			glPushMatrix();
			glTranslatef(-1.5, -2.0, 0.0);
			glRotatef(rotation, 0.0, 1.0, 0.0);
			video->glDrawVertexArray(vertexArrayResource);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(1.5, -2.0, 0.0);
			glRotatef(rotation, 0.0, 1.0, 0.0);
			video->glDrawVertexArray(sphereArrayResource);
			glPopMatrix();

			rotation+=0.1;

			return CONTINUE;
		}
		virtual void mouseMove(int dx, int dy) {
			*currentPosition += vector(0.1f * dx, 0.1f * dy, 0);
		}
		virtual void keyUp(unsigned int key) {
			switch (key) {
				case VK_CONTROL:
					currentPosition = &viewPosition;
					break;
			}
		}
		virtual void keyDown(unsigned int key) {
			switch (key) {
				case VK_CONTROL:
					currentPosition = &lightPosition;
					break;
				case VK_SPACE:
					viewPosition = vector(0.0, 0.0f, -6.0);
					lightPosition = vector(0.0, 0.0, 4.0);
					break;
				case VK_ESCAPE:
					this->getContainer()->stop();
					break;
				case 'f':
				case 'F':
					this->wgl->setFullscreen(!this->wgl->getFullscreen());
				break;
			}
		}

};

//#define SKIP_DEMO
//#define SKIP_TESTS

class PlaygroundTest: public PlaygroundWin32{
	public:
		void init() {
			PlaygroundWin32::init();

#ifdef SKIP_DEMO
			this->getWinHandler()->setEnabled(false);
			this->getVideoRunner()->setEnabled(false);
			this->getAudioRunner()->setEnabled(false);
#else
			this->addRunner(new PlaygroundDemo());
#endif

#ifndef SKIP_TESTS
			this->addRunner(new PlaygroundTests());
#endif
		}
};
#endif
