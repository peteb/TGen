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
			
			TGen::Image * load(TGen::Engine::File * file, const std::string & ext);
			
			static ILHANDLE ILAPIENTRY Open(const ILstring file);
			static void ILAPIENTRY Close(ILHANDLE file);
			static ILboolean ILAPIENTRY Eof(ILHANDLE file);
			static ILint ILAPIENTRY Getc(ILHANDLE file);
			static ILint ILAPIENTRY Read(void * data, ILuint objectSize, ILuint objectCount, ILHANDLE file);
			static ILint ILAPIENTRY Seek(ILHANDLE file, ILint bytes, ILint mode);
			static ILint ILAPIENTRY Tell(ILHANDLE file);	

		private:
			TGen::Image * loadDevIL(TGen::Engine::File * file, const std::string & ext);
			
			#ifdef _PLATFORM_WINDOWS
			TGen::Image * loadSDLImage(TGen::Engine::File * file, const std::string & ext);
			#endif
		};
		
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_IMAGELOADER_H
