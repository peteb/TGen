/*
 *  maploader.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/29/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "maploader.h"
#include "map.h"
#include "log.h"
#include "file.h"
#include "filesystem.h"
#include "maptokenizer.h"
#include "mapmodel.h"
#include "mapsurface.h"

TGen::Engine::MapLoader::MapLoader(TGen::Engine::StandardLogs & logs, TGen::Engine::Filesystem & filesystem)
	: logs(logs)
	, filesystem(filesystem)
{
}

TGen::Engine::Map * TGen::Engine::MapLoader::createMap(const std::string & name, const std::string & filename, const TGen::VertexTransformer & transformer) {
	logs.info["map"] << "loading map " << filename << "..." << TGen::endl;
	
	std::auto_ptr<TGen::Engine::File> file(filesystem.openRead(filename));
	
	TGen::Engine::MapTokenizer tokenizer;
	tokenizer.tokenizeString(file->readAll(), tokens, false);
	currentToken = tokens.getFirstToken();
	endIter = tokens.getEndToken();
	
	TGen::Vector3 origin = TGen::Vector3::Zero;
	transformer.transform(origin);
	
	std::auto_ptr<TGen::Engine::Map> newMap(new TGen::Engine::Map(name, origin));
	
	parseGlobalBlock(newMap.get());
	
	
	return newMap.release();
}

void TGen::Engine::MapLoader::parseGlobalBlock(TGen::Engine::Map * map) {
	if (currentToken == endIter)
		throw TGen::RuntimeException("MapLoader::parseGlobalBlock", "no tokens!");
	
	if (currentToken->first != TGen::Engine::ProcTokenIdent)
		throw TGen::RuntimeException("MapLoader::parseGlobalBlock", "invalid ident!");
	
	step();
	
	while (currentToken != endIter) {
		if (currentToken->first == TGen::Engine::ProcTokenModel) {
			stepAndCheck();
			if (currentToken->first != ProcTokenBlockStart)
				throw TGen::RuntimeException("MapLoader::parseGlobalBlock", "model: no block start!");
			
			stepAndCheck();
			map->addModel(parseModelBlock(map));
		}
		else if (currentToken->first == TGen::Engine::ProcTokenIAP) {
			/*stepAndCheck();
			if (currentToken->first != ProcTokenBlockStart)
				throw TGen::RuntimeException("ProcParser::parseGlobalBlock", "iap: no block start!");
			
			stepAndCheck();
			parseIAPBlock(procmap);*/
		}
		else if (currentToken->first == TGen::Engine::ProcTokenNodes) {
			/*stepAndCheck();
			if (currentToken->first != ProcTokenBlockStart)
				throw TGen::RuntimeException("ProcParser::parseGlobalBlock", "nodes: no block start!");
			
			stepAndCheck();
			parseNodesBlock(procmap);*/
		}
		else {
			
		}
		
		if (currentToken != endIter)
			step();
	}	
}

TGen::Engine::MapModel * TGen::Engine::MapLoader::parseModelBlock(TGen::Engine::Map * map) {
	std::string modelName = currentToken->second; stepAndCheck();
	std::string surfaceCount = currentToken->second; stepAndCheck();
	
	std::auto_ptr<TGen::Engine::MapModel> model(new TGen::Engine::MapModel(modelName, map));

	std::cout << "create model: model name: " << modelName << " surfaces: " << surfaceCount << std::endl;
	
	
	while (currentToken != endIter) {
		if (currentToken->first == ProcTokenBlockStart) {
			stepAndCheck();
			model->addSurface(parseSurfaceBlock());
			//std::cout << "added surf" << std::endl;
		}
		else if (currentToken->first == ProcTokenBlockEnd) {
			//stepAndCheck();
			return model.release();
		}
		
		if (currentToken != endIter)
			step();		
	}
	
	throw TGen::RuntimeException("MapLoader::parseModelBlock", "unexpected end");	
}

TGen::Engine::MapSurface * TGen::Engine::MapLoader::parseSurfaceBlock() {
	std::string materialName = currentToken->second; stepAndCheck();
	std::string numVerts = currentToken->second; stepAndCheck();
	std::string numIndices = currentToken->second; stepAndCheck();
	
	std::auto_ptr<TGen::Engine::MapSurface> surface(new TGen::Engine::MapSurface(materialName));

	std::cout << "create surface: material: " + materialName + " numVerts: " + numVerts + " numIndices: " + numIndices << std::endl;
			
	while (currentToken != endIter) {
		if (currentToken->first == ProcTokenArrayStart) {
			surface->addVertex(parseVertex());
		}
		else if (currentToken->first == TGen::TokenValueNumeric) {
			surface->addIndex(TGen::lexical_cast<uint>(currentToken->second));
		}
		else if (currentToken->first == ProcTokenBlockEnd) {
			////stepAndCheck();
			//if (surface->getNumVertices() != TGen::lexical_cast<int>(numVerts))
			//	throw TGen::RuntimeException("MapLoader::parseSurfaceBlock", "number of vertices doesn't match");
			//if (surface->getNumIndices() != TGen::lexical_cast<int>(numIndices))
			//	throw TGen::RuntimeException("MapLoader::parseSurfaceBlock", "number of indices doesn't match");
			surface->calculateTangents();
			
			return surface.release();
		}
				
		if (currentToken != endIter)
			step();
	}
	
	throw TGen::RuntimeException("MapLoader::parseSurfaceBlock", "unexpected end");	
}

TGen::Engine::MapSurface::VertexDecl::Type TGen::Engine::MapLoader::parseVertex() {
	TGen::Engine::MapSurface::VertexDecl::Type vertex;
		
	if (currentToken->first != ProcTokenArrayStart)
		throw TGen::RuntimeException("MapLoader::parseVertex", "invalid vertex, must start with array start");
	
	std::string x, y, z, u, v, nx, ny, nz;
	stepAndCheck();
	x = currentToken->second;
	stepAndCheck();
	y = currentToken->second;
	stepAndCheck();
	z = currentToken->second;
	stepAndCheck();
	u = currentToken->second;
	stepAndCheck();
	v = currentToken->second;
	stepAndCheck();
	nx = currentToken->second;
	stepAndCheck();
	ny = currentToken->second;
	stepAndCheck();
	nz = currentToken->second;
	stepAndCheck();
	
	if (currentToken->first != ProcTokenArrayEnd)
		throw TGen::RuntimeException("ProcParser::parseVertex", "invalid vertex, must end with array end");
	
	//stepAndCheck();

	//std::cout << "x: " + x + " y: " + y + " z: " + z + " u: " + u + " v: " + v + " nx: " + nx + " ny: " + ny + " nz: " + nz << std::endl;
	vertex.MapSurface::Vertex::Type::x = TGen::lexical_cast<float>(x);
	vertex.MapSurface::Vertex::Type::y = TGen::lexical_cast<float>(z);
	vertex.MapSurface::Vertex::Type::z = TGen::lexical_cast<float>(y);
	vertex.MapSurface::TexCoord::Type::u = TGen::lexical_cast<float>(u);
	vertex.MapSurface::TexCoord::Type::v = TGen::lexical_cast<float>(v);
	vertex.MapSurface::Normal::Type::nx = TGen::lexical_cast<float>(nx);
	vertex.MapSurface::Normal::Type::ny = TGen::lexical_cast<float>(nz);
	vertex.MapSurface::Normal::Type::nz = TGen::lexical_cast<float>(ny);
	vertex.MapSurface::Tangent::Type::x = 0.0;
	vertex.MapSurface::Tangent::Type::y = 0.0;
	vertex.MapSurface::Tangent::Type::z = 0.0;
	
	//std::cout << "x: " << vertex.x << " y: " << vertex.y << " z: " << vertex.z << " u: " << vertex.u << " v: " << vertex.v << " nx: " << vertex.nx << " ny: " << vertex.ny << " nz: " << vertex.nz << std::endl;
	
	return vertex;	
}

void TGen::Engine::MapLoader::step() {
	tokens.stepToken(currentToken, endIter);
}

void TGen::Engine::MapLoader::checkEOS() {
	if (currentToken == endIter)
		throw TGen::RuntimeException("MapLoader::checkEOS", "unexpected end");
}

void TGen::Engine::MapLoader::stepAndCheck() {
	step();
	checkEOS();
}