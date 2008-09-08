/*
 *  worldobject.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/30/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "worldobject.h"
#include "component.h"

TGen::Engine::Symbol TGen::Engine::WorldObject::symbolTranslateTo = TGen::Engine::getUniqueSymbol("translateTo:");

void TGen::Engine::WorldObject::trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode) {
	TGen::Engine::Symbol function = context.getFunctionSymbol();
	
	if (function == symbolTranslateTo) {
		TGen::Engine::Component * transportTo = context.getParameterPtr<TGen::Engine::Component *>(0);
		TGen::Engine::WorldObject * worldObject = dynamic_cast<TGen::Engine::WorldObject *>(transportTo);
		
		TGenAssert(transportTo);
		
		std::cout << ">TRANSPORT TO " << worldObject << " - " << std::string(worldObject->getPosition()) << std::endl;
		
		setPosition(worldObject->getPosition());
		setOrientation(worldObject->getOrientation());
	}
	else {
		context.invalidateCall();
	}
}
