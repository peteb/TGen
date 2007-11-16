/*
 *  devilimage.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 11/16/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_DEVILIMAGE_H
#define _TGEN_ENGINE_DEVILIMAGE_H

#include <tgen_graphics.h>
#include <IL/IL.h>

namespace TGen {
	namespace Engine {
		class DevilImage : public TGen::Image {
		public:
			DevilImage(ILuint imageName);
			
			TGen::Rectangle getSize() const;
			TGen::ImageFormat getFormat() const;
			TGen::FormatType getComponentFormat() const;
			const void * getData() const;
			
		private:
			ILuint imageName;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_DEVILIMAGE_H
