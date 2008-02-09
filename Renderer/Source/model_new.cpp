/*
 *  model_new.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 2/3/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "model_new.h"

TGen::NewModel::NewModel(const std::string & name, const std::string & materialName, const std::string & materialNamePostfix)
	: TGen::NewModelInstance(name, materialName, materialNamePostfix)
{
	
}

TGen::NewModel::~NewModel() {
	
}

