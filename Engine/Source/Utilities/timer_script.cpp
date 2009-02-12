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


TGen::Engine::Utilities::TimerScript::TimerScript(Timer & timer)
	: timer(timer)
{
}


// TODO: flytta objectRef till utilities. kanske inte går? prototypeRef iaf! jo borde ju gå, om man skickar &entityList till utils-subsystem

void TGen::Engine::Utilities::TimerScript::link(const TGen::Engine::ComponentLinker & linker) {

}

void TGen::Engine::Utilities::TimerScript::tick() {

}
