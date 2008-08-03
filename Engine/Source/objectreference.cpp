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

void TGen::Engine::ObjectReference::link(const TGen::Engine::ComponentLinker & linker) {
	std::cout << "LINK OBJECT REF" << std::endl;
	reference.link(linker);
}

void TGen::Engine::ObjectReference::setObjectName(const std::string & name) {
	reference.set(name);
}





void TGen::Engine::ObjectReference::link(const TGen::Engine::ComponentLinker & linker, TGen::Engine::EntityRecipe & prototype) {
	reference.link(linker);
	reference.linkComponentIndex(prototype);	
}

void TGen::Engine::ObjectReference::fastLinkConstructed(const TGen::Engine::ComponentLinker & linker, TGen::Engine::Component & constructed) {
	//reference.link(TGen::Engine::ComponentLinker(NULL, &entity));
}

TGen::Engine::Component * TGen::Engine::ObjectReference::createComponent(const TGen::Engine::EntityRecipe & entity, TGen::Engine::Entity & constructing) {
	return this;
}

