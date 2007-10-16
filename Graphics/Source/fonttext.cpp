/*
 *  fonttext.cpp
 *  TGen Graphics
 *
 *  Created by Peter Backman on 10/9/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "fonttext.h"

TGen::FontText::FontText(const std::string & text, const TGen::Color & color, bool dynamic)
	: text(text)
	, color(color)
	, dynamic(dynamic) 
{
		
}

void TGen::FontText::render(TGen::Renderer & renderer, const TGen::Vector3 & position) {
	
}

