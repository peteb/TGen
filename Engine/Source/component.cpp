/*
 *  component.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/18/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "component.h"
#include "entity.h"
#include "componentlinker.h"

TGen::Engine::Symbol TGen::Engine::Component::symbolGetEntity = TGen::Engine::getUniqueSymbol("getEntity");
TGen::Engine::Symbol TGen::Engine::Component::symbolEnable = TGen::Engine::getUniqueSymbol("enable");
TGen::Engine::Symbol TGen::Engine::Component::symbolDisable = TGen::Engine::getUniqueSymbol("disable");

TGen::Engine::Component::Component(const std::string & name, bool staticComponent) 
	: name(name)
	, owner(NULL)
	, staticComponent(staticComponent)
{
	
}

TGen::Engine::Component::~Component() {
	
}

void TGen::Engine::Component::link(const TGen::Engine::ComponentLinker & linker) {
	if (linker.getEntity()) {
		context = &linker.getEntity()->getContext();	
		owner = linker.getEntity();
	}
}

const std::string & TGen::Engine::Component::getName() const {
	return name;
}

void TGen::Engine::Component::trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode) {
	TGen::Engine::Symbol function = context.getFunctionSymbol();
	
	if (function == symbolGetEntity) {
		std::cout << "PUT ENTITY " << owner << " IN REG " << context.getReturnRegister() << std::endl;
		context.setRegister<TGen::Engine::Entity *>(context.getReturnRegister(), owner);
	}
	else if (function == symbolEnable) {
		setEnabled(true);
	}
	else if (function == symbolDisable) {
		setEnabled(false);
	}
	else {
		context.invalidateCall();
	}
}

void TGen::Engine::Component::setOwner(TGen::Engine::Entity * owner) {
	this->owner = owner;
}

/*TGen::Engine::Entity * TGen::Engine::Component::getOwner() const {
	return owner;
}*/

bool TGen::Engine::Component::isStatic() const {
	return staticComponent;
}

