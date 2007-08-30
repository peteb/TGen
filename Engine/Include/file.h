/*
 *  file.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/27/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_FILE_H
#define _TGEN_ENGINE_FILE_H

#include <iostream>
#include "physfs.h"

namespace TGen {
	namespace Engine {
		class File {
			File(PHYSFS_File * file);
			
		public:
			~File();

			void close();
			std::string readAll();
			
			friend class Filesystem;
			
		private:
			PHYSFS_File * file;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_FILE_H

