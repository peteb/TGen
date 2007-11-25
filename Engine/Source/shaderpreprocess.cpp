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
	
	std::cout << "OUT: " << std::endl << std::endl << ret << std::endl << std::endl;
	
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
