/*
 *  shaderprogram.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/28/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "shaderprogram.h"
#include "renderer.h"
#include <iostream>
#include <string>
#include <vector>

TGen::ShaderProgram::ShaderProgram(TGen::Renderer & creator) 
	: creator(creator) 
{

}

TGen::ShaderProgram::~ShaderProgram() {	
}

void TGen::ShaderProgram::parseShaders(TGen::Renderer & renderer, char * code) {
	char * pos = code;
	char * end = code + strlen(code);
	
	std::vector<std::string> sections[4];
	int currentSection = 0;
	
	for (; pos && pos < end;) {
		char * next = strchr(pos, '\n');
		if (next)
			*next = 0;
		
		char * section = strstr(pos, "#section ");
		if (section) {
			char * sectionName = section + strlen("#section ");
			//char * sectionNameEnd = strchr(sectionName, ' ');
			//*sectionNameEnd = 0;
			
			//std::cout << "Begin section '" << sectionName << "'" << std::endl;
			
			if (strncmp(sectionName, "global", strlen("global")) == 0)
				currentSection = 0;
			else if (strncmp(sectionName, "vertex", strlen("vertex")) == 0)
				currentSection = 1;
			else if (strncmp(sectionName, "geometry", strlen("geometry")) == 0)
				currentSection = 2;
			else if (strncmp(sectionName, "fragment", strlen("fragment")) == 0)
				currentSection = 3;
		}
		else {
			sections[currentSection].push_back(pos);
			//std::cout << "'" << pos << "'" << std::endl;			
		}
		
		if (next)
			pos = next + 1;
		else
			break;
	}

	int codeSize[4];
	for (int i = 0; i < 4; ++i) {
		codeSize[i] = 0;
		for (int a = 0; a < int(sections[i].size()); ++a) {
			codeSize[i] += int(sections[i][a].size()) + 1;
		}
	}
	
	char * sectionCode[4];
	
	for (int i = 0; i < 4; ++i) {
		if (i == 0) {
			sectionCode[i] = static_cast<char *>(malloc(codeSize[i] + 1));
			char * pos = sectionCode[i];
			*pos = 0;
			
			for (int a = 0; a < int(sections[i].size()); ++a) {
				strcpy(pos, sections[i][a].c_str());
				pos += sections[i][a].length();
				strcpy(pos, "\n");
				pos++;
			}
		}
		else {
			sectionCode[i] = static_cast<char *>(malloc(codeSize[0] + codeSize[i] + 1));

			char * pos = sectionCode[i];
			*pos = 0;
			
			strcpy(pos, sectionCode[0]);
			pos += strlen(sectionCode[0]);
			
			for (int a = 0; a < int(sections[i].size()); ++a) {
				strcpy(pos, sections[i][a].c_str());
				pos += sections[i][a].length();
				strcpy(pos, "\n");
				pos++;
			}
		}
	}
	
	for (int i = 0; i < 4; i++) {
		if (codeSize[i] > 0) {
			switch (i) {
				case 1:
					attach(renderer.createVertexShader(sectionCode[i]));
					break;
					
				case 2:
					attach(renderer.createGeometryShader(sectionCode[i]));					
					break;
					
				case 3:
					attach(renderer.createFragmentShader(sectionCode[i]));
					break;				
			}
			
			//std::cout << "SECTION " << i << ": " << std::endl;
			//std::cout << sectionCode[i] << std::endl;
		}
		
		if (sectionCode[i])
			free(sectionCode[i]);
		
		sectionCode[i] = NULL;
	}
	
}
