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

namespace TGen {
	namespace Engine {
		class File;
		
		class Filesystem {
		public:
			Filesystem(const char * argv0);
			~Filesystem();
			
			TGen::Engine::File * openRead(const std::string & path);
			TGen::Engine::File * openWrite(const std::string & path);
			TGen::Engine::File * openAppend(const std::string & path);

			void addSearchPath(const std::string & path, bool override);
			bool exists(const std::string & path);
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_FILESYSTEM_H

