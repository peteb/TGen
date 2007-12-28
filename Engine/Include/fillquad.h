/*
 *  fillquad.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 11/11/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_FILLQUAD_H
#define _TGEN_ENGINE_FILLQUAD_H

#include <tgen_renderer.h>
#include <tgen_graphics.h>

namespace TGen {
	namespace Engine {
		class FillQuad : public TGen::Mesh {
		public:
			FillQuad(const TGen::Vector2 & min, const TGen::Vector2 & max, TGen::VertexDataSource & dataSource);
			~FillQuad();
			
			void preRender(TGen::Renderer & renderer) const;
			void render(TGen::Renderer & renderer) const;
			
			TGen::Vector3 getMax() const;
			TGen::Vector3 getMin() const;
			TGen::Vector3 getOrigin() const;

			typedef TGen::JoinVertexElements2<TGen::Vertex2<float>, TGen::TexCoord2<float, 0> > VertexDecl;
			
		private:
			TGen::VertexData * vb;
			TGen::Vector2 min, max;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_FILLQUAD_H
