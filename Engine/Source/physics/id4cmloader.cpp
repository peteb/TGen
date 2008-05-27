/*
 *  id4cmloader.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/17/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "physics/id4cmloader.h"
#include "physics/id4cmtokenizer.h"
#include "physics/id4cmgeom.h"

#include "filesystem.h"
#include "file.h"
#include "ectriangulator.h"
#include <tgen_renderer.h>

using TGen::Engine::Physics::StridedVertex;
using TGen::Engine::Physics::StridedTriangle;

TGen::Engine::Physics::Id4CMLoader::Id4CMLoader(TGen::Engine::Filesystem & filesystem)
	: filesystem(filesystem)
{
}

TGen::Engine::Physics::Id4CMGeom * TGen::Engine::Physics::Id4CMLoader::createGeom(const std::string & name, const std::string & path, const TGen::VertexTransformer & transformer, dSpaceID space) {
	std::auto_ptr<TGen::Engine::File> file(filesystem.openRead(path));
	
	TGen::Engine::Physics::Id4CMTokenizer tokenizer;
	tokenizer.tokenizeString(file->readAll(), tokens, false);
	currentToken = tokens.getFirstToken();
	endIter = tokens.getEndToken();
	
	std::auto_ptr<TGen::Engine::Physics::Id4CMGeom> newModel(new TGen::Engine::Physics::Id4CMGeom(name));
	
	parseGlobalBlock();

	newModel->vertexData.reserve(vertices.size());
	newModel->indexData.reserve(polygons.size() * 6);
	
	for (int i = 0; i < vertices.size(); ++i) {
		StridedVertex vertex;
		TGen::Vector3 coord = TGen::Vector3(vertices[i].x, vertices[i].y, vertices[i].z);
		
		transformer.transform(coord);
		
		vertex.vertex[0] = coord.x;
		vertex.vertex[1] = coord.y;
		vertex.vertex[2] = coord.z;
		
		newModel->vertexData.push_back(vertex);
	}
	
	uint numIndices = 0;
	
	for (int i = 0; i < polygons.size(); ++i) {
		std::vector<uint> indices;
		std::vector<uint> newIndices;
		
		getContour(indices, polygons[i]);
		

		if (indices.size() == 4) {
			newIndices.push_back(indices[0]);
			newIndices.push_back(indices[1]);
			newIndices.push_back(indices[2]);
			newIndices.push_back(indices[2]);
			newIndices.push_back(indices[3]);
			newIndices.push_back(indices[0]);
			
		}
		else {
			TGen::Engine::EarClippingTriangulator triangulator;
			
			for (int i = 0; i < indices.size(); ++i)
				triangulator.add(vertices[indices[i]]);
			
			
			triangulator.triangulate(newIndices);
			
			for (int i = 0; i < newIndices.size(); ++i)
				newIndices[i] = indices[newIndices[i]];
		}
		
		std::cout << "TRI: " << newIndices.size() << std::endl;

		for (int i = 0; i < newIndices.size(); i += 3) {
			TGen::Engine::Physics::StridedTriangle tri;
			
			std::cout << newIndices[i] << " - " << newIndices[i + 1] << " - " << newIndices[i + 2] << std::endl;
			
			tri.indices[2] = newIndices[i + 0];
			tri.indices[1] = newIndices[i + 1];
			tri.indices[0] = newIndices[i + 2];
			
			newModel->indexData.push_back(tri);			
		}
		
		numIndices += newIndices.size();				
	}
	
	dTriMeshDataID meshData = dGeomTriMeshDataCreate();
	dGeomTriMeshDataBuildSingle(meshData, &newModel->vertexData[0], sizeof(StridedVertex), vertices.size(),
										 &newModel->indexData[0], numIndices, sizeof(StridedTriangle));
	dGeomTriMeshDataPreprocess(meshData);
	
	dGeomID geomId = dCreateTriMesh(space, meshData, NULL, NULL, NULL);
	newModel->setGeom(geomId);
	
	return newModel.release();
}

void TGen::Engine::Physics::Id4CMLoader::getContour(std::vector<uint> & contour, const TGen::Engine::Physics::Id4CMPolygon & polygon) {
	for (int i = 0; i < polygon.getNumEdges(); ++i) {
		int edgeNumSigned = polygon.getEdge(i);
		int edgeNum = abs(edgeNumSigned);
		int firstVert = edges[edgeNum].first;
		int secondVert = edges[edgeNum].second;
		
		if (edgeNumSigned < 0)
			std::swap(firstVert, secondVert);
		
		if (std::find(contour.begin(), contour.end(), firstVert) == contour.end())
			contour.push_back(firstVert);
		
		if (std::find(contour.begin(), contour.end(), secondVert) == contour.end())
			contour.push_back(secondVert);		
	}
}

void TGen::Engine::Physics::Id4CMLoader::parseGlobalBlock() {
	if (currentToken == endIter)
		throw TGen::RuntimeException("Id4CMLoader::parseGlobalBlock", "no tokens!");
	
	if (currentToken->second != "CM")
		throw TGen::RuntimeException("Id4CMLoader::parseGlobalBlock", "file has to start with CM!");
	
	step();
	
	while (currentToken != endIter) {
		if (currentToken->first == Id4CMTokenCollisionModel) {
			stepAndCheck();
			std::string modelName = currentToken->second;
			stepAndCheck();
			
			if (currentToken->first != Id4CMTokenBlockStart)
				throw TGen::RuntimeException("Id4CMLoader::parseGlobalBlock", "model: no block start!");
			
			stepAndCheck();
			
			parseCollisionModelBlock(modelName);
		}
		
		if (currentToken != endIter)
			step();
	}		
}

void TGen::Engine::Physics::Id4CMLoader::parseCollisionModelBlock(const std::string & name) {
	while (currentToken != endIter) {
		if (currentToken->first == Id4CMTokenBlockEnd) {
			stepAndCheck();
			
			return;
		}
		else if (currentToken->first == Id4CMTokenVertices) {
			stepAndCheck();
			
			if (currentToken->first != Id4CMTokenBlockStart)
				throw TGen::RuntimeException("Id4CMLoader::parseCollisionModel", "vertices: no block start!");
			
			stepAndCheck();
			
			parseVertexBlock();
		}
		else if (currentToken->first == Id4CMTokenEdges) {
			stepAndCheck();
			
			if (currentToken->first != Id4CMTokenBlockStart)
				throw TGen::RuntimeException("Id4CMLoader::parseCollisionModel", "edges: no block start!");
			
			stepAndCheck();
			
			parseEdgeBlock();
			
		}
		else if (currentToken->first == Id4CMTokenNodes) {
			stepAndCheck();
			
			if (currentToken->first != Id4CMTokenBlockStart)
				throw TGen::RuntimeException("Id4CMLoader::parseCollisionModel", "nodes: no block start!");
			
			stepAndCheck();
			
			parseNodeBlock();
			
		}
		else if (currentToken->first == Id4CMTokenPolygons) {
			stepAndCheck();
			
			uint polygonMemory = TGen::lexical_cast<uint>(currentToken->second); stepAndCheck();
			
			if (currentToken->first != Id4CMTokenBlockStart)
				throw TGen::RuntimeException("Id4CMLoader::parseCollisionModel", "polygons: no block start!");
			
			stepAndCheck();
			
			parsePolygonBlock();			
		}
		else if (currentToken->first == Id4CMTokenBrushes) {
			
		}
		
		if (currentToken != endIter)
			step();
	}
}

void TGen::Engine::Physics::Id4CMLoader::parseVertexBlock() {
	uint numVertices = TGen::lexical_cast<uint>(currentToken->second);
	
	vertices.reserve(numVertices);
	
	stepAndCheck();
	
	while (currentToken != endIter) {
		if (currentToken->first == Id4CMTokenBlockEnd) {
//			if (vertices.size() != numVertices)
	//			throw TGen::RuntimeException("Id4CMLoader::parseVertexBlock", "vertex count mismatch");
			
			return;
		}
		else if (currentToken->first == Id4CMTokenArrayStart) {
			stepAndCheck();
			std::string x = currentToken->second; stepAndCheck();
			std::string y = currentToken->second; stepAndCheck();
			std::string z = currentToken->second; stepAndCheck();
			
			if (currentToken->first != Id4CMTokenArrayEnd)
				throw TGen::RuntimeException("Id4CMLoader::parseVertexBloxk", "expecting array end, not " + currentToken->second + "!");
						
			TGen::Vector3 coord(TGen::lexical_cast<scalar>(x),
									  TGen::lexical_cast<scalar>(y),
									  TGen::lexical_cast<scalar>(z));
			
			vertices.push_back(coord);
		}
		
		if (currentToken != endIter)
			step();
	}	
}

void TGen::Engine::Physics::Id4CMLoader::parseEdgeBlock() {
	uint numEdges = TGen::lexical_cast<uint>(currentToken->second);
	
	edges.reserve(numEdges);
	
	stepAndCheck();
	
	while (currentToken != endIter) {
		if (currentToken->first == Id4CMTokenBlockEnd) {
			//if (edges.size() != numEdges)
			//	throw TGen::RuntimeException("Id4CMLoader::parseEdgeBlock", "edge count mismatch");
			
			return;
		}
		else if (currentToken->first == Id4CMTokenArrayStart) {
			stepAndCheck();
		
			std::string firstVertex = currentToken->second; stepAndCheck();
			std::string secondVertex = currentToken->second; stepAndCheck();
			
			if (currentToken->first != Id4CMTokenArrayEnd)
				throw TGen::RuntimeException("Id4CMLoader::parseEdgeBlock", "expecting array end, not " + currentToken->second + "!");
			
			std::string unknown1 = currentToken->second; stepAndCheck();
			std::string unknown2 = currentToken->second; stepAndCheck();
			
			edges.push_back(std::make_pair(TGen::lexical_cast<uint>(firstVertex), TGen::lexical_cast<uint>(secondVertex)));
		}
		
		if (currentToken != endIter)
			step();
	}	
}


/*
 void addEdge(uint num);
 
 TGen::Plane3 plane;
 TGen::Vector3 min, max;
 std::string materialName;
 */

void TGen::Engine::Physics::Id4CMLoader::parseNodeBlock() {
	while (currentToken != endIter) {
		if (currentToken->first == Id4CMTokenBlockEnd) {			
			return;
		}
		else if (currentToken->first == Id4CMTokenArrayStart) {
			stepAndCheck();
			
			std::string axis = currentToken->second; stepAndCheck();
			std::string position = currentToken->second; stepAndCheck();
			
			if (currentToken->first != Id4CMTokenArrayEnd)
				throw TGen::RuntimeException("Id4CMLoader::parseNodeBlock", "expecting array end, not " + currentToken->second + "!");
		}
		
		if (currentToken != endIter)
			step();
	}	
}

void TGen::Engine::Physics::Id4CMLoader::parsePolygonBlock() {	
	while (currentToken != endIter) {
		if (currentToken->first == Id4CMTokenBlockEnd) {
			
			return;
		}
		else {
			std::string numSidesStr = currentToken->second; stepAndCheck();
			
			if (currentToken->first != Id4CMTokenArrayStart)
				throw TGen::RuntimeException("Id4CMLoader::parsePolygonBlock", "expecting array start, not " + currentToken->second + "!");
			
			stepAndCheck();
			
			Id4CMPolygon polygon;
			int numSides = TGen::lexical_cast<int>(numSidesStr);
			
			for (int i = 0; i < numSides; ++i) {
				int edgeNum = TGen::lexical_cast<int>(currentToken->second); stepAndCheck();	
				polygon.addEdge(edgeNum);				
			}
			
			if (currentToken->first != Id4CMTokenArrayEnd)
				throw TGen::RuntimeException("Id4CMLoader::parsePolygonBlock", "expecting array end, not " + currentToken->second + "!");
			
			stepAndCheck();
			
			if (currentToken->first != Id4CMTokenArrayStart)
				throw TGen::RuntimeException("Id4CMLoader::parsePolygonBlock", "expecting array start, not " + currentToken->second + "!");
			
			stepAndCheck();
			
			std::string px = currentToken->second; stepAndCheck();
			std::string py = currentToken->second; stepAndCheck();
			std::string pz = currentToken->second; stepAndCheck();
			
			if (currentToken->first != Id4CMTokenArrayEnd)
				throw TGen::RuntimeException("Id4CMLoader::parsePolygonBlock", "expecting array end, not " + currentToken->second + "!");
			
			stepAndCheck();
			
			std::string pd = currentToken->second; stepAndCheck();
			
			polygon.plane = TGen::Plane3(TGen::Vector3(TGen::lexical_cast<scalar>(px),
																	 TGen::lexical_cast<scalar>(py),
																	 TGen::lexical_cast<scalar>(pz)),
												  TGen::lexical_cast<scalar>(pd));
			
			// TODO: gör en parseVertex3 och checkArrayStart/End
			
			if (currentToken->first != Id4CMTokenArrayStart)
				throw TGen::RuntimeException("Id4CMLoader::parsePolygonBlock", "expecting array start, not " + currentToken->second + "!");
			
			stepAndCheck();
			
			std::string minx = currentToken->second; stepAndCheck();
			std::string miny = currentToken->second; stepAndCheck();
			std::string minz = currentToken->second; stepAndCheck();
			
			if (currentToken->first != Id4CMTokenArrayEnd)
				throw TGen::RuntimeException("Id4CMLoader::parsePolygonBlock", "expecting array end, not " + currentToken->second + "!");

			stepAndCheck();
			
			if (currentToken->first != Id4CMTokenArrayStart)
				throw TGen::RuntimeException("Id4CMLoader::parsePolygonBlock", "expecting array start, not " + currentToken->second + "!");
				
			stepAndCheck();
			
			std::string maxx = currentToken->second; stepAndCheck();
			std::string maxy = currentToken->second; stepAndCheck();
			std::string maxz = currentToken->second; stepAndCheck();
			
			if (currentToken->first != Id4CMTokenArrayEnd)
				throw TGen::RuntimeException("Id4CMLoader::parsePolygonBlock", "expecting array end, not " + currentToken->second + "!");

			stepAndCheck();
			
			polygon.materialName = currentToken->second; 
			polygon.min = TGen::Vector3(TGen::lexical_cast<scalar>(minx),
												 TGen::lexical_cast<scalar>(miny),
												 TGen::lexical_cast<scalar>(minz));
			polygon.max = TGen::Vector3(TGen::lexical_cast<scalar>(maxx),
												 TGen::lexical_cast<scalar>(maxy),
												 TGen::lexical_cast<scalar>(maxz));
			
			polygons.push_back(polygon);
		}
		
		if (currentToken != endIter)
			step();
	}		
}


void TGen::Engine::Physics::Id4CMLoader::step() {
	tokens.stepToken(currentToken, endIter);
}

void TGen::Engine::Physics::Id4CMLoader::checkEOS() {
	if (currentToken == endIter)
		throw TGen::RuntimeException("Id4CMLoader::checkEOS", "unexpected end");
}

void TGen::Engine::Physics::Id4CMLoader::stepAndCheck() {
	step();
	checkEOS();
}

