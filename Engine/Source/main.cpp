/*
 *  main.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/26/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include <SDL/SDL.h>
#include <iostream>
#include <iomanip>
#include <tgen_core.h>
#include <tgen_math.h>
#include <tgen_graphics.h>
#include <tgen_opengl.h>
#include <tgen_renderer.h>

#include "app.h"
#include "tgen_engine.h"
#include "settingsregistry.h"

int main(int argc, char ** argv) {
	uint8 debuglibs = (TGen::isCoreDebug() & 1) | (TGen::isMathDebug() & 1) << 1 | (TGen::isGraphicsDebug() & 1) << 2 
		| (TGen::isOpenGLDebug() & 1) << 3 | (TGen::isRendererDebug() & 1) << 4 | (TGen::isEngineDebug() & 1) << 5;
	
	std::cout << "TGen Engine (debug binary: " << std::boolalpha << TGen::isEngineDebug() << ")" << std::endl;
	std::cout << "   debug flags: 0x" << std::hex << std::setw(2) << std::setfill('0') << int(debuglibs) << std::endl;
	std::cout << "   version: " << TGen::Engine::VersionMajor << "." << TGen::Engine::VersionMinor << "." << TGen::Engine::VersionRevision << std::endl;
	
	std::cout << "===================== initializing ======================" << std::endl;
	
	TGen::Engine::SettingsRegistry settings;
	
	settings.addSetting(TGen::Engine::Setting("env_width", "800", "800", TGen::Engine::SettingConfigWriteOnly | TGen::Engine::SettingDump));
	settings.addSetting(TGen::Engine::Setting("env_height", "600", "600", TGen::Engine::SettingConfigWriteOnly | TGen::Engine::SettingDump));
	settings.addSetting(TGen::Engine::Setting("env_fullscreen", "false", "false", TGen::Engine::SettingConfigWriteOnly | TGen::Engine::SettingDump));
	settings.addSetting(TGen::Engine::Setting("fs_game", "adrift", "adrift", TGen::Engine::SettingConfigWriteOnly));

	
	bool fullscreen = bool(settings.getSetting("env_fullscreen"));
	int width = int(settings.getSetting("env_width")), height = int(settings.getSetting("env_height"));
	uint8 bpp = 0;
	
	std::cout << "[sdl]: initializing..." << std::endl;
	std::cout << "[sdl]: width: " << width << " height: " << height << " bpp: " << int(bpp) << std::endl;
	
	uint32 initflags = SDL_INIT_VIDEO;
	uint32 videoflags = SDL_HWSURFACE | SDL_OPENGL | (fullscreen ? SDL_FULLSCREEN : 0);
		
	SDL_Surface * screen = NULL;
		
	if (SDL_Init(initflags) < 0)
		throw TGen::RuntimeException("main", "failed to initialize SDL: ") << SDL_GetError();
		
	// SDL_GL_SetAttribute( SDL_GL_SWAP_CONTROL, 0 );
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_WM_SetCaption("TGen Engine", NULL);
	//SDL_GrabMode(SDL_GRAB_ON);
	//SDL_WM_GrabInput(SDL_GRAB_ON);
	//SDL_ShowCursor(0);
		
	screen = SDL_SetVideoMode(width, height, bpp, videoflags);
	if (!screen)
		throw TGen::RuntimeException("main", "failed to set video mode: ") << SDL_GetError();
	
	std::cout << "[sdl]: initialized" << std::endl;
	
	std::cout << "[app]: initializing..." << std::endl;
	TGen::Engine::App * app = new TGen::Engine::App;
	std::cout << "[app]: initialized" << std::endl;
	
	std::cout << "======================== running ========================" << std::endl;
	
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
	
	std::cout << "===================== shutting down =====================" << std::endl;
	
	delete app;
	SDL_Quit();
	
	std::cout << "Goodbye." << std::endl;
	
	return EXIT_SUCCESS;
}
