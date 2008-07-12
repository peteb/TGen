/*
 *  eventoperation.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/12/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "eventoperation.h"

TGen::Engine::Script::EventOperation::~EventOperation() {
	for (int i = 0; i < operations.size(); ++i)
		delete operations[i];
	
}

void TGen::Engine::Script::EventOperation::addOperation(EventOperation * operation) {
	operations.push_back(operation);
}

void TGen::Engine::Script::EventOperation::trigger(void ** argv, int argc) {
	for (int i = 0; i < operations.size(); ++i)
		operations[i]->trigger(argv, argc);
}

void TGen::Engine::Script::EventOperation::linkGlobally(TGen::Engine::EntityList & entities, TGen::Engine::Entity & entity) {
	for (int i = 0; i < operations.size(); ++i)
		operations[i]->linkGlobally(entities, entity);
}
