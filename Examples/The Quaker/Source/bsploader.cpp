/*
 *  bsploader.cpp
 *  The Quaker
 *
 *  Created by Peter Backman on 7/21/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "bsploader.h"
#include <fstream>

BSPLoader::BSPLoader()
	: textures(NULL)
	, numTextures(0)
	, faces(NULL)
	, numFaces(0)
	, vertices(NULL)
	, numVertices(0)
	, meshverts(NULL)
	, numMeshverts(0)
{
}

BSPLoader::~BSPLoader() {
	delete meshverts;
	delete vertices;
	delete faces;
	delete textures;
}

void BSPLoader::Parse(std::ifstream & file) {
	ReadHeader(file);
	ReadTextures(file);
	ReadFaces(file);
	ReadVertices(file);
	ReadMeshverts(file);
}

void BSPLoader::ReadHeader(std::ifstream & file) {
	if (!file.read(reinterpret_cast<char *>(&header), sizeof(Header)))
		throw TGen::RuntimeException("BSPLoader::ReadHeader", "failed to read header");
	
	if (header.magic[0] != 'I' || header.magic[1] != 'B' || header.magic[2] != 'S' || header.magic[3] != 'P')
		throw TGen::RuntimeException("BSPLoader::ReadHeader", "Invalid magic: ") << header.magic[0] << header.magic[1] << header.magic[2] << header.magic[3];

	if (header.version != 0x2E)
		std::cout << "[bsp]: *** WARNING *** version is not quake 3!" << std::endl;
}

void BSPLoader::ReadTextures(std::ifstream & file) {
	numTextures = header.entries[BSPLumpTextures].length / sizeof(Texture);
	
	delete textures;
	textures = new Texture[numTextures];
	
	file.seekg(header.entries[BSPLumpTextures].offset, std::ios::beg);
	if (!file.read(reinterpret_cast<char *>(textures), header.entries[BSPLumpTextures].length))
		throw TGen::RuntimeException("BSPLoader::ReadTextures", "failed to read textures");
	
	materialDeps.clear();
	
	for (int i = 0; i < numTextures; ++i) {
		std::cout << "[bsp]: " << textures[i].name << std::endl;
		materialDeps.push_back(textures[i].name);
	}
	
	std::cout << "[bsp]: textures: " << numTextures << std::endl;
}

void BSPLoader::ReadFaces(std::ifstream & file) {
	numFaces = header.entries[BSPLumpFaces].length / sizeof(Face);

	delete faces;
	faces = new Face[numFaces];
	
	file.seekg(header.entries[BSPLumpFaces].offset, std::ios::beg);
	if (!file.read(reinterpret_cast<char *>(faces), header.entries[BSPLumpFaces].length))
		throw TGen::RuntimeException("BSPLoader::ReadFaces", "failed to read faces");
	
	std::cout << "[bsp]: faces: " << numFaces << std::endl;
}

void BSPLoader::ReadVertices(std::ifstream & file) {
	numVertices = header.entries[BSPLumpVertices].length / sizeof(Vertex);
	
	delete vertices;
	vertices = new Vertex[numVertices];
	
	file.seekg(header.entries[BSPLumpVertices].offset, std::ios::beg);
	if (!file.read(reinterpret_cast<char *>(vertices), header.entries[BSPLumpVertices].length))
		throw TGen::RuntimeException("BSPLoader::ReadVertices", "failed to read vertices");
	
	std::cout << "[bsp]: vertices: " << numVertices << std::endl;	
}

void BSPLoader::ReadMeshverts(std::ifstream & file) {
	numMeshverts = header.entries[BSPLumpMeshverts].length / sizeof(Meshvert);
	
	delete meshverts;
	meshverts = new Meshvert[numMeshverts];
	
	file.seekg(header.entries[BSPLumpMeshverts].offset, std::ios::beg);
	if (!file.read(reinterpret_cast<char *>(meshverts), header.entries[BSPLumpMeshverts].length))
		throw TGen::RuntimeException("BSPLoader::ReadMeshverts", "failed to read meshverts");
	
	std::cout << "[bsp]: meshverts: " << numMeshverts << std::endl;		
}

const BSPLoader::StringList & BSPLoader::getTextures() const {
	return materialDeps;
}

BSPTree * BSPLoader::CreateTree(TGen::Renderer & renderer, SurfaceLinker & linker) {
	BSPTree * tree = new BSPTree("bsp");
	
	delete tree->vb;
	tree->vb = renderer.CreateVertexBuffer(MyVertex(), sizeof(MyVertex::Type) * numVertices, TGen::UsageStatic);
	
	
	TGen::Vector3 max, min;
	for (int i = 0; i < numVertices; ++i) {
		vertices[i].position[0] /= 3.0f;
		vertices[i].position[1] /= 3.0f;
		vertices[i].position[2] /= 3.0f;
		
		std::swap(vertices[i].position[1], vertices[i].position[2]);
		vertices[i].position[1] = -vertices[i].position[1];
		
		if (vertices[i].position[0] > max.x)
			max.x = vertices[i].position[0];
		if (vertices[i].position[1] > max.y)
			max.y = vertices[i].position[1];
		if (vertices[i].position[2] > max.z)
			max.z = vertices[i].position[2];
		
	}

	int maxo = 0;
	int ones = 0;
	for (int i = 0; i < numMeshverts; ++i) {
		if (meshverts[i].offset == 3)
			ones++;
		
		if (meshverts[i].offset > maxo)
			maxo = meshverts[i].offset;
	}
	
	std::cout << "MAXO: " << maxo << " ONES: " << ones << std::endl;
	
	tree->vb->BufferData(vertices, sizeof(Vertex) * numVertices, NULL);

	
	std::cout << "max: " << std::string(max) << std::endl;
	
	//tree->ib = renderer.CreateIndexBuffer(MyIndex(), sizeof(MyIndex::Type) * numMeshverts, TGen::UsageStatic);
	//tree->ib->BufferData(meshverts, sizeof(Meshvert) * numMeshverts, NULL);
	
	std::cout << "bsp vertex: " << sizeof(Vertex) << " tgen: " << sizeof(MyVertex::Type) << std::endl;
	std::cout << "bsp index: " << sizeof(Meshvert) << " tgen: " << sizeof(MyIndex::Type) << std::endl;
	
	
	for (int i = 0; i < numFaces; ++i) {
		Face * currentFace = &faces[i];
		
		if (currentFace->type == BSPFaceMesh || currentFace->type == BSPFacePolygon) {
			//std::cout << "mesh" << std::endl;
			BSPGeometry * newGeom = new BSPGeometry(*tree, currentFace->type == BSPFacePolygon);
		
			int sizze = currentFace->num_meshvertices;
			
			newGeom->startIndex = 0;
			newGeom->numIndices = sizze;
			newGeom->ib = renderer.CreateIndexBuffer(MyIndex(), sizeof(MyIndex::Type) * sizze, TGen::UsageStatic);
			
			MyIndex::Type * data = new MyIndex::Type[sizze];
			
			for (int a = 0; a < sizze; ++a) {
				data[a] = meshverts[currentFace->meshvert + a].offset + currentFace->vertex;
			}
			
			newGeom->ib->BufferData(data, sizeof(MyIndex::Type) * sizze, NULL);
			delete data;
			
			tree->AddSurface(Surface(linker.getMaterial(textures[currentFace->texture].name), newGeom));
		}
	}
	
	return tree;	
}
