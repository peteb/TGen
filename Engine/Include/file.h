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
#include <tgen_core.h>
#include "physfs.h"

namespace TGen {
	namespace Engine {
		class File : public TGen::InputStream {
			File(PHYSFS_File * file);
			
		public:
			~File();

			void close();
			std::string readAll();
			int64 write(const void * buffer, uint32 size, uint32 count);
			
			uint getReadPos();
			void seekReadPos(uint pos, TGen::Seek rel);
			void read(char * data, uint size);
			uint getSize();
			
			friend class Filesystem;
			
		private:
			PHYSFS_File * file;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_FILE_H

