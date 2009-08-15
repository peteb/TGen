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
#include "brush.h"

TGen::Engine::Q3MapLoader::Q3MapLoader(TGen::Engine::StandardLogs & logs)
	: logs(logs)
{

}


TGen::Engine::Q3Map * TGen::Engine::Q3MapLoader::createMap(const std::string & name, TGen::InputStream & source, const TGen::VertexTransformer & transformer) {
	TGen::auto_ptr<TGen::Engine::Q3Map> map = new TGen::Engine::Q3Map(name);
		
	TGen::Engine::Q3BspFile bsp;
	
	loadBspFile(source, bsp);

	
	TGen::auto_ptr<TGen::Engine::Q3MapModel> newModel = new Q3MapModel(name);

	
	
	
	
	
	for (int i = 0; i < bsp.numBrushes; ++i) {
		
		const Q3Bsp::Brush & q3Brush = bsp.brushes[i];

		Q3MapMesh * mesh = new Q3MapMesh(bsp.textures[q3Brush.texture].name);

		std::vector<TGen::Plane3> brushPlanes;
		
		for (int i = 0; i < q3Brush.n_brushsides; ++i) {
			const Q3Bsp::Brushside & q3Brushside = bsp.brushsides[q3Brush.brushside + i];
			const Q3Bsp::Plane & q3Plane = bsp.planes[q3Brushside.plane];
			
			TGen::Plane3 newPlane(TGen::Vector3(q3Plane.normal[0], q3Plane.normal[1], q3Plane.normal[2]), q3Plane.dist);
			transformer.transform(newPlane);
			
			brushPlanes.push_back(newPlane);
		}
		
		// brushPlanes = fixed planes
		
		for (int i = 0; i < brushPlanes.size(); ++i) {
			std::vector<TGen::Vector3> sideTris = TGen::Engine::TriangulateBrushside(brushPlanes, i);
			
			for (int i = 0; i < sideTris.size(); ++i) {
				Q3MapMesh::VertexDecl::Type vertex(sideTris[i], TGen::Vector2(sideTris[i].x, sideTris[i].y));
				mesh->addVertex(vertex);
			}
		}

		newModel->addMesh(mesh);

	}
	
	

	
	
	
	
	
	
	/*typedef std::vector<TGen::Engine::Q3MapMesh::VertexDecl::Type> Batch;
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
	}*/
	
	
	
	map->addModel(newModel.release());
	
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

	
	file.numBrushes = file.header.entries[Q3Bsp::LumpBrushes].length / sizeof(Q3Bsp::Brush);
	file.brushes = new Q3Bsp::Brush[file.numBrushes];
	
	source.seekReadPos(file.header.entries[Q3Bsp::LumpBrushes].offset, TGen::beg);
	source.read(reinterpret_cast<char *>(file.brushes), file.header.entries[Q3Bsp::LumpBrushes].length);

	
	file.numBrushsides = file.header.entries[Q3Bsp::LumpBrushsides].length / sizeof(Q3Bsp::Brushside);
	file.brushsides = new Q3Bsp::Brushside[file.numBrushsides];
	
	source.seekReadPos(file.header.entries[Q3Bsp::LumpBrushsides].offset, TGen::beg);
	source.read(reinterpret_cast<char *>(file.brushsides), file.header.entries[Q3Bsp::LumpBrushsides].length);

	
	file.numPlanes = file.header.entries[Q3Bsp::LumpPlanes].length / sizeof(Q3Bsp::Plane);
	file.planes = new Q3Bsp::Plane[file.numPlanes];
	
	source.seekReadPos(file.header.entries[Q3Bsp::LumpPlanes].offset, TGen::beg);
	source.read(reinterpret_cast<char *>(file.planes), file.header.entries[Q3Bsp::LumpPlanes].length);
	
	
	
	file.numTextures = file.header.entries[Q3Bsp::LumpTextures].length / sizeof(Q3Bsp::Texture);
	file.textures = new Q3Bsp::Texture[file.numTextures];
	
	source.seekReadPos(file.header.entries[Q3Bsp::LumpTextures].offset, TGen::beg);
	source.read(reinterpret_cast<char *>(file.textures), file.header.entries[Q3Bsp::LumpTextures].length);
}

