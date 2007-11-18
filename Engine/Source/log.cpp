/*
 *  log.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 10/8/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "log.h"
#include "logtarget.h"
#include <iostream>

namespace TGen {
	TGen::Engine::Endl endl;
}

TGen::Engine::Log::Log(LogMessageType type) 
	: type(type)
{
	
}

TGen::Engine::Log::~Log() {
	
}


void TGen::Engine::Log::addTarget(TGen::Engine::LogTarget * target) {
	targets.push_back(target);
}

TGen::Engine::Log & TGen::Engine::Log::output(const std::string & area, const std::string & text) {	
	std::vector<std::string> texts;
	texts.reserve(2);
	
	if (text.find("\n") != std::string::npos) {
		std::string::size_type pos = 0;
		while (pos < text.size()) {
			std::string::size_type nextPos = text.find("\n", pos);
			if (nextPos == std::string::npos)
				nextPos = text.size();
			
			texts.push_back("   " + text.substr(pos, nextPos - pos));
			pos = nextPos + 1;
		}
	}
	else {
		texts.push_back(text);
	}
	
	for (int i = 0; i < targets.size(); ++i) {
		for (int a = 0; a < texts.size(); ++a) {
			targets[i]->output(type, area, texts[a]);
		}
	}
	
	return *this;
}

void TGen::Engine::Log::outputText(const std::string & text, uint user) {
	output("", text);
}

TGen::Engine::StandardLogs::StandardLogs()
	: info(TGen::Engine::Info)
	, warning(TGen::Engine::Warning)
	, error(TGen::Engine::Error)
	, logtargets(NULL)
{
	logtargets = new TGen::Engine::CombinedLogTargets;
	logtargets->addTarget(new TGen::Engine::StreamOutput(std::cout));

	info.addTarget(logtargets);
	warning.addTarget(logtargets);
	error.addTarget(logtargets);	
}

TGen::Engine::StandardLogs::~StandardLogs() {
	delete logtargets;
}

void TGen::Engine::StandardLogs::outputText(const std::string & text, uint user) {
	info.output("", text);
}

