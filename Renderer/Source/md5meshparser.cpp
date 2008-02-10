/*
 *  md5parser.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 1/19/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "md5parser.h"
#include "md5file.h"
#include "md5tokenizer.h"

TGen::MD5::Parser::Parser() {
	
}

TGen::MD5::Parser::~Parser() {
	
}

TGen::MD5::File * TGen::MD5::Parser::parse(TGen::InputStream & source) {
	char * data = new char[source.getSize()];
	source.read(data, source.getSize());
	
	TGen::MD5::Tokenizer tokenizer;
	tokenizer.tokenizeString(data, tokens, false);
	
	delete [] data;
	
	currentToken = tokens.getFirstToken();
	endIter = tokens.getEndToken();
	
	return parseGlobalBlock();
}

TGen::MD5::File * TGen::MD5::Parser::parseGlobalBlock() {
	std::auto_ptr<TGen::MD5::File> file(new TGen::MD5::File);
	
	if (currentToken == endIter)
		throw TGen::RuntimeException("MD5::Parser::parseGlobalBlock", "no tokens!");
		
	while (currentToken != endIter) {
		switch (currentToken->first) {
			case TGen::MD5::TokenVersion:
				stepAndCheck();
				file->setVersion(TGen::lexical_cast<int>(currentToken->second));
				break;
				
			case TGen::MD5::TokenNumMeshes:
				stepAndCheck();
				file->setNumMeshes(TGen::lexical_cast<int>(currentToken->second));
				break;
				
			case TGen::MD5::TokenNumJoints:
				stepAndCheck();
				file->setNumJoints(TGen::lexical_cast<int>(currentToken->second));
				break;
				
			case TGen::MD5::TokenMesh:
				stepAndCheck();
				if (currentToken->first != TGen::MD5::TokenBlockStart)
					throw TGen::RuntimeException("MD5::Parser::parseGlobalBlock", "mesh: no block start");
				
				stepAndCheck();
				file->addMesh(parseMeshBlock());
				break;
				
			case TGen::MD5::TokenJoints:
				stepAndCheck();
				if (currentToken->first != TGen::MD5::TokenBlockStart)
					throw TGen::RuntimeException("MD5::Parser::parseGlobalBlock", "joints: no block start");
				
				stepAndCheck();
				parseJointsBlock(*file);
				break;
				
		}
		
		
		if (currentToken != endIter)
			step();
	}
	
	return file.release();
}

TGen::MD5::FileMesh * TGen::MD5::Parser::parseMeshBlock() {
	std::auto_ptr<TGen::MD5::FileMesh> mesh(new TGen::MD5::FileMesh);
	
	std::vector<TGen::MD5::FileVertex> vertices;
	std::vector<TGen::MD5::FileTriangle> triangles;
	std::vector<TGen::MD5::FileWeight> weights;
	
	while (currentToken != endIter) {
		if (TGen::toLower(currentToken->second) == "shader") {
			stepAndCheck();
			mesh->setMaterial(currentToken->second);
		}
		else if (TGen::toLower(currentToken->second) == "numverts") {
			stepAndCheck();
			int numVerts = TGen::lexical_cast<int>(currentToken->second);
			mesh->setNumVerts(numVerts);
			vertices.resize(numVerts);
		}
		else if (TGen::toLower(currentToken->second) == "numtris") {
			stepAndCheck();
			int numTris = TGen::lexical_cast<int>(currentToken->second);
			mesh->setNumTris(numTris);
			triangles.resize(numTris);
		}
		else if (TGen::toLower(currentToken->second) == "numweights") {
			stepAndCheck();
			int numWeights = TGen::lexical_cast<int>(currentToken->second);
			mesh->setNumWeights(numWeights);
			weights.resize(numWeights);
		}
		else if (TGen::toLower(currentToken->second) == "vert") {
			stepAndCheck();
			int vertNum = TGen::lexical_cast<int>(currentToken->second);
			
			if (vertices.size() < vertNum)
				vertices.resize(vertNum);
			
			stepAndCheck();
			if (currentToken->first != TGen::MD5::TokenArrayStart)
				throw TGen::RuntimeException("MD5::Parser::parseMeshBlock", "expecting array start");
			
			stepAndCheck();
			vertices[vertNum].s = TGen::lexical_cast<float>(currentToken->second);
			
			stepAndCheck();
			vertices[vertNum].t = TGen::lexical_cast<float>(currentToken->second);
			
			stepAndCheck();
			if (currentToken->first != TGen::MD5::TokenArrayEnd)
				throw TGen::RuntimeException("MD5::Parser::parseMeshBlock", "expecting array end");
			
			stepAndCheck();
			vertices[vertNum].startWeight = TGen::lexical_cast<int>(currentToken->second);
			
			stepAndCheck();
			vertices[vertNum].weightCount = TGen::lexical_cast<int>(currentToken->second);
			
		}
		else if (TGen::toLower(currentToken->second) == "tri") {
			stepAndCheck();
			int triNum = TGen::lexical_cast<int>(currentToken->second);
			
			if (triangles.size() < triNum)
				triangles.resize(triNum);
			
			for (int i = 0; i < 3; ++i) {
				stepAndCheck();
				triangles[triNum].indices[i] = TGen::lexical_cast<int>(currentToken->second);
			}
		}
		else if (TGen::toLower(currentToken->second) == "weight") {
			stepAndCheck();
			int weightNum = TGen::lexical_cast<int>(currentToken->second);
			
			if (weights.size() < weightNum)
				weights.resize(weightNum);
			
			stepAndCheck();
			weights[weightNum].joint = TGen::lexical_cast<int>(currentToken->second);
			
			stepAndCheck();
			weights[weightNum].bias = TGen::lexical_cast<float>(currentToken->second);
			
			stepAndCheck();
			if (currentToken->first != TGen::MD5::TokenArrayStart)
				throw TGen::RuntimeException("MD5::Parser::parseMeshBlock", "expecting array start");
			
			stepAndCheck();
			weights[weightNum].position.x = TGen::lexical_cast<float>(currentToken->second);
			
			stepAndCheck();
			weights[weightNum].position.y = TGen::lexical_cast<float>(currentToken->second);
			
			stepAndCheck();
			weights[weightNum].position.z = TGen::lexical_cast<float>(currentToken->second);
			
			stepAndCheck();
			if (currentToken->first != TGen::MD5::TokenArrayEnd)
				throw TGen::RuntimeException("MD5::Parser::parseMeshBlock", "expecting array end");
		}
		
		else if (currentToken->first == TGen::MD5::TokenBlockEnd) {
			mesh->setVertices(vertices);
			mesh->setTriangles(triangles);
			mesh->setWeights(weights);
			
			return mesh.release();
		}
		
		if (currentToken != endIter)
			step();
	}
	
	throw TGen::RuntimeException("MD5::Parser::parseMeshBlock", "unexpected end");
}

void TGen::MD5::Parser::parseJointsBlock(TGen::MD5::File & file) {
	while (currentToken != endIter) {
		std::string jointName = currentToken->second;
		stepAndCheck();
		
		int parentJoint = TGen::lexical_cast<int>(currentToken->second);
		stepAndCheck();
		
		if (currentToken->first != TGen::MD5::TokenArrayStart)
			throw TGen::RuntimeException("MD5::Parser::parseJointsBlock", "expecting array start");
		
		stepAndCheck();
		
		float posX = TGen::lexical_cast<float>(currentToken->second);
		stepAndCheck();
		
		float posY = TGen::lexical_cast<float>(currentToken->second);
		stepAndCheck();
		
		float posZ = TGen::lexical_cast<float>(currentToken->second);
		stepAndCheck();
		
		if (currentToken->first != TGen::MD5::TokenArrayEnd)
			throw TGen::RuntimeException("MD5::Parser::parseJointsBlock", "expecting array end");
		
		stepAndCheck();
		
		if (currentToken->first != TGen::MD5::TokenArrayStart)
			throw TGen::RuntimeException("MD5::Parser::parseJointsBlock", "expecting array start");
		
		stepAndCheck();
		
		float orientX = TGen::lexical_cast<float>(currentToken->second);
		stepAndCheck();
		
		float orientY = TGen::lexical_cast<float>(currentToken->second);
		stepAndCheck();
		
		float orientZ = TGen::lexical_cast<float>(currentToken->second);
		stepAndCheck();
		
		if (currentToken->first != TGen::MD5::TokenArrayEnd)
			throw TGen::RuntimeException("MD5::Parser::parseJointsBlock", "expecting array end");
		
		TGen::MD5::FileJoint * joint = new TGen::MD5::FileJoint;
		joint->name = jointName;
		joint->parent = parentJoint;
		joint->position = TGen::Vector3(posX, posY, posZ);
		joint->orientation = TGen::Quaternion4(orientX, orientY, orientZ);
		
		file.joints.push_back(joint);
		
		if (currentToken != endIter)
			step();

		if (currentToken->first == TGen::MD5::TokenBlockEnd)
			return;
	}
	
	throw TGen::RuntimeException("MD5::Parser::parseJointsBlock", "unexpected end");	
}

void TGen::MD5::Parser::step() {
	tokens.stepToken(currentToken, endIter);
}

void TGen::MD5::Parser::checkEOS() {
	if (currentToken == endIter)
		throw TGen::RuntimeException("MapLoader::checkEOS", "unexpected end");
}

void TGen::MD5::Parser::stepAndCheck() {
	step();
	checkEOS();
}
