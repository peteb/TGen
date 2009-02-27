/*
 *  textpreprocessor.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 11/25/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "preprocessor.h"
#include <iostream>
#include <vector>
#include <tgen_core.h>

using TGen::uint;

TGen::Engine::TextPreprocessor::TextPreprocessor()
	: defineNoValueParam(true)
{
}

std::string TGen::Engine::TextPreprocessor::process(const std::string & contents, const std::string & parameters, bool parseIfs, bool hashDefs) {
	//std::cout << "IN: " << std::endl << std::endl << contents << std::endl << std::endl;
	// TODO: #exec 1 + 3#        #exec {script}#
	
	// parse parameters
	ParameterList params;
	params.reserve(4);
	parseParameters(parameters, params, hashDefs);
	
	//std::cout << "PARAMS: " << parameters << std::endl;
	
	std::string ret = contents;
	
	for (int i = 0; i < params.size(); ++i) {
		std::string out;
		
		if (params[i].first.find("#") != params[i].first.rfind("#") || !hashDefs) {
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
		}
		else {
			if (defineNoValueParam) {
				std::string firstLine = "#define " + params[i].first.substr(1) + " " + params[i].second + "\n";
				
				out += firstLine;
				out += ret;
			}
		}
		
		ret = out;
	}
	
	//std::cout << "FIXED: >" << ret << "<" << std::endl;
	
	if (parseIfs) {
		ret = fixIfs(ret);
		
		//std::cout << ret << std::endl;
	}
	
	ret = fixLoops(ret);
	
	
//	std::cout << "OUT: " << std::endl << std::endl << ret << std::endl << std::endl;
	
	return ret;
}

std::string TGen::Engine::TextPreprocessor::processIncludes(const std::string & contents, TGen::Engine::IncludeCallback & callback) {
	std::string out;
	
	uint pos = contents.find("#include");
	uint lastPos = 0;
	
	while (pos < contents.size() && pos != std::string::npos) {
		out += contents.substr(lastPos, pos - lastPos);
		
		int nextPos = contents.find("#include", pos + 1);
		int spacePos = contents.find("<", pos);
		int spaceEnd = contents.find(">", spacePos);
		
		lastPos = spaceEnd + 2;
		
		std::string ident = contents.substr(spacePos + 1, spaceEnd - spacePos - 1);
		
		out += callback.getIncludeContent(ident);
		
		pos = nextPos;
	}
	
	out += contents.substr(lastPos);
	
	return out;	
}
									 
std::string TGen::Engine::TextPreprocessor::fixIfs(const std::string & input) const {
	std::string out;
	
	int pos = 0;
	while (pos < input.size() && pos != std::string::npos) {
		int nextPos = input.find("#if", pos);
		
		if (nextPos == std::string::npos) {
			out += input.substr(pos);
			break;
		}
		
		out += input.substr(pos, nextPos - pos);
		
		int lineEndPos = input.find("\n", nextPos);
		std::string ifLine = input.substr(nextPos + strlen("#if "), lineEndPos - nextPos - strlen("#if "));
		
		std::cout << "IFLINE: '" << ifLine << "'" << std::endl;
		
		int firstSpace = ifLine.find(" ");
		int lastSpace = ifLine.find(" ", firstSpace + 2);
		
		std::string firstPart = ifLine.substr(0, firstSpace);
		std::string operatorPart = ifLine.substr(firstSpace + 1, lastSpace - firstSpace - 1);
		std::string comparePart = ifLine.substr(lastSpace + 1);
		
		comparePart.erase(comparePart.find_last_not_of(" ") + 1);
		comparePart.erase(comparePart.find_last_not_of("\t") + 1);
		
		//std::cout << "1st part '" << firstPart << "'" << std::endl;
		//std::cout << "2nd part '" << operatorPart << "'" << std::endl;
		//std::cout << "3rd part '" << comparePart << "'" << std::endl;
		
		bool passedCompare = false;
		
		if (operatorPart == "==")
			passedCompare = (firstPart == comparePart);
		else if (operatorPart == "!=")
			passedCompare = (firstPart != comparePart);

		int blockEnd = input.find("#endif", lineEndPos);

		if (passedCompare)
			out += input.substr(lineEndPos + 1, blockEnd - lineEndPos - 1);
		
		pos = blockEnd + strlen("#endif") + 1;
	}
	
	
	return out;
}

std::string TGen::Engine::TextPreprocessor::fixLoops(const std::string & input) const {
	int pos = 0;
	std::string out;
	
	while (pos < input.size() && pos != std::string::npos) {
		int nextPos = input.find("#loop", pos);
		if (nextPos == std::string::npos) {
			out += input.substr(pos);
			break;
		}
		
		out += input.substr(pos, nextPos - pos);
		int lineEndPos = input.find("\n", nextPos);
		std::string loopLine = input.substr(nextPos + strlen("#loop "), lineEndPos - nextPos - strlen("#loop "));
		
		int firstSpace = loopLine.find(" ");
		int lastSpace = loopLine.find(" ", firstSpace + 2);
		std::string loopParamVar = loopLine.substr(0, firstSpace);
		std::string loopParamStart = loopLine.substr(firstSpace + 1, lastSpace - firstSpace - 1);
		std::string loopParamEnd = loopLine.substr(lastSpace + 1, loopLine.size() - lastSpace - 1);
		
		int loopEnd = input.find("#end", nextPos);
		if (loopEnd == std::string::npos)
			throw TGen::RuntimeException("TextPreprocessor::process", "no #end for #loop!");
		
		std::string loopContents = input.substr(lineEndPos + 1, loopEnd - lineEndPos - 1);
		
		//std::cout << loopLine << " | " << loopParamStart << " - " << loopParamEnd << std::endl;
		
		int startNum = TGen::lexical_cast<int>(loopParamStart);
		int endNum = TGen::lexical_cast<int>(loopParamEnd);
		
		for (int i = startNum; i < endNum; ++i) {
			std::string ppName = loopParamVar + "=" + TGen::lexical_cast<std::string>(i);
			std::string fixedContents;
			
			TGen::Engine::TextPreprocessor processor;
			fixedContents = processor.process(loopContents, ppName);
			
			out += fixedContents;
		}
		
		pos = loopEnd + strlen("#end");
	}	
	
	return out;
}

void TGen::Engine::TextPreprocessor::parseParameters(const std::string & parameters, ParameterList & out, bool hashDefs) {
	int pos = 0;
	
	while (pos != std::string::npos && pos < parameters.size()) {
		int nextPos = parameters.find(",", pos);
		std::string thisParam = parameters.substr(pos, nextPos - pos);
		
		std::string paramName, paramValue;
		int eqPos = thisParam.find("=");
		
		if (eqPos == std::string::npos) {	// common define
			if (defineNoValueParam) {
				paramName = "#" + thisParam;
				paramValue = "1";				
			}
			else {
				throw TGen::RuntimeException("TextPreprocessor::parseParameters", "parameter invalid: " + thisParam);
			}
		}
		else {
			if (hashDefs)
				paramName = "#" + thisParam.substr(0, eqPos) + "#";
			else
				paramName = thisParam.substr(0, eqPos);
				
			paramValue = thisParam.substr(eqPos + 1, thisParam.size() - eqPos - 1);
		}

		//std::cout << "PREPROCESSOR: ADD DEFINITION TOKEN: \"" << paramName << "\" value: \"" << paramValue << "\"" << std::endl;
		
		out.push_back(ParameterList::value_type(paramName, paramValue));
		
		if (nextPos == std::string::npos)
			break;
		
		pos = nextPos + 1;
	}	
}
