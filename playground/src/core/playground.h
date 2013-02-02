/*
 * playground.h
 *
 *  Created on: 03/11/2012
 *      Author: Lean
 */

#ifndef PLAYGROUND_H_
#define PLAYGROUND_H_

#include "javaLike.h"
#include "resources/ResourceManager.h"
#include "log/logger.h"
#include <vector>
#include <map>

enum PlaygroundStatus {
	CREATED, INITIALIZED, RUNNERS_BROKEN, RUNNERS_INITIALIZED, RUNNING, STOPPED
};

enum LoopResult {
	CONTINUE, SKIP, STOP
};

enum Interests {
	RESIZE = 1,
	MOVE = 2,
	KEY_DOWN = 4,
	KEY_UP = 8,
	MOUSE_MOVE = 16,
	MOUSE_BUTTON_DOWN = 32,
	MOUSE_BUTTON_UP = 64
};

class Playground;

class PlaygroundRunner {
	private:
		Playground *container;
	public:
		PlaygroundRunner() {
			container = null;
		}
		virtual boolean init() {
			return true;
		}
		virtual boolean afterInit() {
					return true;
		}
		virtual void beforeLoop() {
		}
		virtual LoopResult doLoop() {
			return CONTINUE;
		}
		virtual void afterLoop() {
		}
		virtual ~PlaygroundRunner() {
		}

		void setContainer(Playground *container) {
			this->container = container;
		}
		Playground *getContainer() {
			return this->container;
		}

		virtual unsigned char getId() = 0;

		virtual unsigned char getInterests() {
			return 0;
		}

		virtual void resize(unsigned int height, unsigned width) {

		}

		virtual void keyDown(unsigned int key) {

		}
		virtual void keyUp(unsigned int key) {

		}

		virtual void mouseMove(int dx,
				int dy) {

		}
};

class Playground {
	private:
		Logger *logger;
		ResourceManager *resourceManager;
		std::vector<PlaygroundRunner *> runners;
		std::map<unsigned char, PlaygroundRunner *> runners_by_id;
		PlaygroundStatus status;
		std::vector<PlaygroundRunner *> resizeObservers;
		std::vector<PlaygroundRunner *> moveObservers;
		std::vector<PlaygroundRunner *> keyDownObservers;
		std::vector<PlaygroundRunner *> keyUpObservers;
		std::vector<PlaygroundRunner *> mouseMoveObservers;
		std::vector<PlaygroundRunner *> mouseButtonDownObservers;
		std::vector<PlaygroundRunner *> mouseButtonUpObservers;

	public:
		Playground() {
			logger = Logger::getLogger("core/Playground.h");
			status = CREATED;
			resourceManager = null;
			resourceManager = this->getResourceManager();
		}

		virtual ResourceManager *getResourceManager()
		{
			if(resourceManager == null)
				resourceManager = new ResourceManager("/Lean/dev/media/");

			return resourceManager;

		}

		PlaygroundStatus getStatus() {
			return status;
		}

		void setStatus(const PlaygroundStatus &status) {
			this->status = status;
		}

		PlaygroundRunner *getRunner(const unsigned char &id) {
			return runners_by_id[id];
		}

		void addRunner(PlaygroundRunner *runner) {
			runner->setContainer(this);
			runners.push_back(runner);
			runners_by_id[runner->getId()] = runner;
		}

		// creación de los hilos
		virtual void init() {
			status = INITIALIZED;
			logger->debug("framework initialized");
		}

		virtual void initRunners() {
			for (std::vector<PlaygroundRunner *>::iterator currentRunnerIterator =
					runners.begin(); currentRunnerIterator != runners.end();
					currentRunnerIterator++) {
				if (!(*currentRunnerIterator)->init()) {
					status = RUNNERS_BROKEN;
					return;
				}

				unsigned char interests =
						(*currentRunnerIterator)->getInterests();

				if (interests & RESIZE)
					this->resizeObservers.push_back(*currentRunnerIterator);
				if (interests & MOVE)
					this->moveObservers.push_back(*currentRunnerIterator);
				if (interests & KEY_DOWN)
					this->keyDownObservers.push_back(*currentRunnerIterator);
				if (interests & KEY_UP)
					this->keyUpObservers.push_back(*currentRunnerIterator);
				if (interests & MOUSE_MOVE)
					this->mouseMoveObservers.push_back(*currentRunnerIterator);
				if (interests & MOUSE_BUTTON_UP)
					this->mouseButtonUpObservers.push_back(
							*currentRunnerIterator);
				if (interests & MOUSE_BUTTON_DOWN)
					this->moveObservers.push_back(*currentRunnerIterator);
			}

			for (std::vector<PlaygroundRunner *>::iterator currentRunnerIterator =
								runners.begin(); currentRunnerIterator != runners.end();
								currentRunnerIterator++) {
							if (!(*currentRunnerIterator)->afterInit()) {
								status = RUNNERS_BROKEN;
								return;
							}
			}


			status = RUNNERS_INITIALIZED;
		}

		virtual void run() {
			if (status == INITIALIZED)
				initRunners();

			if (status == RUNNERS_INITIALIZED || status == STOPPED) {
				status = RUNNING;

				while (status == RUNNING)
					this->loop();
			}
		}

		virtual void stop() {
			status = STOPPED;
		}

		virtual void loop() {
			for (std::vector<PlaygroundRunner *>::iterator currentRunnerIterator =
					runners.begin(); currentRunnerIterator != runners.end();
					currentRunnerIterator++)
				(*currentRunnerIterator)->beforeLoop();

			for (std::vector<PlaygroundRunner *>::iterator currentRunnerIterator =
					runners.begin(); currentRunnerIterator != runners.end();
					currentRunnerIterator++) {
				LoopResult result = (*currentRunnerIterator)->doLoop();

				if (result != CONTINUE) {
					if (result == STOP)
						this->status = STOPPED;
					break;
				}
			}

			for (std::vector<PlaygroundRunner *>::iterator currentRunnerIterator =
					runners.begin(); currentRunnerIterator != runners.end();
					currentRunnerIterator++)
				(*currentRunnerIterator)->afterLoop();
		}

		void resize(unsigned int height, unsigned width) {
			for (std::vector<PlaygroundRunner *>::iterator currentRunnerIterator =
					resizeObservers.begin();
					currentRunnerIterator != resizeObservers.end();
					currentRunnerIterator++)
				(*currentRunnerIterator)->resize(height, width);
		}

		void keyDown(unsigned int key) {
			for (std::vector<PlaygroundRunner *>::iterator currentRunnerIterator =
								keyDownObservers.begin();
								currentRunnerIterator != keyDownObservers.end();
								currentRunnerIterator++)
							(*currentRunnerIterator)->keyDown(key);
		}

		void mouseMove(int dx, int dy) {
			for (std::vector<PlaygroundRunner *>::iterator currentRunnerIterator =
								mouseMoveObservers.begin();
								currentRunnerIterator != mouseMoveObservers.end();
								currentRunnerIterator++)
							(*currentRunnerIterator)->mouseMove(dx, dy);
		}

		virtual ~Playground() {
			this->keyDownObservers.clear();
			this->keyUpObservers.clear();
			this->resizeObservers.clear();
			this->moveObservers.clear();
			this->mouseMoveObservers.clear();
			this->mouseButtonDownObservers.clear();
			this->mouseButtonUpObservers.clear();

			delete resourceManager;

			for (std::vector<PlaygroundRunner *>::iterator currentRunnerIterator =
					runners.begin(); currentRunnerIterator != runners.end();
					currentRunnerIterator++)
				delete (*currentRunnerIterator);


			Logger::deleteLoggers();
		}
};
#endif /* PLAYGROUND_H_ */
