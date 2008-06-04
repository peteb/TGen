/*
 *  modelfactory.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/26/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "modelfactory.h"
#include "generateline.h"
#include "filesystem.h"
#include "file.h"
#include "modelgenerator.h"
#include "transformerfactory.h"

#include <tgen_renderer.h>
#include <tgen_core.h>

TGen::Engine::ModelFactory::ModelFactory(TGen::Engine::Filesystem & filesystem)
	: filesystem(filesystem)
{
}

TGen::NewModel * TGen::Engine::ModelFactory::createModel(const TGen::Engine::GenerateLine & line, TGen::VertexDataSource & dataSource) {
	std::auto_ptr<TGen::NewModel> newModel;
	
	TGen::VertexTransformList transformers;
	TGen::Engine::TransformerFactory transFactory;
	
	transformers.addTransformer(transFactory.createTransformers(line));
		
	if (line.getName().substr(0, 4) == "gen:") {		// a model generator
		TGen::Engine::ModelGenerator modelGenerator;
		newModel.reset(modelGenerator.createModel(line, dataSource, transformers));		
	}
	else {	// a file
		std::string filename = line.getName();
		std::auto_ptr<TGen::Engine::File> file(filesystem.openRead(filename));
		
		if (filename.find(".md3") != std::string::npos) {
			TGen::MD3::Parser modelParser;
			std::auto_ptr<TGen::MD3::File> md3file(modelParser.parse(*file));
			//md3file->printInfo(std::cout);
			newModel.reset(md3file->createModel(dataSource, transformers));
		}	
	}
	
	if (!newModel.get())
		throw TGen::RuntimeException("ModelFactory::createModel", "model failed to load: " + line.getName());
	
	return newModel.release();
}
