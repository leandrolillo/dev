#include <iostream>
#include <stdio.h>

#include "openGL/OpenGLRunner.h"

class PhysicsPlayground: public Playground {
private:
	PlaygroundRunner *windowsRunner;
protected:
	PlaygroundRunner *getWindowRunner() {
		if (windowsRunner == null) {
			windowsRunner = new OpenGLRunner();
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
