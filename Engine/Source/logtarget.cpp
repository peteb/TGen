/*
 *  logtarget.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 10/8/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "logtarget.h"


TGen::Engine::StreamOutput::StreamOutput(std::ostream & stream)
	: stream(stream)
{
}

void TGen::Engine::StreamOutput::output(LogMessageType type, const std::string & title, const std::string & text) {
	std::string name;
	switch (type) {
		case TGen::Engine::Warning:
			name = "**WARNING**";
			break;
			
		case TGen::Engine::Error:
			name = "**ERROR**";
			break;
	}
	
	if (!name.empty())
		stream << name << " ";
	
	if (!title.empty())
		stream << "[" << title << "]: ";
	
	stream << text << std::endl;
}


TGen::Engine::CombinedLogTargets::CombinedLogTargets() {
	
}

TGen::Engine::CombinedLogTargets::~CombinedLogTargets() {
	for (int i = 0; i < targets.size(); ++i)
		delete targets[i];
}

void TGen::Engine::CombinedLogTargets::addTarget(TGen::Engine::LogTarget * target) {
	targets.push_back(target);
}

void TGen::Engine::CombinedLogTargets::output(LogMessageType type, const std::string & title, const std::string & text) {
	for (int i = 0; i < targets.size(); ++i)
		targets[i]->output(type, title, text);
}

