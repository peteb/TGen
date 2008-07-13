/*
 *  event.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/12/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "script/event.h"

TGen::Engine::Script::Event::Event(const std::string & name)
	: TGen::Engine::Component(name)
{

}


void TGen::Engine::Script::Event::linkGlobally(TGen::Engine::EntityList & entities, TGen::Engine::Entity & entity) {
	TGen::Engine::Script::EventOperation::linkGlobally(entities, entity);
}

