/*
 *  printscene.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/13/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "printscene.h"
#include "face.h"
#include "geometry.h"

TGen::ScenePrinter::ScenePrinter(std::ostream & stream) 
	: stream(stream)
	, level(0)
{
	
}

bool TGen::ScenePrinter::pre(TGen::SceneNode & node) const {
	std::string levelName;
	
	for (int i = 0; i < level; ++i)
		levelName += "   ";
	
	stream << levelName << (node.hasChanged() ? "!" : "") << node.getName();
	stream << " [" << std::string(node.getLocalPosition()) << ", " << std::string(node.getWorldPosition()) << "]";	
	stream << " [" << std::string(TGen::Vector3(node.getLocalOrientation())) << ", " << std::string(TGen::Vector3(node.getWorldOrientation())) << "]";
	stream << std::endl;
	
	for (int i = 0; i < node.getFaces().size(); ++i) {
		stream << levelName << "   face " << i << " ";
		stream << "[" << (node.getFaces()[i].getMaterial() ? "" : "!") << node.getFaces()[i].getMaterialName() << ", ";
		
		if (!node.getFaces()[i].getGeometry()) {
			stream << "no geometry]";
		}
		else {
			if (node.getFaces()[i].getGeometry()->getLeaves())
				stream << node.getFaces()[i].getGeometry()->getLeaves()->size() << " submeshes]";
			else
				stream << "no submeshes]";
		}
		
		stream << std::endl;
	}
	
	level++;
	
	return true;
}

void TGen::ScenePrinter::post(TGen::SceneNode & node) const {
	level--;
	
}
