/*
 *  fonttext.cpp
 *  TGen Graphics
 *
 *  Created by Peter Backman on 10/9/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "fonttext.h"
#include "renderer.h"

TGen::FontText::FontText(const std::string & text, TGen::Font * font, const TGen::Color & color, bool dynamic)
	: text(text)
	, font(font)
	, color(color)
	, dynamic(dynamic) 
	, needUpdate(true)
	, vb(NULL)
{
		
}

TGen::FontText::~FontText() {
	delete vb;
}

void TGen::FontText::preRender(TGen::Renderer & renderer) const {
	renderer.setVertexBuffer(vb);	
}

void TGen::FontText::render(TGen::Renderer & renderer) const {
	renderer.drawPrimitive(TGen::PrimitiveQuads, 0, text.size() * 4);	// TODO: spara undan hur mycket som ska rendreras, kan ju ändras...
}

void TGen::FontText::update(TGen::Renderer & renderer) {
	if (needUpdate) {
		std::cout << "updating: " << text << std::endl;
		
		uint verticeCount = text.size() * 4;
		
		VertexDecl::Type * vertices = new VertexDecl::Type[verticeCount];
		
		TGen::Vector2 pos = position;
		
		charSize = TGen::Vector2::Identity * 32.0f;		// borde sättas när man initierar
		charSpacing = charSize * 0.6;
		
		TGen::Vector2 texSize = TGen::Vector2(1.0f / 16.0f, 1.0f / 16.0f);
		
		for (int i = 0; i < text.size(); ++i) {
			TGen::Vector2 texOffset;
			texOffset.x = text[i] % 16;
			texOffset.y = int(text[i] / 16);
			texOffset *= texSize;
			
			TGen::Color color = TGen::Color::Red;
			
			VertexDecl::Type * vertex = &vertices[i * 4];
			vertex[0].x = pos.x;
			vertex[0].y = pos.y;
			vertex[0].u = texOffset.x;
			vertex[0].v = texOffset.y;
			
			vertex[1].x = pos.x + charSize.x;
			vertex[1].y = pos.y;
			vertex[1].u = texOffset.x + texSize.x;
			vertex[1].v = texOffset.y;

			vertex[2].x = pos.x + charSize.x;
			vertex[2].y = pos.y + charSize.y;
			vertex[2].u = texOffset.x + texSize.x;
			vertex[2].v = texOffset.y + texSize.y;

			vertex[3].x = pos.x;
			vertex[3].y = pos.y + charSize.y;
			vertex[3].u = texOffset.x;
			vertex[3].v = texOffset.y + texSize.y;
			
			for (int i = 0; i < 4; ++i) {
				vertex[i].r = color.r;
				vertex[i].g = color.g;
				vertex[i].b = color.b;
				vertex[i].a = color.a;
			}
			
			pos += TGen::Vector2(charSpacing.x, 0.0f);
		}
		
		if (vb)
			delete vb;
		
		vb = renderer.createVertexBuffer(VertexDecl(), sizeof(VertexDecl::Type) * verticeCount, TGen::UsageDynamic);
		vb->bufferData(vertices, sizeof(VertexDecl::Type) * verticeCount, 0);
		delete vertices;
		
		needUpdate = false;
	}
}

void TGen::FontText::setPosition(const TGen::Vector2 & pos) {
	position = pos;
	needUpdate = true;
}

