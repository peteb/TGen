/*
 *  maploader.cpp
 *  The Place
 *
 *  Created by Peter Backman on 7/13/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "maploader.h"
#include "mesh.h"
#include "meshdata.h"
#include <iostream>

MapLoader::MapLoader() {}

MapLoader::~MapLoader() {}

void MapLoader::Parse(const char * code, std::list<Map *> & maps) {
	MapTokenizer tokenizer;	
	tokenizer.TokenizeString(code, tokens, false);
	
	currentToken = tokens.getFirstToken();
	endIter = tokens.getEndToken();
	
	ParseGlobalBlock();
}

void MapLoader::ParseGlobalBlock() {
	while (currentToken != endIter) {
		if (currentToken->first == MapTokenMesh) {
			StepToken();
			std::string meshName = getStringToken("global.mesh: expecting string value for name", false, true);
			StepOverLF();
			
			if (currentToken->first != MapTokenBlockStart) 
				throw TGen::RuntimeException("MapLoader::ParseGlobalBlock", "mesh: expecting block start, not '" + currentToken->second + "'!");

			StepToken();
			
			Mesh * newMesh = new Mesh(meshName);
			std::cout << "entering mesh " << meshName << "..." << std::endl;
			ParseMeshBlock(newMesh);
			std::cout << "leaving mesh" << std::endl;
			
		}
		else if (currentToken->first != MapTokenEndOfLine) {
			throw TGen::RuntimeException("MapLoader::ParseGlobalBlock", "not expecting '" + currentToken->second + "'!");			
		}
		
		StepToken();
	}
}

void MapLoader::ParseMeshBlock(Mesh * mesh) {
	while (currentToken != endIter && currentToken->first != MapTokenBlockEnd) {
		if (currentToken->first == MapTokenVertices) {
			TGen::VertexStructure vertexStructure;
			
			StepToken();
			while (currentToken->first != MapTokenEndOfLine && currentToken->first != MapTokenBlockStart) {
				std::string format = getStringToken("mesh.vertices: expecting string value for format", false, false);
				StepToken();
				vertexStructure.AddElement(format);
			}
			
			while (currentToken->first == MapTokenEndOfLine) {
				StepToken();
			}	

			if (currentToken->first != MapTokenBlockStart) 
				throw TGen::RuntimeException("MapLoader::ParseMeshBlock", "mesh.vertices: expecting block start, not '" + currentToken->second + "'!");
			
			StepToken();
			
			MeshData * meshData = new MeshData(vertexStructure);
			StepToken();
			
			std::cout << "entering vertices..." << std::endl;
			ParseVertices(meshData);
			std::cout << "leaving vertices" << std::endl;
		}
		else if (currentToken->first == MapTokenIndices) {
			
		}
		else if (currentToken->second == "primitive") {
			StepToken();
			std::string primitiveType = getStringToken("mesh.primitive: expecting string value for type", false, false);
			
		}
		else if (currentToken->first != MapTokenEndOfLine) {
			throw TGen::RuntimeException("MapLoader::ParseMeshBlock", "not expecting '" + currentToken->second + "'!");						
		}
		
		StepToken();
	}
	
}

void MapLoader::ParseVertices(MeshData * mesh) {
	int components = 0, formatComponents = mesh->structure.getComponentCount();
	int totalComponentsSize = 0;
	
	std::vector<std::pair<std::string, TGen::FormatType> > data;
	data.reserve(100);
	
	while (currentToken != endIter && currentToken->first != MapTokenBlockEnd) {
		while (currentToken->first == MapTokenEndOfLine && currentToken->first != MapTokenBlockEnd)
			StepToken();
		
		if (currentToken->first == MapTokenBlockEnd)
			break;
		
		std::string value = getNumericToken("mesh.vertices: expecting numeric value for component", false, false);
		int componentPos = components % formatComponents;

		TGen::VertexElement vel = mesh->structure.getElementAtComponent(componentPos);
		std::cout << "VALUE: " << value << " (" << componentPos << ") TYPE " << int(vel.dataType) << std::endl;
	
		totalComponentsSize += TGen::FormatTypeSize(vel.dataType);
		
		data.push_back(std::pair<std::string, TGen::FormatType>(value, vel.dataType));
		
		components++;
		StepToken();
	}
	
	if (components % formatComponents)
		throw TGen::RuntimeException("MapLoader::ParseVertices", "number of components and buffer format mismatch");
	
	std::cout << "Total components: " << data.size() << " bytes: " << totalComponentsSize << std::endl;
	
	void * vertexData = static_cast<void *>(malloc(totalComponentsSize));
	void * pos = vertexData;
	
	for (int i = 0; i < data.size(); ++i) {
		std::stringstream ss;
		
		if (reinterpret_cast<uchar *>(pos) >= reinterpret_cast<uchar *>(vertexData) + totalComponentsSize)
			throw TGen::RuntimeException("MapLoader::ParseVertices", "out of bounds when writing vertex data");
		
		switch (data[i].second) {
			case TGen::TypeFloat: {
				float value = 0.0f;
				ss << data[i].first;
				ss >> value;
				
				memcpy(pos, &value, sizeof(float));
				break;
			}
				
			case TGen::TypeDouble: {
				float value = 0.0;
				ss << data[i].first;
				ss >> value;
				
				memcpy(pos, &value, sizeof(double));
				break;
			}
				
			case TGen::TypeUnsignedInt:
			case TGen::TypeInt:
			{
				unsigned int value = 0;
				ss << data[i].first;
				ss >> value;
				
				memcpy(pos, &value, sizeof(int));
				break;
			}
				
			case TGen::TypeUnsignedShort:
			case TGen::TypeShort:
			{
				unsigned short value = 0;
				ss << data[i].first;
				ss >> value;
				
				memcpy(pos, &value, sizeof(short));
				break;
			}
				
			case TGen::TypeUnsignedByte:
			case TGen::TypeByte:
			{
				unsigned char value = 0;
				ss << data[i].first;
				ss >> value;
				
				memcpy(pos, &value, sizeof(char));
				break;
			}

			default:
				throw TGen::RuntimeException("MapLoader::ParseVertices", "invalid datatype");
		}
		
		pos = reinterpret_cast<uchar *>(pos) + TGen::FormatTypeSize(data[i].second);
	}
	
	std::cout << "written " << reinterpret_cast<uchar *>(pos) - reinterpret_cast<uchar *>(vertexData) << " bytes" << std::endl;

	mesh->data = vertexData;
	// TODO: data, vad gör man med den va. spara alla MeshData i en lista som betas av när man länkar, då skapar den alla vbs osv
}

void MapLoader::Link(MapLinkCallback & callback) {
	
}


std::string MapLoader::getStringToken(const std::string & name, bool ignorelf, bool quote) {
	std::string ret;
	
	if (ignorelf) {
		while(currentToken->first == MapTokenEndOfLine) {
			tokens.NextToken(currentToken, endIter);
		}
	}
	
	if (currentToken->first == TGen::TokenValueString || currentToken->first == TGen::TokenQuote)
		ret = currentToken->second;
	else
		throw TGen::RuntimeException("MapLoader::getStringToken", name + ", not '" + currentToken->second + "' (") << currentToken->first << ")";
	
	return ret;
}

std::string MapLoader::getNumericToken(const std::string & name, bool ignorelf, bool quote) {
	std::string ret;
	
	if (ignorelf) {
		while(currentToken->first == MapTokenEndOfLine) {
			tokens.NextToken(currentToken, endIter);
		}
	}
	
	if (currentToken->first == TGen::TokenValueNumeric)
		ret = currentToken->second;
	else
		throw TGen::RuntimeException("MapLoader::getNumericTokenk", name + ", not '" + currentToken->second + "' (") << currentToken->first << ")";
	
	return ret;
}

void MapLoader::StepToken() {
	tokens.NextToken(currentToken, endIter);
}

void MapLoader::StepOverLF() {
	do {
		StepToken();
	} while(currentToken->first == MapTokenEndOfLine);	
}

MapTokenizer::MapTokenizer() {
	autoAddTilNextIgnore = true;
	
	ignores.push_back(" ");
	ignores.push_back("\t");
	ignores.push_back("\r");
	//	ignores.push_back("\n");
	
	quotes.push_back(StringPair("\"", "\""));
	comments.push_back(StringPair("//", "\n"));
	comments.push_back(StringPair("/*", "*/"));
		
	tokens["{"] = MapTokenBlockStart;
	tokens["}"] = MapTokenBlockEnd;
	tokens["mesh"] = MapTokenMesh;
	tokens["vertices"] = MapTokenVertices;
	tokens["indices"] = MapTokenIndices;
	tokens["\n"] = MapTokenEndOfLine;
}

int MapTokenizer::getSpecialToken(char * text, TGen::TokenStream & stream) {
	return 0;
}
