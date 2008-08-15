/*
 *  objectreference.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/25/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "objectreference.h"

TGen::Engine::ObjectReference::ObjectReference(const std::string & name)
	: TGen::Engine::ResourceComponent(name)
	, TGen::Engine::ComponentRecipe(name)
{
}

void * TGen::Engine::ObjectReference::getData() const {
	return reinterpret_cast<void *>(reference.get());
}



void TGen::Engine::ObjectReference::prelink(const TGen::Engine::ComponentLinker & linker) {
	reference.prelink(linker);		
}

void TGen::Engine::ObjectReference::link(const TGen::Engine::ComponentLinker & linker) {
	reference.link(linker);

//	if (linker.getComponent())
	//	linker.getComponent()->setOwner(reinterpret_cast<TGen::Engine::Entity *>(0xBEEF));
}



void TGen::Engine::ObjectReference::setObjectName(const std::string & name) {
	reference.set(name);
}




TGen::Engine::Component * TGen::Engine::ObjectReference::createComponent(const TGen::Engine::EntityRecipe & entity, TGen::Engine::Entity & constructing) {
	return this;
}

