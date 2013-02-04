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

			posicion = vector(0.0, -1.0f, -5.0);
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

			Source *oggSource = audio->createSource("background.ogg");

			Source *backgroundSource = audio->createSource("background.wav");
			if(backgroundSource != null)
				audio->playSource(backgroundSource);

			JpegResource *jpegResource = (JpegResource *)this->getContainer()->getResourceManager()->load("imagenes/ss/REBECCA_ROMIJN_STAMOS0118.JPG");
			PngResource *pngResource = (PngResource *)this->getContainer()->getResourceManager()->load("imagenes/ss/MB.PNG");

			textureResource = (TextureResource *)this->getContainer()->getResourceManager()->load("imagenes/ss/MB.PNG", "video/texture");
			anotherTextureResource = (TextureResource *)this->getContainer()->getResourceManager()->load("imagenes/ss/REBECCA_ROMIJN_STAMOS0118.JPG", "video/texture");

			return true;
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
