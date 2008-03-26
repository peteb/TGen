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
#include <tgen_core.h>

TGen::NewModel * TGen::Engine::ModelGenerator::createModel(const TGen::Engine::GenerateLine & line, TGen::VertexDataSource & dataSource, const TGen::VertexTransformer & transformer)
{
	std::string genName = line.getName().substr(4);
	
	throw TGen::RuntimeException("ModelGenerator", "invalid gentype: '" + genName + "'");
}
