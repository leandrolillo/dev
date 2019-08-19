#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>

int eventFilter(void *context, SDL_Event *event) {
    switch (event->type) {
        case SDL_KEYDOWN:
            SDL_Log("SDL_KEYDOWN %d", event->key.keysym.sym);
            return 0;
        case SDL_KEYUP:
            SDL_Log("SDL_KEYUP %d", event->key.keysym.sym);
            return 0;
        case SDL_MOUSEMOTION:
            SDL_Log("SDL_MOUSEMOTION (%d,%d) delta=(%d,%d)", event->motion.x, event->motion.y, event->motion.xrel, event->motion.yrel);
            return 0;
        case SDL_MOUSEBUTTONDOWN:
            SDL_Log("SDL_MOUSEBUTTONDOWN %d", event->button.button);
            return 0;
        case SDL_MOUSEBUTTONUP:
            SDL_Log("SDL_MOUSEBUTTONUP %d", event->button.button);
            return 0;
        case SDL_MOUSEWHEEL:
            SDL_Log("SDL_MOUSEWHEEL %d %d", event->wheel.direction, event->wheel.y);
            return 0;
    }

    return 1;
}

/*
 * Lesson 0: Test to make sure SDL is setup properly
 */
int main(int, char**){
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    
    
    SDL_Window *window = SDL_CreateWindow("SDL2/OpenGL Demo", 0, 0, 640, 480, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
  
    SDL_AddEventWatch(eventFilter, nullptr);
    // Create an OpenGL context associated with the window.
    SDL_GLContext glcontext = SDL_GL_CreateContext(window);

    unsigned char running = 1;
    while ( running )
    {
        SDL_Event windowEvent;
        
        if ( SDL_PollEvent( &windowEvent ) )
        {
            switch (windowEvent.type) {
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_APP_DIDENTERFOREGROUND:
                    SDL_Log("SDL_APP_DIDENTERFOREGROUND");
                    break;
                case SDL_APP_DIDENTERBACKGROUND:
                    SDL_Log("SDL_APP_DIDENTERBACKGROUND");
                    break;
                case SDL_WINDOWEVENT:
                    switch (windowEvent.window.event) {
                        case SDL_WINDOWEVENT_SHOWN:
                            SDL_Log("SDL_WINDOWEVENT_SHOWN %d", windowEvent.window.windowID);
                            break;
                        case SDL_WINDOWEVENT_HIDDEN:
                            SDL_Log("SDL_WINDOWEVENT_HIDDEN %d", windowEvent.window.windowID);
                            break;
                        case SDL_WINDOWEVENT_MOVED:
                            SDL_Log("SDL_WINDOWEVENT_MOVED %d", windowEvent.window.windowID);
                            break;
                            
                        default:
                            SDL_Log("SDL_WINDOW_EVENT: %d %d", windowEvent.window.event, windowEvent.window.windowID);
                            
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        
        // Clear the colorbuffer
        glClearColor( rand() % 255 / 255.0f, rand() % 255 / 255.0f, rand() % 255 / 255.0f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );
        
        // draw OpenGL
        
        SDL_GL_SwapWindow( window );
    }
    

    // Once finished with OpenGL functions, the SDL_GLContext can be deleted.
    SDL_GL_DeleteContext(glcontext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
