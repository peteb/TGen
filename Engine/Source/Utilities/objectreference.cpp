/*
 *  objectreference.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/25/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "utilities/objectreference.h"

TGen::Engine::Utilities::ObjectReference::ObjectReference(const std::string & name)
	: TGen::Engine::ResourceComponent(name)
	, TGen::Engine::ComponentRecipe(name)
{
}

void * TGen::Engine::Utilities::ObjectReference::getData() const {
	return reinterpret_cast<void *>(reference.get());
}



void TGen::Engine::Utilities::ObjectReference::prelink(const TGen::Engine::ComponentLinker & linker) {
	reference.prelink(linker);		
}

void TGen::Engine::Utilities::ObjectReference::link(const TGen::Engine::ComponentLinker & linker) {
	reference.link(linker);
}


void TGen::Engine::Utilities::ObjectReference::setObjectName(const std::string & name) {
	reference.set(name);
}


TGen::Engine::Component * TGen::Engine::Utilities::ObjectReference::createComponent(const TGen::Engine::EntityRecipe & entity, TGen::Engine::Entity & constructing) {
	return this;
}

