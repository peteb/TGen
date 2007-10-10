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

int run(int argc, char ** argv, TGen::Engine::StandardLogs & logs);

int main(int argc, char ** argv) {
	try {
		// initialize log system
		TGen::Engine::StandardLogs logs;
		std::ios_base::sync_with_stdio(false);
	
		try {
			return run(argc, argv, logs);
		}
		catch (TGen::RuntimeException & e) {
			logs.error << e;
			logs.error << "unhandled exception, quitting..." << TGen::endl;
			return EXIT_FAILURE;
		}
		catch (std::exception & e) {
			logs.error["unhandled"] << e.what() << TGen::endl;
			logs.error << "unhandled exception, quitting..." << TGen::endl;
			return EXIT_FAILURE;
		}
	}
	catch (std::exception & e) {
		std::cerr << "ERROR IN STRAPPER: " << e.what() << std::endl;
		std::cerr << "unhandled exception, quitting..." << std::endl;
		return EXIT_FAILURE;
	}
	catch (...) {
		std::cerr << "UNKNOWN ERROR IN STRAPPER!" << std::endl;
		std::cerr << "unhandled exception, quitting..." << std::endl;
		return EXIT_FAILURE;
	}
}

int run(int argc, char ** argv, TGen::Engine::StandardLogs & logs) {
	uint8 debuglibs = (TGen::isCoreDebug() & 1) | (TGen::isMathDebug() & 1) << 1 | (TGen::isGraphicsDebug() & 1) << 2 
	| (TGen::isOpenGLDebug() & 1) << 3 | (TGen::isRendererDebug() & 1) << 4 | (TGen::isEngineDebug() & 1) << 5;
	
	
	logs.info["str+"] << "TGen Engine (debug binary: " << std::boolalpha << TGen::isEngineDebug() << ")" << TGen::endl;
	logs.info["str+"] << "   debug flags: 0x" << std::hex << std::setw(2) << std::setfill('0') << int(debuglibs) << TGen::endl;
	logs.info["str+"] << "   version: " << TGen::Engine::VersionMajor << "." << TGen::Engine::VersionMinor << "." << TGen::Engine::VersionRevision << TGen::endl;
	
	logs.info << TGen::separator("initializing");
	
	
	
	
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
	TGen::Engine::Filesystem * fs = new TGen::Engine::Filesystem(argv[0], logs);
	
	fs->addSearchPath(variables.getVariable("fs_game").getValue(), true);
	
	
	
	
	// read info file
	TGen::Engine::File * infoFile = fs->openRead("info");
	if (!infoFile)
		throw TGen::RuntimeException("main", "no info file!");
	
	TGen::PropertyTreeParser propParser;
	TGen::PropertyTree props = propParser.parse(infoFile->readAll().c_str());
	delete infoFile;
	
	variables["game_name"] = props.getProperty("game/name").second;
	variables["game_author"] = props.getProperty("game/author").second;
	
	
	
	
	
	// setup env
	TGen::Engine::SDL * sdl = new TGen::Engine::SDL(variables, props, logs);
	TGen::Engine::App * app = new TGen::Engine::App(variables, *sdl, fs, props, sdl->getRenderer(), logs);
	
	
	
	
	// run forrest
	logs.info << TGen::separator("running");
	
	sdl->run(app);
	
	logs.info << TGen::separator("shutting down");
	
	
	
	// shut down
	
	delete app;
	delete sdl;
	delete fs;
	
	logs.info["str-"] << "Goodbye, have a nice day!" << TGen::endl;
	
	return EXIT_SUCCESS;	
}

