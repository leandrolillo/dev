/*
 * playground.h
 *
 *  Created on: 03/11/2012
 *      Author: Lean
 */

#ifndef PLAYGROUND_H_
#define PLAYGROUND_H_

#include <ResourceManager.h>
#include <Logger.h>
#include <JavaLike.h>
#include <vector>
#include <map>

enum class PlaygroundStatus {
	CREATED,
	INITIALIZED,
	RUNNERS_BROKEN,
	RUNNERS_INITIALIZED,
	RUNNING,
	STOPPED,
	ERROR
};

enum class LoopResult {
	CONTINUE, SKIP, STOP, FINISHED
};

enum Interests {
	RESIZE = 1,
	MOVE = 2,
	KEY_DOWN = 4,
	KEY_UP = 8,
	MOUSE_MOVE = 16,
	MOUSE_BUTTON_DOWN = 32,
	MOUSE_BUTTON_UP = 64,
	MOUSE_WHEEL = 128
};

class Playground;

class PlaygroundRunner {
private:
	Playground *container = null;
	bool _enabled = true;

public:
	PlaygroundRunner() {
	}

	virtual ~PlaygroundRunner() {
    }

	virtual bool init() {
		return true;
	}
	virtual bool afterInit() {
		return true;
	}
	virtual void beforeLoop() {
	}
	virtual LoopResult doLoop() {
		return LoopResult::CONTINUE;
	}
	virtual void afterLoop() {
	}

	void setContainer(Playground *container) {
		this->container = container;
	}
	Playground *getContainer() {
		return this->container;
	}

	virtual unsigned char getId() const = 0;

	virtual unsigned char getInterests() {
		return 0;
	}

	virtual void onResize(unsigned int height, unsigned width) {
	}

	virtual void onKeyDown(unsigned int key, unsigned int keyModifier) {
	}

	virtual void onKeyUp(unsigned int key, unsigned int keyModifier) {
	}

	virtual void onMouseMove(int x, int y, int dx, int dy) {
	}

	virtual void onMouseButtonDown(unsigned char button, int x, int y) {
	}

	virtual void onMouseButtonUp(unsigned char button, int x, int y) {
	}

	virtual void onMouseWheel(int wheel) {
	}

	unsigned char getEnabled() const {
		return _enabled;
	}

	void setEnabled(unsigned char enabled) {
		this->_enabled = enabled;
	}
};

class Playground {
private:
	Logger *logger = LoggerFactory::getLogger("core/Playground.h");
	ResourceManager *resourceManager = null;
	String resourcesRootFolder;
	std::vector<std::unique_ptr<PlaygroundRunner>> runners;
	std::map<unsigned char, PlaygroundRunner *> runners_by_id;
	PlaygroundStatus status = PlaygroundStatus::CREATED;
	std::vector<PlaygroundRunner *> resizeObservers;
	std::vector<PlaygroundRunner *> moveObservers;
	std::vector<PlaygroundRunner *> keyDownObservers;
	std::vector<PlaygroundRunner *> keyUpObservers;
	std::vector<PlaygroundRunner *> mouseMoveObservers;
	std::vector<PlaygroundRunner *> mouseButtonDownObservers;
	std::vector<PlaygroundRunner *> mouseButtonUpObservers;
	std::vector<PlaygroundRunner *> mouseWheelObservers;

public:
	Playground() : Playground("Lean/dev/media/"){
	}

	Playground(const String &rootFolder) {
		resourcesRootFolder = rootFolder;
	}

	virtual ResourceManager *getResourceManager() {
		if (resourceManager == null) {
			logger->debug("Creating resource manager");
			resourceManager = new ResourceManager(this->resourcesRootFolder);
		}

		return resourceManager;

	}

	PlaygroundStatus getStatus() const {
		return status;
	}

	void setStatus(const PlaygroundStatus &status) {
		this->status = status;
	}

	PlaygroundRunner *getRequiredRunner(const unsigned char id) const {
	    if(runners_by_id.at(id) == null) {
	        throw InvalidArgumentException("Could not find required playground runner");
	    }

	    return runners_by_id.at(id);
	}

	PlaygroundRunner *getRunner(const unsigned char id) const {
		return runners_by_id.at(id);
	}

	void addRunner(PlaygroundRunner *runner) {
		logger->debug("Adding runner with id [%d]", runner->getId());

		/**
		 * Check for duplicates
		 */
		for (auto &currentRunner : runners) {
			if (currentRunner->getId() == runner->getId()) {
				logger->error("Runner with id [%d] already added - skipping", runner->getId());
				return;
			} else {
			    logger->debug("Added runner with id [%d]", runner->getId());
			}
		}

		/**
		 * Insert ordered by id
		 */
		std::vector<std::unique_ptr<PlaygroundRunner>>::iterator currentRunnerIterator = runners.begin();
		while(currentRunnerIterator != runners.end() && (*currentRunnerIterator)->getId() < runner->getId()) {
			currentRunnerIterator++;
		}

		runner->setContainer(this);
		runners.insert(currentRunnerIterator, std::unique_ptr<PlaygroundRunner>(runner));
		runners_by_id[runner->getId()] = runner;
	}

	// "thread" creation
	virtual void init() {
		logger->debug("framework initialized");
	}

	virtual bool initRunners() {
		logger->debug("initializing runners:");

		for (auto &currentRunner : runners) {
			logger->debug("Initializing runner [%d]", currentRunner->getId());

			try {
                if (!currentRunner->init()) {
                    logger->error("Failed to initialize runner [%d]", currentRunner->getId());
                    return false;
                }
			} catch(Exception &exception) {
			    logger->error("Failed to initialize runner [%d]: [%s]", currentRunner->getId(), exception.toString().c_str());
			    return false;
			} catch(...) {
                logger->error("Failed to initialize runner [%d]", currentRunner->getId());
                return false;
			}

			unsigned char interests = currentRunner->getInterests();

			if (interests & RESIZE)
				this->resizeObservers.push_back(currentRunner.get());
			if (interests & MOVE)
				this->moveObservers.push_back(currentRunner.get());
			if (interests & KEY_DOWN)
				this->keyDownObservers.push_back(currentRunner.get());
			if (interests & KEY_UP)
				this->keyUpObservers.push_back(currentRunner.get());
			if (interests & MOUSE_MOVE)
				this->mouseMoveObservers.push_back(currentRunner.get());
			if (interests & MOUSE_BUTTON_UP)
				this->mouseButtonUpObservers.push_back(currentRunner.get());
			if (interests & MOUSE_BUTTON_DOWN)
				this->mouseButtonDownObservers.push_back(currentRunner.get());
			if (interests & MOUSE_WHEEL)
				this->mouseWheelObservers.push_back(currentRunner.get());
		}

		for (auto &currentRunner : runners) {
		    try {
                if (!currentRunner->afterInit()) {
                    logger->error("Failed to after initialize runner [%d]", currentRunner->getId());
                    return false;
                } else {
                    logger->debug("Successful initialization of runner [%d]", currentRunner->getId());
                }
		    } catch(Exception &exception) {
                logger->error("Failed to initialize runner [%d]: [%s]", currentRunner->getId(), exception.toString().c_str());
                return false;
            } catch(...) {
                logger->error("Failed to after initialize runner [%d]", currentRunner->getId());
                return false;
		    }
		}

		return true;
	}

	virtual void run() {
		if (status == PlaygroundStatus::CREATED) {
			logger->debug("Initializing playground");
			init();
			status = PlaygroundStatus::INITIALIZED;
		}

		if (status == PlaygroundStatus::INITIALIZED) {
			if (initRunners()) {
				status = PlaygroundStatus::RUNNERS_INITIALIZED;
				logger->info("Initialization completed");
			} else {
				status = PlaygroundStatus::RUNNERS_BROKEN;
				logger->error("Runners initialization failed");
			}
		}

		if (status == PlaygroundStatus::RUNNERS_INITIALIZED || status == PlaygroundStatus::STOPPED) {
			logger->verbose("Entering loop");

			status = PlaygroundStatus::RUNNING;

			while (status == PlaygroundStatus::RUNNING)
				this->loop();
		}
		logger->debug("Finished looping... shutting down");
		shutdown();
		logger->info("done.");
	}

	virtual void stop() {
		status = PlaygroundStatus::STOPPED;
	}

	virtual void loop() {
		try {
			logger->verbose("Calling enabled runners beforeLoop");
			for (auto &currentRunner : runners) {
				if (currentRunner->getEnabled()) {
					currentRunner->beforeLoop();
				}
			}

			logger->verbose("Calling enabled runners doLoop");
			unsigned int activeRunners = 0;
			for (std::vector<std::unique_ptr<PlaygroundRunner>>::iterator currentRunnerIterator =
					runners.begin(); currentRunnerIterator != runners.end();
					currentRunnerIterator++) {
				if ((*currentRunnerIterator)->getEnabled()) {
					logger->verbose("Running Loop of %d", (*currentRunnerIterator)->getId());
					LoopResult result = (*currentRunnerIterator)->doLoop();

					if (result != LoopResult::CONTINUE) {
						if (result == LoopResult::STOP) {
							this->status = PlaygroundStatus::STOPPED;
						} else if (result == LoopResult::FINISHED) {
							runners.erase(currentRunnerIterator);
						} else // result == SKIP
							activeRunners++;

						break;
					} else
						activeRunners++;
				}
			}

			logger->verbose("Calling enabled runners afterLoop");
			for (auto &currentRunner : runners) {
				if (currentRunner->getEnabled()) {
					currentRunner->afterLoop();
				}
			}

			if (activeRunners == 0) {
				this->status = PlaygroundStatus::STOPPED;
				logger->debug("There're no more runners active... stopping");
			}

		} catch (Exception &exception) {
			logger->error("Loop broken: [%s]", exception.getMessage().c_str());
			this->status = PlaygroundStatus::ERROR;
		}
	}

	void onResize(unsigned int height, unsigned width) {
		for (auto currentRunner : resizeObservers) {
			currentRunner->onResize(height, width);
		}
	}

	void onKeyDown(unsigned int key, unsigned int keyModifier) {
		for (auto currentRunner : keyDownObservers) {
			currentRunner->onKeyDown(key, keyModifier);
		}
	}

	void onKeyUp(unsigned int key, unsigned int keyModifier) {
		for (auto currentRunner : keyUpObservers) {
			currentRunner->onKeyUp(key, keyModifier);
		}
	}

	void onMouseMove(int x, int y, int dx, int dy) {
		for (auto currentRunner : mouseMoveObservers) {
			currentRunner->onMouseMove(x, y, dx, dy);
		}
	}

	void onMouseButtonDown(unsigned char button, int x, int y) {
		for (auto currentRunner : mouseButtonDownObservers) {
			currentRunner->onMouseButtonDown(button, x, y);
		}
	}

	void onMouseButtonUp(unsigned char button, int x, int y) {
		for (auto currentRunner : mouseButtonUpObservers) {
			currentRunner->onMouseButtonUp(button, x, y);
		}
	}

	void onMouseWheel(int wheel) {
		for (auto currentRunner : mouseWheelObservers) {
			currentRunner->onMouseWheel(wheel);
		}
	}

	virtual void shutdown() {
		//Clearing vectors is not really necessary.
        this->keyDownObservers.clear();
        this->keyUpObservers.clear();
        this->resizeObservers.clear();
        this->moveObservers.clear();
        this->mouseMoveObservers.clear();
        this->mouseButtonDownObservers.clear();
        this->mouseButtonUpObservers.clear();

        //No longer necessary if using unique_ptr
//        for (std::vector<PlaygroundRunner *>::iterator currentRunnerIterator =
//                runners.begin(); currentRunnerIterator != runners.end();
//                currentRunnerIterator++) {
//            delete (*currentRunnerIterator);
//        }

        delete resourceManager;

        LoggerFactory::deleteLoggers();
	}

	virtual ~Playground() {
	}
};
#endif /* PLAYGROUND_H_ */
