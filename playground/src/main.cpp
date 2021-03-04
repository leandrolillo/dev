#include <iostream>
#include <stdio.h>

#ifdef _WIN32
	#include "windowing/win32apiRunner.h"
#else
	#include "windowing/SDLRunner.h"
#endif

class PhysicsPlayground: public Playground {
private:
	PlaygroundRunner *windowsRunner;
protected:
	PlaygroundRunner *getWindowRunner() {
		if (windowsRunner == null) {
			#ifdef _WIN32
			windowsRunner = new WglRunner();
			#else
			windowsRunner = new SDLRunner();
			#endif
		}
		return windowsRunner;
	}
public:
	PhysicsPlayground() {
		this->windowsRunner = null;
	}
	void init() {
		Playground::init();

		this->addRunner(this->getWindowRunner());
	}

};

int main(int, char**){
    PhysicsPlayground playground;
    
    playground.run();
    
    return 0;
}
