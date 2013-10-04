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
private:
	WglRunner *wglRunner;
	VideoRunner *videoRunner;
	AudioRunner *audioRunner;
protected:
	PlaygroundRunner *getWinHandler() {
		if (wglRunner == null)
			wglRunner = new WglRunner();

		return wglRunner;
	}
	PlaygroundRunner *getVideoRunner() {
		if (videoRunner == null)
			videoRunner = new VideoRunner();

		return videoRunner;
	}
	PlaygroundRunner *getAudioRunner() {
		if (audioRunner == null)
			audioRunner = new AudioRunner();

		return audioRunner;
	}
public:
	PlaygroundWin32() {
		this->audioRunner = null;
		this->videoRunner = null;
		this->wglRunner = null;
	}
	void init() {
		Playground::init();

		this->addRunner(this->getWinHandler());
		this->addRunner(this->getVideoRunner());
		this->addRunner(this->getAudioRunner());

	}

};
#endif /* PLAYGROUNDWIN32_H_ */
