/*
 *  eventoperation.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/12/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "eventoperation.h"
#include "entity.h"

TGen::Engine::Script::EventOperation::~EventOperation() {
	for (int i = 0; i < operations.size(); ++i)
		delete operations[i];
	
}

void TGen::Engine::Script::EventOperation::addOperation(EventOperation * operation) {
	operations.push_back(operation);
}

void TGen::Engine::Script::EventOperation::trigger(TGen::Engine::TriggerContext & context) {
	for (int i = 0; i < operations.size(); ++i)
		operations[i]->trigger(context);
}

void TGen::Engine::Script::EventOperation::linkGlobally(TGen::Engine::EntityList & entities, TGen::Engine::Entity & entity) {
	context = &entity.getContext();

	for (int i = 0; i < operations.size(); ++i)
		operations[i]->linkGlobally(entities, entity);
}
