/*
 * PlaygroundWin32.h
 *
 *  Created on: 20/11/2012
 *      Author: Lean
 */

#ifndef PLAYGROUNDWIN32_H_
#define PLAYGROUNDWIN32_H_

#include<stdio.h>

#include "core/playground.h"
#include "wglRunner.h"
#include "video/videoRunner.h"
#include "audio/audioRunner.h"

class PlaygroundWin32: public Playground {
	protected:
		PlaygroundRunner *getWinHandler() {
			return new WglRunner();
		}
		PlaygroundRunner *getVideoRunner() {
			return new VideoRunner();
		}
		PlaygroundRunner *getAudioRunner() {
			return new AudioRunner();
		}
	public:
		void init() {
			Playground::init();

			this->addRunner(this->getWinHandler());
			this->addRunner(this->getVideoRunner());
			this->addRunner(this->getAudioRunner());

		}

};
#endif /* PLAYGROUNDWIN32_H_ */
