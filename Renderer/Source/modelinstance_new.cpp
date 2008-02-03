/*
 *  modelinstance_new.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 2/3/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "modelinstance_new.h"
#include "meshinstance_new.h"
#include "renderlist.h"

TGen::NewModelInstance::NewModelInstance(const std::string & name)
	: name(name)
{
}

TGen::NewModelInstance::~NewModelInstance() {

}

std::string TGen::NewModelInstance::getName() const {
	return name;
}

void TGen::NewModelInstance::update() {

}

TGen::ModelJoint TGen::NewModelInstance::getJoint(const std::string & name) const {
	throw TGen::RuntimeException("ModelInstance::getJoint", "no implementation! Did you dereference links?");
}
