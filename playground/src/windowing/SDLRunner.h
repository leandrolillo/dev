/*
 * win32api.h
 *
 *  Created on: 21/11/2012
 *      Author: Lean
 */

#ifndef _WIN32

#ifndef SDLRUNNER_H_
#define SDLRUNNER_H_

#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "../core/Playground.h"


int eventFilter(void *context, SDL_Event *event);

class SDLRunner: public PlaygroundRunner {
	public:
		static const unsigned char ID = 0;

	private:
		SDL_Window *window;
		boolean fullScreen;
		Logger *logger;

	public:
		SDLRunner() {
			printf("SDL Runner constructor\n");
			fullScreen = false;
			logger = Logger::getLogger("SDLRunner.h");
			printf("retrieved logger for sdlrunner\n");
			window = null;


		}

		virtual unsigned char getId() {
			return SDLRunner::ID;
		}

		boolean getFullscreen() {
			return this->fullScreen;
		}

		virtual boolean init() {
			if (SDL_Init(SDL_INIT_VIDEO) != 0){
					logger->error("SDL_Init Error: %s", SDL_GetError());
			        return false;
			    }

			logger->debug("SDL initialized");

			SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);

			this->window = SDL_CreateWindow("SDL2/OpenGL Demo", 0, 0, 640, 480, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);

			logger->debug("SDL Window created");

			SDL_AddEventWatch(eventFilter, this);

			logger->debug("SDL event watch registered");

			return true;
		}

		virtual boolean afterInit() {
			return true;
		}

		LoopResult doLoop() {
			SDL_Event windowEvent;

			if (SDL_PollEvent( &windowEvent ) ){
				switch (windowEvent.type) {
					case SDL_QUIT:
						return STOP;
				}
			}

			return CONTINUE;
		}

		virtual int processEvent(SDL_Event *event) {
		    switch (event->type) {
		        case SDL_KEYDOWN:
		            //SDL_Log("SDL_KEYDOWN %d", event->key.keysym.sym);
		            logger->debug("KEYDOWN: %d", event->key.keysym.sym);
		            this->getContainer()->keyDown(event->key.keysym.sym);
		            return 0;
		        case SDL_KEYUP:
		            //SDL_Log("SDL_KEYUP %d", event->key.keysym.sym);
		        	logger->debug("KEYUP: %d", event->key.keysym.sym);
		        	this->getContainer()->keyUp(event->key.keysym.sym);
		            return 0;
		        case SDL_MOUSEMOTION:
		            //SDL_Log("SDL_MOUSEMOTION (%d,%d) delta=(%d,%d)", event->motion.x, event->motion.y, event->motion.xrel, event->motion.yrel);
		            this->getContainer()->mouseMove(event->motion.xrel, event->motion.yrel);
		            logger->debug("MOUSEMOVE: (%d, %d)", event->motion.xrel, event->motion.yrel);
		            return 0;
		        case SDL_MOUSEBUTTONDOWN:
		            //SDL_Log("SDL_MOUSEBUTTONDOWN %d", event->button.button);
		            logger->debug("MOUSEBUTTONDOWN: %d", event->button.button);
		        	this->getContainer()->mouseButtonDown(event->button.button);
		            return 0;
		        case SDL_MOUSEBUTTONUP:
		            //SDL_Log("SDL_MOUSEBUTTONUP %d", event->button.button);
		        	logger->debug("MOUSEBUTTONUP: %d", event->button.button);
		        	this->getContainer()->mouseButtonUp(event->button.button);
		            return 0;
		        case SDL_MOUSEWHEEL:
		            //SDL_Log("SDL_MOUSEWHEEL %d %d", event->wheel.direction, event->wheel.y);
		        	logger->debug("MOUSEWHEEL: %d", event->wheel.y);
		        	this->getContainer()->mouseWheel(event->wheel.y);
		            return 0;
		    }

		    return 1;
		}

		virtual ~SDLRunner() {
			SDL_DestroyWindow(window);
			logger->debug("SDL Window destroyed");
			SDL_Quit();
			logger->debug("SDL shutdown");
		}

//		boolean setIcon(HICON Icono) {
//			return (SetClassLong(this->getWnd(), GCL_HICON, (LONG) Icono) == 0 ?
//					false : true);
//		}
//		boolean setMenu(HMENU Menu) {
//			if (GetMenu(this->getWnd()) == NULL) {
//				RECT WndRect;
//				GetClientRect(this->getWnd(), &WndRect);
//
//				if (!::SetMenu(this->getWnd(), Menu))
//					return (false);
//
//				resize(WndRect.right - WndRect.left,
//						WndRect.bottom - WndRect.top);
//
//				return (true);
//			}
//
//			return (::SetMenu(this->getWnd(), Menu));
//		}
//		boolean setCursor(HCURSOR Cursor) {
//			return (::SetClassLong(this->getWnd(), GCL_HCURSOR,
//					(long) Cursor) == null ? false : true);
//		}

		boolean resizeWindow(unsigned int height, unsigned int width) {
			return true;
		}

		boolean move(unsigned short x, unsigned short y) {
			return true;
		}

		boolean setFullscreen(boolean fullScreen) {
			if (fullScreen) {
				// do fullscreen stuff
				this->fullScreen = true;
			} else {
				// do fullscreen stuff
				this->fullScreen = false;
			}

			return this->fullScreen;
		}
		void show(boolean show)
		{
		}

	protected:

};

int eventFilter(void *context, SDL_Event *event) {
	SDLRunner *runner = (SDLRunner *) context;
		return runner->processEvent(event);
}

#endif /* SDLRUNNER_H_ */
#endif /* _WIN32*/
