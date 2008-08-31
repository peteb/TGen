/*
 *  timer_script.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/27/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "utilities/timer_script.h"
#include "utilities/timer.h"

#define TIMER TGen::Engine::Utilities::TimerScript

TGen::Engine::Symbol TIMER::symbolReset = TGen::Engine::getUniqueSymbol("reset");
TGen::Engine::Symbol TIMER::symbolSetInterval = TGen::Engine::getUniqueSymbol("setInterval:");
TGen::Engine::Symbol TIMER::symbolGetInterval = TGen::Engine::getUniqueSymbol("getInterval:");
TGen::Engine::Symbol TIMER::symbolGetElapsedTime = TGen::Engine::getUniqueSymbol("getElapsedTime");


TGen::Engine::Utilities::TimerScript::TimerScript(Timer & timer)
	: timer(timer)
{
}

bool TGen::Engine::Utilities::TimerScript::trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode) {
	TGen::Engine::Symbol function = context.getFunctionSymbol();
	
	if (function == symbolReset) {
		timer.reset();
	}
	else if (function == symbolSetInterval) {
		timer.setInterval(context.getParameter<scalar>(0));
	}
	else if (function == symbolGetInterval) {
		context.setReturn<scalar>(timer.getInterval());
	}
	else if (function == symbolGetElapsedTime) {
		context.setReturn<scalar>(timer.getElapsedTime());
	}
	else {
		return false;
	}
	
	return true;
}

// TODO: flytta objectRef till utilities. kanske inte går? prototypeRef iaf! jo borde ju gå, om man skickar &entityList till utils-subsystem

void TGen::Engine::Utilities::TimerScript::link(const TGen::Engine::ComponentLinker & linker) {
	eventTick.link(linker);
}

void TGen::Engine::Utilities::TimerScript::setEventTick(const std::string & eventName) {
	eventTick.set(eventName);
}

void TGen::Engine::Utilities::TimerScript::tick() {
	TGen::Engine::TriggerContext * context = eventTick->context;
	TGenAssert(context);
	
	eventTick->trigger(*context, TGen::Engine::TriggerPrecise);		
}
