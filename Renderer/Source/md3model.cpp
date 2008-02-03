/*
 *  md3model.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/23/07.
 *  Renamed by Peter Backman on 2/3/08.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "md3model.h"
#include "model_new.h"

TGen::MD3::Model::Model(const std::string & name)
	: TGen::NewModel(name)
{
		
}

TGen::MD3::Model::~Model() {

}

bool TGen::MD3::Model::isPureInstance() const {
	return false;
}

TGen::MD3::Model * TGen::MD3::Model::instantiate() {
	// if static, return this, otherwise, return animatable instance
	return this;
}


// max/min/origin/radius i model
