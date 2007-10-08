/*
 *  main.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/26/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include <iostream>
#include <iomanip>
#include <tgen_core.h>
#include <tgen_math.h>
#include <tgen_graphics.h>
#include <tgen_opengl.h>
#include <tgen_renderer.h>

#include "app.h"
#include "tgen_engine.h"
#include "variablesregistry.h"
#include "sdl.h"
#include "filesystem.h"
#include "file.h"
#include <SDL/SDL.h>

int main(int argc, char ** argv) {
	uint8 debuglibs = (TGen::isCoreDebug() & 1) | (TGen::isMathDebug() & 1) << 1 | (TGen::isGraphicsDebug() & 1) << 2 
		| (TGen::isOpenGLDebug() & 1) << 3 | (TGen::isRendererDebug() & 1) << 4 | (TGen::isEngineDebug() & 1) << 5;
	
	std::cout << "TGen Engine (debug binary: " << std::boolalpha << TGen::isEngineDebug() << ")" << std::endl;
	std::cout << "   debug flags: 0x" << std::hex << std::setw(2) << std::setfill('0') << int(debuglibs) << std::endl;
	std::cout << "   version: " << TGen::Engine::VersionMajor << "." << TGen::Engine::VersionMinor << "." << TGen::Engine::VersionRevision << std::endl;
	
	std::cout << "===================== initializing ======================" << std::endl;
	
	// setup variables
	TGen::Engine::VariablesRegistry variables;
	
	variables.addVariable(TGen::Engine::Variable("env_width", "800", "800", TGen::Engine::VariableConfigWriteOnly | TGen::Engine::VariableDump));
	variables.addVariable(TGen::Engine::Variable("env_height", "600", "600", TGen::Engine::VariableConfigWriteOnly | TGen::Engine::VariableDump));
	variables.addVariable(TGen::Engine::Variable("env_fullscreen", "false", "false", TGen::Engine::VariableConfigWriteOnly | TGen::Engine::VariableDump));
	variables.addVariable(TGen::Engine::Variable("fs_game", "adrift", "adrift", TGen::Engine::VariableConfigWriteOnly));
	variables.addVariable(TGen::Engine::Variable("game_name", "TGen Engine", "TGen Engine", TGen::Engine::VariableConfigWriteOnly));
	variables.addVariable(TGen::Engine::Variable("game_author", "Peter Backman", "Peter Backman", TGen::Engine::VariableConfigWriteOnly));
	variables.addVariable(TGen::Engine::Variable("version", TGen::Engine::getVersionString(), TGen::Engine::getVersionString(), TGen::Engine::VariableReadOnly));

	// TODO: ConfigWriteOnly should only be writable until the game starts
	// TODO: dump variables to file
	
	// setup filesystem
	TGen::Engine::Filesystem * fs = new TGen::Engine::Filesystem(argv[0]);
	
	fs->addSearchPath(variables.getVariable("fs_game").getValue(), true);
	
	TGen::Engine::File * infoFile = fs->openRead("info");
	if (!infoFile)
		throw TGen::RuntimeException("main", "no info file!");
	
	TGen::PropertyTreeParser propParser;
	TGen::PropertyTree props = propParser.parse(infoFile->readAll().c_str());
	delete infoFile;
	
	variables.getVariable("game_name") = props.getProperty("game/name").second;
	variables.getVariable("game_author") = props.getProperty("game/author").second;
	
	// TODO: riktigt logsystem med targets
	// ska vara subsystems, graphics::init, sound::init, physics::init, etc. initieras av App::App
	
	
	// setup env
	TGen::Engine::SDL * sdl = new TGen::Engine::SDL(variables, props);
	TGen::Engine::App * app = new TGen::Engine::App(variables, *sdl, fs, props, sdl->getRenderer());
	
	std::cout << "======================== running ========================" << std::endl;
	
	sdl->run(app);
	
	std::cout << "===================== shutting down =====================" << std::endl;
	
	delete app;
	delete sdl;
	delete fs;
	
	std::cout << "Goodbye." << std::endl;
	
	return EXIT_SUCCESS;
}
