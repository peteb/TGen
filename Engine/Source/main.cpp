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
#include <algorithm>
#include <tgen_core.h>
#include <tgen_math.h>
#include <tgen_graphics.h>
#include <tgen_opengl.h>
#include <tgen_renderer.h>
#include <SDL/SDL.h>

#include "app.h"
#include "log.h"
#include "logtarget.h"
#include "tgen_engine.h"
#include "variableregister.h"
#include "commandregister.h"
#include "sdl.h"
#include "filesystem.h"
#include "file.h"
#include "cmdset.h"
#include "cmddumpvars.h"
#include "devicecollection.h"

#ifdef _PLATFORM_OSX
#define _PLATFORM_FILE "platform_cocoa.h"
#endif

#ifdef _PLATFORM_WINDOWS
#define _PLATFORM_FILE "platform_windows.h"
#endif

#include _PLATFORM_FILE
#include "platform_cocoa.h"

#include <sys/time.h>

using TGen::uint8;

int run(int argc, char ** argv, TGen::Engine::StandardLogs & logs);
void preExit();
// FIXA BINÄR och stoppa det här i egen fil

FILE * heapLog = NULL;

/*void * operator new(size_t size) throw() {
	void * alloced = malloc(size);
	
	if (alloced) {
		//std::cout << "+" << size;
		
		timeval t;
		gettimeofday(&t, NULL);
				
		if (heapLog)
			fprintf(heapLog, "%i:%i alloc 0x%x 0x%x\n", t.tv_sec, t.tv_usec, alloced, size);
	}
	
	return alloced;
}

void operator delete(void * object) throw() {
	if (object) {
		timeval t;
		gettimeofday(&t, NULL);
		
		if (heapLog)
			fprintf(heapLog, "%i:%i free 0x%x\n", t.tv_sec, t.tv_usec, object);
		
		free(object);
	}
}*/
  

int main(int argc, char ** argv) {
	heapLog = fopen("heap.log", "w");
	fwrite("", 0, 0, heapLog);
	fclose(heapLog);
	
	heapLog = fopen("heap.log", "a");
	
	try {
		// initialize log system
		TGen::Engine::StandardLogs logs;
		std::ios_base::sync_with_stdio(false);
	
		try {
			return run(argc, argv, logs);
		}
		catch (const TGen::RuntimeException & e) {
			logs.error << e;
			logs.error << "unhandled exception, quitting..." << TGen::endl;
			TGen::Engine::Platform::DisplayExceptionWindow(e, "Unhandled Exception");
			preExit();

			return EXIT_FAILURE;
		}
		catch (const std::exception & e) {
			logs.error["unhandled"] << e.what() << TGen::endl;
			logs.error << "unhandled exception, quitting..." << TGen::endl;
			DisplayErrorWindow("Unhandled sc++l exception", e.what());
			preExit();
			
			return EXIT_FAILURE;
		}
	}
	catch (const std::exception & e) {
		std::cerr << "ERROR IN STRAPPER: " << e.what() << std::endl;
		std::cerr << "unhandled exception, quitting..." << std::endl;
		DisplayErrorWindow("Unhandled sc++l exception in strapper", e.what());
		preExit();
		
		return EXIT_FAILURE;
	}
	catch (...) {
		std::cerr << "UNKNOWN ERROR IN STRAPPER!" << std::endl;
		std::cerr << "unhandled exception, quitting..." << std::endl;
		DisplayErrorWindow("Unhandled unknown exception in strapper", "");
		
		preExit();
		return EXIT_FAILURE;
	}
}

void preExit() {
	#ifdef _PLATFORM_WINDOWS
	system("pause");
	#endif
}

void createVariables(TGen::Engine::VariableRegister & variables) {
	variables += TGen::Engine::Variable("env_width", "512", "512", TGen::Engine::VariableConfigWriteOnly | TGen::Engine::VariableDump);
	variables += TGen::Engine::Variable("env_height", "512", "512", TGen::Engine::VariableConfigWriteOnly | TGen::Engine::VariableDump);
	variables += TGen::Engine::Variable("env_fullscreen", "false", "false", TGen::Engine::VariableConfigWriteOnly | TGen::Engine::VariableDump);
	variables += TGen::Engine::Variable("filesystem_game", "cleanmod", "testmod", TGen::Engine::VariableConfigWriteOnly);
	variables += TGen::Engine::Variable("game_name", "TGen Engine", "TGen Engine", TGen::Engine::VariableConfigWriteOnly);
	variables += TGen::Engine::Variable("game_author", "Peter Backman", "Peter Backman", TGen::Engine::VariableConfigWriteOnly);
	variables += TGen::Engine::Variable("version", TGen::Engine::getVersionString(), TGen::Engine::getVersionString(), TGen::Engine::VariableReadOnly);
	variables += TGen::Engine::Variable("map", "", "", 0);
	variables += TGen::Engine::Variable("r_renderer", "forward", "forward", TGen::Engine::VariableConfigWriteOnly);
	variables += TGen::Engine::Variable("r_syncVtrace", "false", "false", 0);
	
	//variables += TGen::Engine::Variable("r_postprocessor", "default", "default", 0);	
}

void createCommands(TGen::Engine::CommandRegister & commands, TGen::Engine::VariableRegister & variables) {
	commands.addCommand(new TGen::Engine::Command("set", new TGen::Engine::CommandSet(variables)));
	commands.addCommand(new TGen::Engine::Command("dumpvars", new TGen::Engine::CommandDumpVars(variables)));	
}

TGen::PropertyTree readGameInfoFile(TGen::Engine::Filesystem & filesystem) {
	std::auto_ptr<TGen::Engine::File> infoFile(filesystem.openRead("info"));
	
	TGen::PropertyTreeParser propParser;
	
	return propParser.parse(infoFile->readAll().c_str());
}

void addGameDependencies(TGen::PropertyTree & deps, TGen::Engine::Filesystem & filesystem) {
	TGen::PropertyTree::PropertyMap::const_iterator depIter = deps.getProperties().begin();
	
	for (; depIter != deps.getProperties().end(); ++depIter)
		filesystem.addSearchPath(depIter->second, false);	
}

void executeAutos(TGen::PropertyTree & autos, TGen::Engine::CommandRegister & commands, TGen::Engine::Filesystem & filesystem, TGen::Engine::StandardLogs & logs) {
	std::vector<std::string> autoexecs;
	autoexecs.push_back("vardump");
	
	TGen::PropertyTree::PropertyMap::const_iterator iter = autos.getProperties().begin();
	
	for (; iter != autos.getProperties().end(); ++iter) {
		if (std::find(autoexecs.begin(), autoexecs.end(), iter->second) == autoexecs.end())
			autoexecs.push_back(iter->second);
	}
	
	for (int i = 0; i < autoexecs.size(); ++i) {
		try {
			logs.info["run"];
			commands.executeFile(autoexecs[i], filesystem, logs.info);
		}
		catch (std::exception & e) {
			logs.warning["run"] << e.what() << TGen::endl;
		}
	}
}

int run(int argc, char ** argv, TGen::Engine::StandardLogs & logs) {
	uint8 debuglibs = (TGen::isCoreDebug() & 1) | (TGen::isMathDebug() & 1) << 1 | (TGen::isGraphicsDebug() & 1) << 2 
	| (TGen::isOpenGLDebug() & 1) << 3 | (TGen::isRendererDebug() & 1) << 4 | (TGen::isEngineDebug() & 1) << 5;
	
	
	logs.info["run"] << "TGen Engine (debug binary: " << std::boolalpha << TGen::isEngineDebug() << ")" << TGen::endl;
	logs.info["run"] << "   debug flags: 0x" << std::hex << std::setw(2) << std::setfill('0') << int(debuglibs) << TGen::endl;
	logs.info["run"] << "   version: " << TGen::Engine::getVersionString() << TGen::endl;
	
	logs.info << TGen::separator("initializing");
	

	TGen::Engine::VariableRegister variables;
	TGen::Engine::CommandRegister commands;
	
	createVariables(variables);
	createCommands(commands, variables);
	
	
	// this is just a test
	try {
		std::vector<std::string> params;
		params.push_back("r_maxRefresh");
		params.push_back("666");
		commands["set"].execute(params, logs);
	}
	catch (TGen::Engine::CommandException & e) {
		logs.info << e.what() << TGen::endl;
	}
	
	
	// TODO: fonts
	// TODO: ConfigWriteOnly should only be writable until the game starts, until "running"
	
	
	
	// setup filesystem
	TGen::Engine::Filesystem * filesystem = new TGen::Engine::Filesystem(argv[0], logs);
	filesystem->addSearchPath(variables["filesystem_game"].getValue(), true);
	
	logs.info["run"] << "reading info file..." << TGen::endl;
	
	
	
	// read info file
	TGen::PropertyTree props = readGameInfoFile(*filesystem);
	
	variables["game_name"] = props["game/name"].second;
	variables["game_author"] = props["game/author"].second;
	
	
	executeAutos(props.getNode("game").getNode("autoexec"), commands, *filesystem, logs);
	addGameDependencies(props.getNode("game").getNode("deps"), *filesystem);    // TODO: pathsep
	
	
	TGen::Engine::DeviceCollection inputDevices;
	
	// TODO: filesystem.logDir, spara loggen där! sen nollställ loggfiler vid run
	
	// setup env
	TGen::Engine::Environment * env = new TGen::Engine::SDL(variables, props, logs);
	env->registerInputDevices(inputDevices);
	
	logs.info["run"];
	inputDevices.printDevices(logs.info);
	
	TGen::Engine::App * app = new TGen::Engine::App(variables, commands, *env, filesystem, props, env->getRenderer(), logs, inputDevices);
		
	// run forrest, run.....
	logs.info << TGen::separator("running");
	env->run(app);
	logs.info << TGen::separator("shutting down");
	
	//commands["dumpvars"].execute(params, logs);
	
	
	// shut down
	delete app;
	delete env;
	
	logs.info["run"] << "dumping variables..." << TGen::endl;
	variables.dumpVariables(*filesystem);
	
	delete filesystem;
	
	logs.info["run"] << "goodbye, have a nice day!" << TGen::endl;
	
	return EXIT_SUCCESS;	
}

