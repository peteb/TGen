/*
 *  vbrenderable.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 11/28/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_VBRENDERABLE_H
#define _TGEN_ENGINE_VBRENDERABLE_H

#include <tgen_graphics.h>

namespace TGen {
	
	namespace Engine {
		class VBRenderable : public TGen::Renderable {
		public:
			VBRenderable(TGen::VertexData * vb, TGen::PrimitiveType type, uint size);
			
			void preRender(TGen::Renderer & renderer) const;
			void render(TGen::Renderer & renderer) const;
			
		private:
			TGen::PrimitiveType type;
			TGen::VertexData * vb;
			uint size;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_VBRENDERABLE_H
