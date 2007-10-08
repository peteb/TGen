/*
 *  sdl.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/27/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "sdl.h"
#include "variablesregistry.h"
#include "app.h"
#include <SDL/SDL.h>
#include <tgen_opengl.h>

TGen::Engine::SDL::SDL(TGen::Engine::VariablesRegistry & variables, const TGen::PropertyTree & props)
	: variables(variables)
	, renderer(NULL)
{
	bool fullscreen = bool(variables.getVariable("env_fullscreen"));
	int width = int(variables.getVariable("env_width")), height = int(variables.getVariable("env_height"));
	uint8 bpp = 0;
	
	std::cout << "[sdl]: initializing..." << std::endl;
	std::cout << "[sdl]: width: " << width << " height: " << height << " bpp: " << int(bpp) << std::endl;
	
	uint32 initflags = SDL_INIT_VIDEO;
	uint32 videoflags = SDL_HWSURFACE | SDL_OPENGL | (fullscreen ? SDL_FULLSCREEN : 0);
	
	SDL_Surface * screen = NULL;
	
	if (SDL_Init(initflags) < 0)
		throw TGen::RuntimeException("SDL::SDL", "failed to initialize SDL: ") << SDL_GetError();
	
	// SDL_GL_SetAttribute( SDL_GL_SWAP_CONTROL, 0 );
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_WM_SetCaption(variables.getVariable("game_name").getValue().c_str(), NULL);
	//SDL_GrabMode(SDL_GRAB_ON);
	//SDL_WM_GrabInput(SDL_GRAB_ON);
	//SDL_ShowCursor(0);
	
	screen = SDL_SetVideoMode(width, height, bpp, videoflags);
	if (!screen)
		throw TGen::RuntimeException("SDL::SDL", "failed to set video mode: ") << SDL_GetError();
	
	std::cout << "[sdl]: initialized" << std::endl;	
	
	renderer = new TGen::OpenGL::Renderer;
}

TGen::Engine::SDL::~SDL() {
	delete renderer;
	//delete app;
}

int TGen::Engine::SDL::run(TGen::Engine::App * app) {
	SDL_Event event;
	
	while (app->isRunning()) {
		int events = 0;
		
		while (SDL_PollEvent(&event) && events < 100) {
			switch (event.type) {
				case SDL_KEYDOWN:
					//app->keyDown(event.key.keysym);
					break;
					
				case SDL_KEYUP:
					//gApp->KeyUp(event.key.keysym);					
					break;
					
				case SDL_MOUSEMOTION:
					//gApp->MouseMove(event.motion);
					break;
					
				case SDL_QUIT:
					app->quit();
					break;
					
			}
			
			events++;
		}
		
		app->tick();
	}
	
	return 1;
}


TGen::Renderer & TGen::Engine::SDL::getRenderer() {
	if (!renderer)
		throw TGen::RuntimeException("SDL::getRenderer", "no renderer created!");
	
	return *renderer;
}

void TGen::Engine::SDL::swapBuffers() {
	SDL_GL_SwapBuffers();
}
