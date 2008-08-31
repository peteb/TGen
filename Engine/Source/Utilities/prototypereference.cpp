/*
 *  prototypereference.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/30/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "utilities/prototypereference.h"
#include "componentlinker.h"
#include "entitylist.h"

TGen::Engine::Utilities::PrototypeReference::PrototypeReference(const std::string & name) 
	: TGen::Engine::ResourceComponent(name)
	, TGen::Engine::ComponentRecipe(name)
	, prototype(NULL)
{
	
}

void * TGen::Engine::Utilities::PrototypeReference::getData() const {
	return reinterpret_cast<void *>(prototype);
}

void TGen::Engine::Utilities::PrototypeReference::setPrototypeName(const std::string & name) {
	this->prototypeName = name;
}

void TGen::Engine::Utilities::PrototypeReference::link(const TGen::Engine::ComponentLinker & linker) {
	if (linker.getEntityList())
		prototype = linker.getEntityList()->getPrototype(prototypeName, std::nothrow);
}

void TGen::Engine::Utilities::PrototypeReference::prelink(const TGen::Engine::ComponentLinker & linker) {
	link(linker);
}

TGen::Engine::Component * TGen::Engine::Utilities::PrototypeReference::createComponent(const TGen::Engine::EntityRecipe & entity, TGen::Engine::Entity & constructing) {
	return this;
}

