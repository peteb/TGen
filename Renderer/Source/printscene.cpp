/*
 *  printscene.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/13/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include <tgen_core.h>
#include "printscene.h"
#include "face.h"
#include "geometry.h"
#include "modelinstance_new.h"
#include "meshinstance_new.h"

TGen::ScenePrinter::ScenePrinter(std::ostream & stream) 
	: stream(stream)
	, level(0)
{
	
}

bool TGen::ScenePrinter::pre(TGen::SceneNode & node) const {
	std::string levelName;
	
	for (int i = 0; i < level; ++i)
		levelName += "   ";
	
	stream << levelName << (node.hasChanged() ? ">" : "-") << node.getName();
	stream << " pos: [" << std::string(node.getLocalPosition()) << ", " << std::string(node.getWorldPosition()) << "]";	
	//stream << " orient: [" << std::string(node.getLocalOrientation()) << ", " << std::string(node.getWorldOrientation()) << "]";
	stream << std::endl;
	
	/*for (int i = 0; i < node.getFaces().size(); ++i) {
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
	}*/
	
	for (int i = 0; i < node.getModels().size(); ++i) {
		TGen::NewModelInstance * model = DerefRes(node.getModels()[i]);
		stream << levelName << "   model " << i << " (" << model->getName() << ")" << std::endl;
		stream << levelName << "      meshes " << model->getNumMeshes() << ": " << std::endl;
		
		for (int a = 0; a < model->getNumMeshes(); ++a) {
			TGen::NewMeshInstance * mesh = model->getMesh(a);
			std::string materialName = node.getModels()[i]->getOverridingMaterialName();
			TGen::Material * material = node.getModels()[i]->getOverridingMaterial();
			
			if (materialName.empty())
				materialName = mesh->getMaterialName();
			
			if (!material)
				material = mesh->getMaterial();
			
			stream << levelName << "         " << a << " material";
			
			if (!node.getModels()[i]->getOverridingMaterialName().empty())
				stream << " (overriden by model)";
			
			stream << ": " << materialName << " (linked: " << material << ")" << std::endl;
		}
	}
	
	level++;
	
	return true;
}

void TGen::ScenePrinter::post(TGen::SceneNode & node) const {
	level--;
	
}
