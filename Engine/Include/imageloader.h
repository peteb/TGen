/*
 *  imageloader.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 11/16/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_IMAGELOADER_H
#define _TGEN_ENGINE_IMAGELOADER_H

#include <IL/IL.h>
#include <tgen_graphics.h>

namespace TGen {
	namespace Engine {
		class File;
		
		class ImageLoader {
		public:
			ImageLoader();
			~ImageLoader();
			
			TGen::Image * load(TGen::Engine::File * file);
			
			static ILHANDLE Open(const char * file);
			static void Close(ILHANDLE file);
			static ILboolean Eof(ILHANDLE file);
			static ILint Getc(ILHANDLE file);
			static ILint Read(void * data, ILuint objectSize, ILuint objectCount, ILHANDLE file);
			static ILint Seek(ILHANDLE file, ILint bytes, ILint mode);
			static ILint Tell(ILHANDLE file);		
		};
		
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_IMAGELOADER_H
