/*
 *  timer.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/24/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "timer.h"

#define TIMER TGen::Engine::Utilities::Timer

TGen::Engine::Symbol TIMER::symbolReset = TGen::Engine::getUniqueSymbol("reset");


TGen::Engine::Utilities::Timer::Timer(const std::string & name) 
	: TGen::Engine::Component(name)
	, interval(1.0)
	, totalTime(0.0)
	, accumTime(0.0)
{
	
}


void TGen::Engine::Utilities::Timer::link(const TGen::Engine::ComponentLinker & linker) {
	eventTick.link(linker);
}

void TGen::Engine::Utilities::Timer::trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode) {
	TGen::Engine::Symbol function = context.getFunctionSymbol();
	
	if (function == symbolReset) {
		reset();
	}
	else {
		TGen::Engine::Component::trigger(context, mode);
	}
}


void TGen::Engine::Utilities::Timer::setEnabled(bool enabled) {
	this->enabled = enabled;
}

void TGen::Engine::Utilities::Timer::setEventTick(const std::string & eventName) {
	eventTick.set(eventName);
}

void TGen::Engine::Utilities::Timer::setInterval(scalar interval) {
	this->interval = interval;
}

void TGen::Engine::Utilities::Timer::update(scalar dt) {
	if (!enabled)
		return;
	
	totalTime += dt;
	accumTime += dt;
	
	if (accumTime >= interval) {
		tick();	
		
		accumTime -= interval;
	}
}

void TGen::Engine::Utilities::Timer::tick() {
	TGen::Engine::TriggerContext * context = eventTick->context;
	TGenAssert(context);
	
	//context->setParameter(0, with);
	//context->numParameters = 1;
	
	eventTick->trigger(*context, TGen::Engine::TriggerPrecise);	
}

void TGen::Engine::Utilities::Timer::reset() {
	accumTime = 0.0;
}
