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
#include "q3bsp/q3bsp.h"

TGen::Engine::Q3MapLoader::Q3MapLoader(TGen::Engine::StandardLogs & logs)
	: logs(logs)
{

}


TGen::Engine::Q3Map * TGen::Engine::Q3MapLoader::createMap(const std::string & name, TGen::InputStream & source, const TGen::VertexTransformer & transformer) {
	TGen::auto_ptr<TGen::Engine::Q3Map> map = new TGen::Engine::Q3Map(name);
		
	TGen::Engine::Q3BspFile bsp;
	
	loadBspFile(source, bsp);

	
	TGen::Engine::Q3MapModel * newModel = new Q3MapModel("test");

	
	typedef std::vector<TGen::Engine::Q3MapMesh::VertexDecl::Type> Batch;
	typedef std::map<std::string, Batch> BatchMap;
	BatchMap batches;
	
	for (int i = 0; i < bsp.numFaces; ++i) {
		Q3Bsp::Face * face = &bsp.faces[i];
		
		if (face->type == 3 || face->type == 1) {
			//TGen::Engine::Q3MapMesh * mesh = new Q3MapMesh;
			
			for (int a = 0; a < face->num_meshverts; ++a) {
				int fixedA = a;
				
				if (a % 3 == 0)		// TODO: matematisk formel
					fixedA = a + 2;
				else if (a % 3 == 1)
					fixedA = a;
				else
					fixedA = a - 2;
				
				int offset = bsp.meshverts[fixedA + face->meshvert].offset + face->vertex;
				Q3Bsp::Vertex * vert = &bsp.vertices[offset];
				
				TGen::Vector3 pos(vert->position[0], vert->position[1], vert->position[2]);
				transformer.transform(pos);
				
				TGen::Vector2 texcoord(vert->texcoord[0][0], vert->texcoord[0][1]);
				
				TGen::Engine::Q3MapMesh::VertexDecl::Type newVertex(pos, texcoord);

				const Q3Bsp::Texture & texture = bsp.textures[face->texture];
				batches[texture.name].push_back(newVertex);
			}
		}
	}

	std::cout << "BSP batch sizes" << std::endl;
	
	for (BatchMap::iterator iter = batches.begin(); iter != batches.end(); ++iter) {		
		const BatchMap::value_type & batch = *iter;
		
		Q3MapMesh * mesh = new Q3MapMesh(batch.first);
		
		std::cout << batch.first << " used: " << batch.second.size() << std::endl;
		
		for (int i = 0; i < batch.second.size(); ++i) {
			mesh->addVertex(batch.second[i]);
		}

		newModel->addMesh(mesh);
	}
	
	
	map->addModel(newModel);
	
	return map.release();
}

void TGen::Engine::Q3MapLoader::loadBspFile(TGen::InputStream & source, TGen::Engine::Q3BspFile & file) {
	source.read(reinterpret_cast<char *>(&file.header), sizeof(file.header));
	
	if (file.header.magic[0] != 'I' || file.header.magic[1] != 'B' || file.header.magic[2] != 'S' || file.header.magic[3] != 'P')
		throw TGen::RuntimeException("Q3MapLoader::createMap", "Invalid magic: ") << file.header.magic[0] << file.header.magic[1] << file.header.magic[2] << file.header.magic[3];
	
	logs.info["bsp3"] << "version: " << std::hex << file.header.version << TGen::endl;
	
	if (file.header.version != 0x2E)
		logs.warning["bsp3"] << "version is not quake 3!" << TGen::endl;
	
	
	
	file.numModels = file.header.entries[Q3Bsp::LumpModels].length / sizeof(Q3Bsp::Model);
	file.models = new Q3Bsp::Model[file.numModels];
	
	source.seekReadPos(file.header.entries[Q3Bsp::LumpModels].offset, TGen::beg);
	source.read(reinterpret_cast<char *>(file.models), file.header.entries[Q3Bsp::LumpModels].length);
	
	
	file.numFaces = file.header.entries[Q3Bsp::LumpFaces].length / sizeof(Q3Bsp::Face);
	file.faces = new Q3Bsp::Face[file.numFaces];
	
	source.seekReadPos(file.header.entries[Q3Bsp::LumpFaces].offset, TGen::beg);
	source.read(reinterpret_cast<char *>(file.faces), file.header.entries[Q3Bsp::LumpFaces].length);
	
	
	file.numVertices = file.header.entries[Q3Bsp::LumpVertices].length / sizeof(Q3Bsp::Vertex);
	file.vertices = new Q3Bsp::Vertex[file.numVertices];
	
	source.seekReadPos(file.header.entries[Q3Bsp::LumpVertices].offset, TGen::beg);
	source.read(reinterpret_cast<char *>(file.vertices), file.header.entries[Q3Bsp::LumpVertices].length);
	
	
	file.numMeshverts = file.header.entries[Q3Bsp::LumpMeshverts].length / sizeof(Q3Bsp::Meshvert);
	file.meshverts = new Q3Bsp::Meshvert[file.numMeshverts];
	
	source.seekReadPos(file.header.entries[Q3Bsp::LumpMeshverts].offset, TGen::beg);
	source.read(reinterpret_cast<char *>(file.meshverts), file.header.entries[Q3Bsp::LumpMeshverts].length);
	
	
	file.numTextures = file.header.entries[Q3Bsp::LumpTextures].length / sizeof(Q3Bsp::Texture);
	file.textures = new Q3Bsp::Texture[file.numTextures];
	
	source.seekReadPos(file.header.entries[Q3Bsp::LumpTextures].offset, TGen::beg);
	source.read(reinterpret_cast<char *>(file.textures), file.header.entries[Q3Bsp::LumpTextures].length);
}

