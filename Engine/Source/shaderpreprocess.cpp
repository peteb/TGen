/*
 *  shaderpreprocess.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 11/25/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "shaderpreprocess.h"
#include <iostream>
#include <vector>
#include <tgen_core.h>

std::string TGen::Engine::ShaderPreProcessor::process(const std::string & contents, const std::string & parameters) {
	//std::cout << "IN: " << std::endl << std::endl << contents << std::endl << std::endl;
	
	// parse parameters
	ParameterList params;
	params.reserve(4);
	parseParameters(parameters, params);
	
	
	std::string ret = contents;
	
	for (int i = 0; i < params.size(); ++i) {
		std::string out;
		
		int pos = 0;
		while (pos < contents.size() && pos != std::string::npos) {
			int nextPos = ret.find(params[i].first, pos);
			if (nextPos == std::string::npos) {
				out += ret.substr(pos);
				break;
			}
			
			out += ret.substr(pos, nextPos - pos);
			out += params[i].second;
			
			pos = nextPos + params[i].first.size();
		}
		
		ret = out;
	}
	
	//std::cout << "DEFF: " << ret << std::endl;
	
	// fix loops
	int pos = 0;
	std::string out;
	while (pos < ret.size() && pos != std::string::npos) {
		int nextPos = ret.find("#loop", pos);
		if (nextPos == std::string::npos) {
			out += ret.substr(pos);
			break;
		}
		
		out += ret.substr(pos, nextPos - pos);
		int lineEndPos = ret.find("\n", nextPos);
		std::string loopLine = ret.substr(nextPos + strlen("#loop "), lineEndPos - nextPos - strlen("#loop "));
		
		int firstSpace = loopLine.find(" ");
		int lastSpace = loopLine.find(" ", firstSpace + 2);
		std::string loopParamVar = loopLine.substr(0, firstSpace);
		std::string loopParamStart = loopLine.substr(firstSpace + 1, lastSpace - firstSpace - 1);
		std::string loopParamEnd = loopLine.substr(lastSpace + 1, loopLine.size() - lastSpace - 1);
		
		int loopEnd = ret.find("#end", nextPos);
		if (loopEnd == std::string::npos)
			throw TGen::RuntimeException("ShaderPreProcessor::process", "no #end for #loop!");
		
		std::string loopContents = ret.substr(lineEndPos + 1, loopEnd - lineEndPos - 1);
		
		//std::cout << loopLine << " | " << loopParamStart << " - " << loopParamEnd << std::endl;
		
		int startNum = TGen::lexical_cast<int>(loopParamStart);
		int endNum = TGen::lexical_cast<int>(loopParamEnd);
		
		for (int i = startNum; i < endNum; ++i) {
			std::string ppName = loopParamVar + "=" + TGen::lexical_cast<std::string>(i);
			std::string fixedContents;
			
			TGen::Engine::ShaderPreProcessor processor;
			fixedContents = processor.process(loopContents, ppName);
			
			out += fixedContents;
		}
		
		pos = loopEnd + strlen("#end");
	}
	
	ret = out;
	
	//std::cout << "OUT: " << std::endl << std::endl << ret << std::endl << std::endl;
	
	return ret;
}

void TGen::Engine::ShaderPreProcessor::parseParameters(const std::string & parameters, ParameterList & out) {
	int pos = 0;
	
	while (pos != std::string::npos && pos < parameters.size()) {
		int nextPos = parameters.find(",", pos);
		std::string thisParam = parameters.substr(pos, nextPos - pos);
		
		int eqPos = thisParam.find("=");
		
		if (eqPos == std::string::npos)
			throw TGen::RuntimeException("ShaderPreProcessor::parseParameters", "parameter invalid: " + thisParam);
		
		
		std::string paramName, paramValue;
		paramName = "#" + thisParam.substr(0, eqPos) + "#";
		paramValue = thisParam.substr(eqPos + 1, thisParam.size() - eqPos - 1);
		
		out.push_back(ParameterList::value_type(paramName, paramValue));
		
		if (nextPos == std::string::npos)
			break;
		
		pos = nextPos + 1;
	}	
}