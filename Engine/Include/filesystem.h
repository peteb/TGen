/*
 *  filesystem.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/27/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_FILESYSTEM_H
#define _TGEN_ENGINE_FILESYSTEM_H

#include <vector>

namespace TGen {
	namespace Engine {
		class File;
		class StandardLogs;
		
		class Filesystem {
		public:
			Filesystem(const char * argv0, TGen::Engine::StandardLogs & logs);
			~Filesystem();

			void addSearchPath(const std::string & path, bool override);
			
			TGen::Engine::File * openRead(const std::string & path);
			TGen::Engine::File * openWrite(const std::string & path);
			TGen::Engine::File * openAppend(const std::string & path);
			
			bool exists(const std::string & path);
			//void outputPath(const std::string & path);
			void enumerateFiles(const std::string & path, std::vector<std::string> & output, bool recurse = false);
			std::string getRealPath(const std::string & dir);
			
		private:
			TGen::Engine::StandardLogs & logs;
			std::string base;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_FILESYSTEM_H

