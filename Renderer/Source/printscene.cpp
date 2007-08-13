/*
 *  printscene.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/13/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "printscene.h"

TGen::ScenePrinter::ScenePrinter(std::ostream & stream) 
	: stream(stream)
	, level(0)
{
	
}

bool TGen::ScenePrinter::pre(TGen::SceneNode & node) const {
	for (int i = 0; i < level; ++i)
		stream << "   ";
	
	stream << node.getName() << std::endl;
	
	level++;
	
	return true;
}

void TGen::ScenePrinter::post(TGen::SceneNode & node) const {
	level--;
	
}
