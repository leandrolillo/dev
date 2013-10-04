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
	CREATED,
	INITIALIZED,
	RUNNERS_BROKEN,
	RUNNERS_INITIALIZED,
	RUNNING,
	STOPPED,
	ERROR
};

enum LoopResult {
	CONTINUE, SKIP, STOP, FINISHED
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
	Playground *container;boolean enabled;

public:
	PlaygroundRunner() {
		container = null;
		enabled = true;
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

	virtual void mouseMove(int dx, int dy) {

	}

	unsigned char getEnabled() const {
		return enabled;
	}

	void setEnabled(unsigned char enabled) {
		this->enabled = enabled;
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

	virtual ResourceManager *getResourceManager() {
		if (resourceManager == null)
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
		for (std::vector<PlaygroundRunner *>::iterator currentRunnerIterator =
				runners.begin(); currentRunnerIterator != runners.end();
				currentRunnerIterator++) {
			if ((*currentRunnerIterator)->getId() == runner->getId()) {
				logger->error("Runner with id [%d] already added - skipping",
						runner->getId());
				return;
			}
		}

		runner->setContainer(this);
		runners.push_back(runner);
		runners_by_id[runner->getId()] = runner;
	}

	// creación de los hilos
	virtual void init() {
		logger->debug("framework initialized");
	}

	virtual boolean initRunners() {
		for (std::vector<PlaygroundRunner *>::iterator currentRunnerIterator =
				runners.begin(); currentRunnerIterator != runners.end();
				currentRunnerIterator++) {
			if (!(*currentRunnerIterator)->init()) {
				logger->error("Failed to initialize runner [%d]",
						(*currentRunnerIterator)->getId());
				return false;
			}

			unsigned char interests = (*currentRunnerIterator)->getInterests();

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
				this->mouseButtonUpObservers.push_back(*currentRunnerIterator);
			if (interests & MOUSE_BUTTON_DOWN)
				this->moveObservers.push_back(*currentRunnerIterator);
		}

		for (std::vector<PlaygroundRunner *>::iterator currentRunnerIterator =
				runners.begin(); currentRunnerIterator != runners.end();
				currentRunnerIterator++) {
			if (!(*currentRunnerIterator)->afterInit()) {
				logger->error("Failed to after initialize runner [%d]",
						(*currentRunnerIterator)->getId());
				return false;
			}
		}

		return true;
	}

	virtual void run() {
		if (status == CREATED) {
			init();
			status = INITIALIZED;
		}

		if (status == INITIALIZED) {
			if (initRunners()) {
				status = RUNNERS_INITIALIZED;
				logger->debug("Runners initialization completed");
			} else {
				status = RUNNERS_BROKEN;
				logger->error("Runners initialization failed");
			}
		}

		if (status == RUNNERS_INITIALIZED || status == STOPPED) {
			logger->debug("Entering loop");

			status = RUNNING;

			while (status == RUNNING)
				this->loop();
		}
		logger->debug("Finished looping");
	}

	virtual void stop() {
		status = STOPPED;
	}

	virtual void loop() {
		try {
			for (std::vector<PlaygroundRunner *>::iterator currentRunnerIterator =
					runners.begin(); currentRunnerIterator != runners.end();
					currentRunnerIterator++)
				if ((*currentRunnerIterator)->getEnabled())
					(*currentRunnerIterator)->beforeLoop();

			unsigned int activeRunners = 0;
			for (std::vector<PlaygroundRunner *>::iterator currentRunnerIterator =
					runners.begin(); currentRunnerIterator != runners.end();
					currentRunnerIterator++) {
				if ((*currentRunnerIterator)->getEnabled()) {
					LoopResult result = (*currentRunnerIterator)->doLoop();

					if (result != CONTINUE) {
						if (result == STOP) {
							this->status = STOPPED;
						} else if (result == FINISHED) {
							runners.erase(currentRunnerIterator);
							delete *currentRunnerIterator;
						} else // result == SKIP
							activeRunners++;

						break;
					} else
						activeRunners++;
				}
			}

			for (std::vector<PlaygroundRunner *>::iterator currentRunnerIterator =
					runners.begin(); currentRunnerIterator != runners.end();
					currentRunnerIterator++)
				if ((*currentRunnerIterator)->getEnabled())
					(*currentRunnerIterator)->afterLoop();

			if (activeRunners == 0) {
				this->status = STOPPED;
				logger->debug("There're no runners active");
			}

		} catch (Exception &exception) {
			logger->error("Loop broken: [%s]", exception.getMessage().c_str());
			this->status = ERROR;
		}
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

	void keyUp(unsigned int key) {
		for (std::vector<PlaygroundRunner *>::iterator currentRunnerIterator =
				keyUpObservers.begin();
				currentRunnerIterator != keyUpObservers.end();
				currentRunnerIterator++)
			(*currentRunnerIterator)->keyUp(key);
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
