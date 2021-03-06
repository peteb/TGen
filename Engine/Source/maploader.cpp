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
#include "mapnode.h"
#include "mapportal.h"

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
	
	parseGlobalBlock(newMap.get(), transformer);
	
	
	return newMap.release();
}

void TGen::Engine::MapLoader::parseGlobalBlock(TGen::Engine::Map * map, const TGen::VertexTransformer & transformer) {
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
			map->addModel(parseModelBlock(map, transformer));
		}
		else if (currentToken->first == TGen::Engine::ProcTokenIAP) {
			stepAndCheck();
			if (currentToken->first != ProcTokenBlockStart)
				throw TGen::RuntimeException("ProcParser::parseGlobalBlock", "iap: no block start!");
			
			stepAndCheck();
			parseIAPBlock(map, transformer);
		}
		else if (currentToken->first == TGen::Engine::ProcTokenNodes) {
			stepAndCheck();
			if (currentToken->first != ProcTokenBlockStart)
				throw TGen::RuntimeException("ProcParser::parseGlobalBlock", "nodes: no block start!");
			
			stepAndCheck();
			parseNodesBlock(map, transformer);
		}
		else {
			
		}
		
		if (currentToken != endIter)
			step();
	}	
}

TGen::Engine::MapModel * TGen::Engine::MapLoader::parseModelBlock(TGen::Engine::Map * map, const TGen::VertexTransformer & transformer) {
	std::string modelName = currentToken->second; stepAndCheck();
	std::string surfaceCount = currentToken->second; stepAndCheck();
	
	std::auto_ptr<TGen::Engine::MapModel> model(new TGen::Engine::MapModel(modelName, map));

	std::cout << "create model: model name: " << modelName << " surfaces: " << surfaceCount << std::endl;
	
	int surfacesAdded = 0;
	
	while (currentToken != endIter) {
		if (currentToken->first == ProcTokenBlockStart) {
			stepAndCheck();
			model->addSurface(parseSurfaceBlock(transformer));
			surfacesAdded++;
			//std::cout << "added surf" << std::endl;
		}
		else if (currentToken->first == ProcTokenBlockEnd) {
			//stepAndCheck();
			if (surfacesAdded != TGen::lexical_cast<int>(surfaceCount))
				throw TGen::RuntimeException("MapLoader::parseModelBlock", "surface count missmatch");
			
			return model.release();
		}
		
		if (currentToken != endIter)
			step();		
	}
	
	throw TGen::RuntimeException("MapLoader::parseModelBlock", "unexpected end");	
}

void TGen::Engine::MapLoader::parseIAPBlock(TGen::Engine::Map * map, const TGen::VertexTransformer & transformer) {
	std::string numAreas = currentToken->second; stepAndCheck();
	std::string numIAP = currentToken->second; stepAndCheck();
	
	int portalsCreated = 0;
	
	while (currentToken != endIter) {
		if (currentToken->first == ProcTokenBlockEnd) {
			//stepAndCheck();
			if (portalsCreated != TGen::lexical_cast<int>(numIAP))
				throw TGen::RuntimeException("MapLoader::parseIAPBlock", "portal count missmatch");
			
			return;
		}
		else {
			std::string numPoints = currentToken->second; stepAndCheck();
			std::string posArea = currentToken->second; stepAndCheck();
			std::string negArea = currentToken->second; stepAndCheck();
			
			std::cout << "num points: " << numPoints << std::endl;

			int numPointsInt = TGen::lexical_cast<int>(numPoints);

			std::auto_ptr<TGen::Engine::MapPortal> portal(new TGen::Engine::MapPortal(TGen::lexical_cast<int>(posArea), TGen::lexical_cast<int>(negArea)));
			
			for (int i = 0; i < numPointsInt; ++i) {
				if (i > 0)
					stepAndCheck();

				if (currentToken->first != ProcTokenArrayStart)
					throw TGen::RuntimeException("MapLoader::parseIAPBlock", "expecting array start, not " + currentToken->second + "!");
			
				stepAndCheck();
				std::string x = currentToken->second; stepAndCheck();
				std::string y = currentToken->second; stepAndCheck();
				std::string z = currentToken->second; stepAndCheck();
				
				if (currentToken->first != ProcTokenArrayEnd)
					throw TGen::RuntimeException("MapLoader::parseIAPBlock", "expecting array end, not " + currentToken->second + "!");
				
				TGen::Vector3 point(TGen::lexical_cast<scalar>(x), TGen::lexical_cast<scalar>(y), TGen::lexical_cast<scalar>(z));
				transformer.transform(point);
				
				portal->addPoint(point);			
			}
			
			stepAndCheck();
			
			map->addPortal(portal.release());
			portalsCreated++;
		}
		
		//if (currentToken != endIter && currentToken->first != ProcTokenBlockEnd)
		//	step();		
	}
}


TGen::Engine::MapSurface * TGen::Engine::MapLoader::parseSurfaceBlock(const TGen::VertexTransformer & transformer) {
	std::string materialName = currentToken->second; stepAndCheck();
	std::string numVerts = currentToken->second; stepAndCheck();
	std::string numIndices = currentToken->second; stepAndCheck();
	
	std::auto_ptr<TGen::Engine::MapSurface> surface(new TGen::Engine::MapSurface(materialName));

	std::cout << "create surface: material: " + materialName + " numVerts: " + numVerts + " numIndices: " + numIndices << std::endl;
	int vertsAdded = 0, indsAdded = 0;
	
	while (currentToken != endIter) {
		if (currentToken->first == ProcTokenArrayStart) {
			
			TGen::Engine::MapSurface::VertexDecl::Type newVertex = parseVertex(transformer);
			TGen::Vector3 vertexPos = TGen::Vector3(newVertex.MapSurface::Vertex::Type::x, newVertex.MapSurface::Vertex::Type::y, newVertex.MapSurface::Vertex::Type::z);

			surface->addVertex(newVertex);
			
			if (vertsAdded == 0) {
				surface->min = surface->max = surface->origin = vertexPos;
			}
			else {
				surface->min.x = std::min(surface->min.x, vertexPos.x);
				surface->min.y = std::min(surface->min.y, vertexPos.y);
				surface->min.z = std::min(surface->min.z, vertexPos.z);
				
				surface->max.x = std::max(surface->max.x, vertexPos.x);
				surface->max.y = std::max(surface->max.y, vertexPos.y);
				surface->max.z = std::max(surface->max.z, vertexPos.z);
				
			}
			
			vertsAdded++;
		}
		else if (currentToken->first == TGen::TokenValueNumeric) {
			surface->addIndex(TGen::lexical_cast<uint>(currentToken->second));
			indsAdded++;
		}
		else if (currentToken->first == ProcTokenBlockEnd) {
			surface->calculateTangents();
			surface->swapWinding();
			
			surface->origin = TGen::Interpolate(surface->min, surface->max, 0.5);
			
			if (vertsAdded != TGen::lexical_cast<int>(numVerts))
				throw TGen::RuntimeException("MapLoader::parseSurfaceBlock", "vertex count missmatch");
			
			if (indsAdded != TGen::lexical_cast<int>(numIndices))
				throw TGen::RuntimeException("MapLoader::parseSurfaceBlock", "index count missmatch");
			
			return surface.release();
		}
				
		if (currentToken != endIter)
			step();
	}
	
	throw TGen::RuntimeException("MapLoader::parseSurfaceBlock", "unexpected end");	
}

void TGen::Engine::MapLoader::parseNodesBlock(TGen::Engine::Map * map, const TGen::VertexTransformer & transformer) {
	std::string numNodes = currentToken->second; stepAndCheck();
	
	TGen::Engine::MapLinkNode * rootNode = NULL;
	
	while (currentToken != endIter) {
		if (currentToken->first == ProcTokenArrayStart) {
			stepAndCheck();
			std::string x = currentToken->second; stepAndCheck();
			std::string y = currentToken->second; stepAndCheck();
			std::string z = currentToken->second; stepAndCheck();
			std::string d = currentToken->second; stepAndCheck();
						
			if (currentToken->first != ProcTokenArrayEnd)
				throw TGen::RuntimeException("MapLoader::parseNodesBlock", "expecting array end, not " + currentToken->second);
			
			stepAndCheck();
			std::string positiveChild = currentToken->second; stepAndCheck();
			std::string negativeChild = currentToken->second;
			
			TGen::Vector3 normal(TGen::lexical_cast<scalar>(x),
										TGen::lexical_cast<scalar>(y),
										TGen::lexical_cast<scalar>(z));
			
			TGen::Vector3 distance(TGen::lexical_cast<scalar>(d), 0.0f, 0.0f);
			
			scalar sign = (TGen::lexical_cast<scalar>(d) < 0.0 ? -1.0 : 1.0);
			
			transformer.transform(normal);
			transformer.transform(distance);

			normal.normalize();
			
			TGen::Plane3 plane(normal, distance.getMagnitude() * sign);
						
			int posChild = TGen::lexical_cast<int>(positiveChild);
			int negChild = TGen::lexical_cast<int>(negativeChild);
			
			TGen::Engine::MapLinkNode * node = new TGen::Engine::MapLinkNode(plane);
			
			if (posChild > 0)
				node->setPosChild(posChild);
			else if (posChild < 0)
				node->setPosLeaf(new TGen::Engine::MapLeafNode(-1 - posChild));
			else
				node->setPosLeaf(NULL);
			
			if (negChild > 0)
				node->setNegChild(negChild);
			else if (negChild < 0)
				node->setNegLeaf(new TGen::Engine::MapLeafNode(-1 - negChild));
			else
				node->setNegLeaf(NULL);
			
			map->addNode(node);
			
			if (!rootNode) {
				rootNode = node;
			}
		}
		else if (currentToken->first == ProcTokenBlockEnd) {
			rootNode->link(map);
			map->setRootNode(rootNode);
			//rootNode->print();
			
			return;
		}
		
		if (currentToken != endIter)
			step();
	}
}

TGen::Engine::MapSurface::VertexDecl::Type TGen::Engine::MapLoader::parseVertex(const TGen::VertexTransformer & transformer) {
	TGen::Engine::MapSurface::VertexDecl::Type vertex;
		
	if (currentToken->first != ProcTokenArrayStart)
		throw TGen::RuntimeException("MapLoader::parseVertex", "invalid vertex, must start with array start");
	
	std::string x, y, z, u, v, nx, ny, nz;
	stepAndCheck();
	x = currentToken->second; stepAndCheck();
	y = currentToken->second; stepAndCheck();
	z = currentToken->second; stepAndCheck();
	u = currentToken->second; stepAndCheck();
	v = currentToken->second; stepAndCheck();
	nx = currentToken->second; stepAndCheck();
	ny = currentToken->second; stepAndCheck();
	nz = currentToken->second; stepAndCheck();
		
	if (currentToken->first != ProcTokenArrayEnd)
		throw TGen::RuntimeException("ProcParser::parseVertex", "invalid vertex, must end with array end");
	
	//stepAndCheck();

	TGen::Vector3 coord(TGen::lexical_cast<scalar>(x), 
							  TGen::lexical_cast<scalar>(y), 
							  TGen::lexical_cast<scalar>(z));
	
	TGen::Vector3 normal(TGen::lexical_cast<scalar>(nx),
								TGen::lexical_cast<scalar>(ny),
								TGen::lexical_cast<scalar>(nz));
	
	transformer.transform(coord);
	transformer.transform(normal);
	
	normal.normalize();  // transformer might scale the normal, we do this to fix that
	

	vertex.MapSurface::Vertex::Type::x = coord.x; //TGen::lexical_cast<float>(x) * 0.05;
	vertex.MapSurface::Vertex::Type::y = coord.y; //TGen::lexical_cast<float>(z) * 0.05;
	vertex.MapSurface::Vertex::Type::z = coord.z; //TGen::lexical_cast<float>(y) * 0.05;
	vertex.MapSurface::TexCoord::Type::u = TGen::lexical_cast<float>(u);
	vertex.MapSurface::TexCoord::Type::v = TGen::lexical_cast<float>(v);
	vertex.MapSurface::Normal::Type::nx = normal.x; //TGen::lexical_cast<float>(nx);
	vertex.MapSurface::Normal::Type::ny = normal.y; //TGen::lexical_cast<float>(nz);
	vertex.MapSurface::Normal::Type::nz = normal.z; //TGen::lexical_cast<float>(ny);
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