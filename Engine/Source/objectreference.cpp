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
{
}

void * TGen::Engine::ObjectReference::getData() const {
	return reinterpret_cast<void *>(reference.get());
}

void TGen::Engine::ObjectReference::linkGlobally(TGen::Engine::EntityList & entities, TGen::Engine::Entity & entity) {
	reference.link(entities, entity);
}

void TGen::Engine::ObjectReference::setObjectName(const std::string & name) {
	reference.set(name);
}

