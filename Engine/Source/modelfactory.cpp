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
#include <tgen_renderer.h>
#include <tgen_core.h>

TGen::Engine::ModelFactory::ModelFactory(TGen::Engine::Filesystem & filesystem)
	: filesystem(filesystem)
{
}

TGen::NewModel * TGen::Engine::ModelFactory::createModel(const TGen::Engine::GenerateLine & line, TGen::VertexDataSource & dataSource) {
	std::auto_ptr<TGen::NewModel> newModel(NULL);
	
	TGen::VertexTransformList transformers;
	
	// Parse transformers from gline:
	for (TGen::Engine::GenerateLine::ParameterMap::const_iterator iter = line.getParameters().begin();
		  iter != line.getParameters().end(); ++iter) {
		
		if (iter->first == "scale") {
			transformers.addTransformer(new TGen::VertexScaler(TGen::lexical_cast<scalar>(iter->second)));
		}
		else if (iter->first == "preset") {
			if (iter->second == "q3") {
				transformers.addTransformer(new TGen::VertexSwapper(TGen::VertexSwapper::Y_AXIS, TGen::VertexSwapper::Z_AXIS));
			}
			else {
				throw TGen::RuntimeException("ModelFactory::createModel", "invalid component transform preset: '" + iter->second + "'");
			}
		}
	}
	
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
