/*
 *  font.cpp
 *  TGen Graphics
 *
 *  Created by Peter Backman on 10/9/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "font.h"
#include "color.h"
#include "fonttext.h"

TGen::Font::Font(TGen::Material * material)
	: material(material)
{
		
}

TGen::FontText * TGen::Font::createText(const std::string & text) {
	return new TGen::FontText(text, this, TGen::Color::Red, true);
}

TGen::Material * TGen::Font::getMaterial() const {
	return material;
}
