/*
 *  newmain.cpp
 *  The Quaker
 *
 *  Created by Peter Backman on 8/1/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include <tgen_opengl.h>
#include <tgen_core.h>
#include <SDL/SDL.h>
#include "app.h"

extern App * gApp;

int main(int argc, char ** argv) {
	std::cout << "TGen 'The Quaker' demo (debug binary: " << std::boolalpha << TGen::isOpenGLDebug() << ")" << std::endl << std::endl;	
	std::cout << "[sdl]: initializing..." << std::endl;
	
	Uint32 initflags = SDL_INIT_VIDEO;
	Uint32 videoflags = SDL_HWSURFACE | SDL_OPENGL;// | SDL_FULLSCREEN;
		
		SDL_Surface * screen = NULL;
		Uint8 video_bpp = 0;
		
		if (SDL_Init(initflags) < 0)
			throw TGen::RuntimeException("main", "failed to initialize SDL: ") << SDL_GetError();
		
		SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, 16);
		SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
		SDL_WM_SetCaption("TGen 'The Quaker' demo", NULL);
		//SDL_GrabMode(SDL_GRAB_ON);
		SDL_WM_GrabInput(SDL_GRAB_ON);
		SDL_ShowCursor(0);
		
		screen = SDL_SetVideoMode(800, 600, video_bpp, videoflags);
		if (!screen) {
			throw TGen::RuntimeException("main", "failed to set video mode: ") << SDL_GetError();
			//SDL_Quit();
		}
		
		
		SDL_GetRelativeMouseState(NULL, NULL);
		
		SDL_Event event;
		gApp = new App;
		std::cout << "======================== running ========================" << std::endl;
		
		while (gApp->isRunning()) {
			int events = 0;
			
			while (SDL_PollEvent(&event) && events < 100) {
				switch (event.type) {
					case SDL_KEYDOWN:
						gApp->KeyDown(event.key.keysym);
						break;
						
					case SDL_KEYUP:
						gApp->KeyUp(event.key.keysym);					
						break;
						
					case SDL_MOUSEMOTION:
						gApp->MouseMove(event.motion);
						break;
						
					case SDL_QUIT:
						gApp->Quit();
						break;
						
				}
				
				events++;
			}
			
			gApp->Update();
			gApp->Render();
		}
		
		std::cout << "===================== shutting down =====================" << std::endl;
		
		delete gApp;
		
		/*done = 0;
		while ( !done ) {
			
			while ( SDL_PollEvent(&event) ) {
				switch (event.type) {
					
					case SDL_MOUSEMOTION:
						break;
					case SDL_MOUSEBUTTONDOWN:
						break;
					case SDL_KEYDOWN:
					case SDL_QUIT:
						done = 1;
						break;
					default:
						break;
				}
			}
		}
		*/
		
		SDL_Quit();
		
		return EXIT_SUCCESS;
}


