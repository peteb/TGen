/*
 *  timer.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/24/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "timer.h"
#include "timer_script.h"

using TGen::scalar;

TGen::Engine::Utilities::Timer::Timer(const std::string & name) 
	: TGen::Engine::Component(name)
	, interval(1.0)
	, totalTime(0.0)
	, accumTime(0.0)
	, scriptInterface(NULL)
{
	
}

TGen::Engine::Utilities::Timer::~Timer() {
	delete scriptInterface;
}

void TGen::Engine::Utilities::Timer::link(const TGen::Engine::ComponentLinker & linker) {

}

void TGen::Engine::Utilities::Timer::setScriptInterface(TGen::Engine::Utilities::TimerScript * scriptInterface) {
	this->scriptInterface = scriptInterface;
}

void TGen::Engine::Utilities::Timer::setEnabled(bool enabled) {
	this->enabled = enabled;
}

void TGen::Engine::Utilities::Timer::setInterval(scalar interval) {
	this->interval = interval;
}

scalar TGen::Engine::Utilities::Timer::getInterval() const {
	return interval;
}

scalar TGen::Engine::Utilities::Timer::getElapsedTime() const {
	return totalTime;
}

void TGen::Engine::Utilities::Timer::reset() {
	accumTime = 0.0f;
	totalTime = 0.0f;
}

void TGen::Engine::Utilities::Timer::update(scalar dt) {
	if (!enabled)
		return;
	
	totalTime += dt;
	accumTime += dt;
	
	if (accumTime - offsetTime >= interval) {
		tick();	
		
		accumTime -= interval;
	}
}

void TGen::Engine::Utilities::Timer::setOffsetTime(scalar time) {
	offsetTime = time;
}

void TGen::Engine::Utilities::Timer::tick() {
	if (scriptInterface)
		scriptInterface->tick();
}

