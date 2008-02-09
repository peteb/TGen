/*
 *  sdl.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/27/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "sdl.h"
#include "variableregister.h"
#include "app.h"
#include "sdlkeyboard.h"
#include "sdlmouse.h"
#include "sdlgamepad.h"
#include "devicecollection.h"
#include <SDL/SDL.h>
#include <tgen_opengl.h>

TGen::Engine::SDL::SDL(TGen::Engine::VariableRegister & variables, const TGen::PropertyTree & props, TGen::Engine::StandardLogs & logs)
	: variables(variables)
	, renderer(NULL)
	, logs(logs)
	, keyboard(NULL)
	, mouse(NULL)
{
	bool fullscreen = bool(variables.getVariable("env_fullscreen"));
	int width = int(variables.getVariable("env_width")), height = int(variables.getVariable("env_height"));
	uint8 bpp = 0;
		
	logs.info["sdl+"] << "initializing..." << endl;
	logs.info["sdl+"] << std::dec << "   width: " << width << " height: " << height << " bpp: " << int(bpp) << TGen::endl;
	
	uint32 initflags = SDL_INIT_VIDEO;
	uint32 videoflags = SDL_HWSURFACE | SDL_OPENGL | (fullscreen ? SDL_FULLSCREEN : 0);
	
	SDL_Surface * screen = NULL;
	
	if (SDL_Init(initflags) < 0)
		throw TGen::RuntimeException("SDL::SDL", "failed to initialize SDL: ") << SDL_GetError();
	
	// SDL_GL_SetAttribute( SDL_GL_SWAP_CONTROL, 0 );
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	SDL_WM_SetCaption(variables.getVariable("game_name").getValue().c_str(), NULL);
	
	if (bool(variables.getVariable("r_syncVtrace")))
		SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);
	//SDL_GrabMode(SDL_GRAB_ON);
	//SDL_WM_GrabInput(SDL_GRAB_ON);
	//SDL_ShowCursor(0);
	
	screen = SDL_SetVideoMode(width, height, bpp, videoflags);
	if (!screen)
		throw TGen::RuntimeException("SDL::SDL", "failed to set video mode: ") << SDL_GetError();
	
	
	renderer = new TGen::OpenGL::Renderer;
	renderer->setViewport(TGen::Rectangle(width, height));
	logs.info["sdl+"] << "created renderer: " << TGen::endl;
	logs.info["sdl+"] << std::string(renderer->getCaps()) << TGen::endl;
	logs.info["sdl+"] << "initialized" << TGen::endl;
}

TGen::Engine::SDL::~SDL() {
	logs.info["sdl-"] << "shutting down..." << TGen::endl;

	delete renderer;
}

int TGen::Engine::SDL::run(TGen::Engine::App * app) {
	SDL_Event event;
	
	while (app->isRunning()) {
		int events = 0;
		
		while (SDL_PollEvent(&event) && events < 100) {
			switch (event.type) {
				case SDL_KEYDOWN:
					if (keyboard)
						keyboard->onBinaryEvent(event.key.keysym, TGen::Engine::StateDown);
					//app->keyDown(event.key.keysym);
					break;
					
				case SDL_KEYUP:
					if (keyboard)
						keyboard->onBinaryEvent(event.key.keysym, TGen::Engine::StateUp);
					
					break;
					
				case SDL_MOUSEMOTION:
					if (mouse)
						mouse->onMotionEvent(event.motion);
					//gApp->MouseMove(event.motion);
					break;
					
				case SDL_MOUSEBUTTONUP:
				case SDL_MOUSEBUTTONDOWN:
					if (mouse)
						mouse->onButtonEvent(event.button);
					
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

void TGen::Engine::SDL::registerInputDevices(TGen::Engine::DeviceCollection & inputDevices) {
	if (keyboard || mouse || !gamepads.empty())
		throw TGen::RuntimeException("SDL::registerInputDevices", "devices already registered!");
	
	logs.info["sdl"] << "registering input devices..." << TGen::endl;
	
	keyboard = new TGen::Engine::SDLKeyboard(0);
	mouse = new TGen::Engine::SDLMouse(0);
	
	inputDevices.addDevice(keyboard);
	inputDevices.addDevice(mouse);
	
	// TODO: deleta detta på ~ i DeviceCOllection!!!!!!!!!
	logs.info["sdl"] << SDL_NumJoysticks() << " gamepads connected" << TGen::endl;
	
	gamepads.reserve(SDL_NumJoysticks());
	
	for (int i = 0; i < SDL_NumJoysticks(); ++i) {
		try {
			inputDevices.addDevice(new TGen::Engine::SDLGamepad(i /* + devCollection.getDevicesByName("") */, i));
		}
		catch (const TGen::RuntimeException & e) {
			logs.error["SDL"] << "failed to create gamepad # " << i << TGen::endl;
		}
	}
}

