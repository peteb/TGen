/*
 *  q3maploader.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/27/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#include <tgen_core.h>

#include "q3bsp/q3maploader.h"
#include "q3bsp/q3map.h"
#include "q3bsp/q3mapmodel.h"
#include "q3bsp/q3mapmesh.h"
#include "filesystem.h"
#include "file.h"
#include "log.h"


TGen::Engine::Q3MapLoader::Q3MapLoader(TGen::Engine::StandardLogs & logs)
	: logs(logs)
{

}


TGen::Engine::Q3Map * TGen::Engine::Q3MapLoader::createMap(const std::string & name, TGen::InputStream & source, const TGen::VertexTransformer & transformer) {
	TGen::auto_ptr<TGen::Engine::Q3Map> map = new TGen::Engine::Q3Map(name);
		
	Header header;

	source.read(reinterpret_cast<char *>(&header), sizeof(header));
	
	if (header.magic[0] != 'I' || header.magic[1] != 'B' || header.magic[2] != 'S' || header.magic[3] != 'P')
		throw TGen::RuntimeException("Q3MapLoader::createMap", "Invalid magic: ") << header.magic[0] << header.magic[1] << header.magic[2] << header.magic[3];

	logs.info["bsp3"] << "version: " << std::hex << header.version << TGen::endl;

	if (header.version != 0x2E)
		logs.warning["bsp3"] << "version is not quake 3!" << TGen::endl;

	
	
	int numModels = header.entries[7].length / sizeof(Model);
	Model * models = new Model[numModels];
	
	
	source.seekReadPos(header.entries[7].offset, TGen::beg);
	source.read(reinterpret_cast<char *>(models), header.entries[7].length);
	
	std::cout << "[bsp]: " << numModels << " models: " << std::endl;
		
	for (int i = 0; i < numModels; ++i) {
		std::cout << "       " << i << ": BB min: (" << models[i].bbmin[0] << ", " << models[i].bbmin[1] << ", " << models[i].bbmin[2];
		std::cout << ") BB max: (" << models[i].bbmax[0] << ", " << models[i].bbmax[1] << ", " << models[i].bbmax[2];
		std::cout << ") face: " << models[i].face << " numfaces: " << models[i].num_faces << " brush: " << models[i].brush << " numbrushes: " << models[i].num_brushes << std::endl;
	}		
	
	
	
	
	int numFaces = header.entries[13].length / sizeof(Face);
	Face * faces = new Face[numFaces];
	
	source.seekReadPos(header.entries[13].offset, TGen::beg);
	source.read(reinterpret_cast<char *>(faces), header.entries[13].length);

	
	
	
	int numVertices = header.entries[10].length / sizeof(Vertex);
	Vertex * vertices = new Vertex[numVertices];
	
	source.seekReadPos(header.entries[10].offset, TGen::beg);
	source.read(reinterpret_cast<char *>(vertices), header.entries[10].length);
	
	
	
	//exit(33);
	
	TGen::Engine::Q3MapModel * newModel = new Q3MapModel("test");
	TGen::Engine::Q3MapMesh * newMesh = new Q3MapMesh;
	
	/*newMesh->addVertex(TGen::Engine::Q3MapMesh::VertexDecl::Type(TGen::Vector3(-10.0f, 0.0f, -10.0f)));
	newMesh->addVertex(TGen::Engine::Q3MapMesh::VertexDecl::Type(TGen::Vector3(10.0f, 0.0f, 10.0f)));
	newMesh->addVertex(TGen::Engine::Q3MapMesh::VertexDecl::Type(TGen::Vector3(-10.0f, 0.0f, 10.0f)));
	*/
	
	for (int i = 0; i < numVertices; ++i) {
		Vertex * vert = &vertices[i];
		
		TGen::Vector3 pos(vert->position[0], vert->position[1], vert->position[2]);
		transformer.transform(pos);
		
		newMesh->addVertex(TGen::Engine::Q3MapMesh::VertexDecl::Type(pos));
	}
	
	
	newModel->addMesh(newMesh);
	map->addModel(newModel);
	
	return map.release();
}
