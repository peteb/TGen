/*
 *  fonttext.h
 *  TGen Graphics
 *
 *  Created by Peter Backman on 10/9/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_GRAPHICS_FONTTEXT_H
#define _TGEN_GRAPHICS_FONTTEXT_H

#include "color.h"
#include "vertexstructure.h"
#include "renderable.h"
#include <tgen_core.h>

namespace TGen {
	class Renderer;
	class Font;
	class VertexData;
	
	enum FontTextFormat {
		// color osv?
	};
	
	class FontText : public TGen::Renderable {
	public:
		FontText(const std::string & text, Font * font, const TGen::Color & color, bool dynamic);
		~FontText();
		
		void setPosition(const TGen::Vector2 & pos);
		void update(TGen::Renderer & renderer);
		void preRender(TGen::Renderer & renderer) const;
		void render(TGen::Renderer & renderer) const;

	private:
		typedef TGen::JoinVertexElements3<TGen::Vertex2<float>, Color4<float>, TexCoord2<float, 0> > VertexDecl;

		std::string text;
		TGen::Vector2 position, charSize, charSpacing;
		TGen::Color color, shadowColor;
		FontTextFormat format;
		VertexData * vb;
		Font * font;
		bool shadow, dynamic, needUpdate;
		scalar size;
	};
	
	
} // !TGen

#endif // !_TGEN_GRAPHICS_FONTTEXT_H
