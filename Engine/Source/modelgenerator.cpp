/*
 *  modelgenerator.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/26/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "modelgenerator.h"
#include "generateline.h"
#include "cubemesh.h"
#include "cubemodel.h"
#include <tgen_core.h>
#include <tgen_math.h>

TGen::NewModel * TGen::Engine::ModelGenerator::createModel(const TGen::Engine::GenerateLine & line, TGen::VertexDataSource & dataSource, const TGen::VertexTransformer & transformer) {
	std::string genName = line.getName().substr(4);
	
	TGen::NewModel * newModel = NULL;
	
	if (genName == "cube" || genName == "box") {
		scalar width = TGen::lexical_cast<scalar>(line.getParameter("width", "1.0"));
		scalar height = TGen::lexical_cast<scalar>(line.getParameter("height", "1.0"));
		scalar depth = TGen::lexical_cast<scalar>(line.getParameter("depth", "1.0"));
		
		TGen::Vector3 min(-width / 2.0, -height / 2.0, -depth / 2.0);
		TGen::Vector3 max(width / 2.0, height / 2.0, depth / 2.0);
		TGen::Vector3 texMax;
		
		texMax.x = TGen::lexical_cast<scalar>(line.getParameter("texwidth", "1.0"));
		texMax.y = TGen::lexical_cast<scalar>(line.getParameter("texheight", "1.0"));
		texMax.z = TGen::lexical_cast<scalar>(line.getParameter("texdepth", "1.0"));
		
		TGen::Engine::CubeMesh * mesh = new TGen::Engine::CubeMesh(line.getParameter("material", "cubemat"), "", min, max, TGen::Vector3::Zero, texMax);
		
		mesh->createVertexData(dataSource);
		// fill mesh with data. iofs CubeModel och CubeMesh kan gott och väl vara mer generic...
		
		
		TGen::Engine::CubeModel * model = new TGen::Engine::CubeModel(line.getStringLine(), line.getParameter("material", "cubemat"), "", min, max, dataSource);
		model->addMesh(mesh);
		
		newModel = model;
	}
	else {
		throw TGen::RuntimeException("ModelGenerator", "invalid gentype: '" + genName + "'");
	}
	
	return newModel;
}
