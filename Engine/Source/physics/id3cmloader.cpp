/*
 *  id3cmloader.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/15/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#include "physics/id3cmloader.h"
#include "physics/id3cmgeom.h"

#include "q3bsp/q3bsp.h"
#include "q3bsp/q3maploader.h"

#include "filesystem.h"
#include "file.h"
#include "brush.h"

#include <tgen_core.h>
#include <tgen_math.h>
#include <tgen_renderer.h>

TGen::Engine::Physics::Id3CMLoader::Id3CMLoader(TGen::Engine::Filesystem & filesystem)
	: filesystem(filesystem)
{
}

TGen::Engine::Physics::Id3CMGeom * TGen::Engine::Physics::Id3CMLoader::createGeom(const std::string & name, const std::string & path, const TGen::VertexTransformer & transformer, dSpaceID space) {
	TGen::Engine::Q3BspFile bsp;
	
	TGen::auto_ptr<TGen::Engine::File> file = filesystem.openRead(path);
	TGen::Engine::Q3MapLoader::LoadBspFile(file.deref(), bsp);

	std::auto_ptr<TGen::Engine::Physics::Id3CMGeom> newModel(new TGen::Engine::Physics::Id3CMGeom(name));
	
	
	
	for (int i = 0; i < bsp.numModels; ++i) {
		const Q3Bsp::Model & q3Model = bsp.models[i];
		
		for (int i = q3Model.brush; i < q3Model.brush + q3Model.num_brushes; ++i) {
			const Q3Bsp::Brush & q3Brush = bsp.brushes[i];
			
			//Q3MapMesh * mesh = new Q3MapMesh(bsp.textures[q3Brush.texture].name);
			if (i == 440) {
				std::cout << "BOE" << std::endl;
			}
			
			std::cout << "Brush texture: " << bsp.textures[q3Brush.texture].name << " flags: " << bsp.textures[q3Brush.texture].flags << " content: " << bsp.textures[q3Brush.texture].contents << std::endl;
			
			
			if (std::string(bsp.textures[q3Brush.texture].name) == "textures/common/trigger") {
				std::cout << "TJA" << std::endl;
				
				for (int i = 0; i < q3Brush.n_brushsides; ++i) {
					const Q3Bsp::Brushside & q3Brushside = bsp.brushsides[q3Brush.brushside + i];

					std::cout << "SIDE texture: " << bsp.textures[q3Brushside.texture].name << " flags: " << bsp.textures[q3Brushside.texture].flags << " content: " << bsp.textures[q3Brushside.texture].contents << std::endl;
				}
			}
			
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
				std::vector<TGen::Vector3> sideTris = TGen::Engine::TriangulateBrushside(brushPlanes, i, false);
				
				StridedTriangle tri;
				int triPart = 0;
				
				for (int i = 0; i < sideTris.size(); ++i) {					
					StridedVertex vert;
					vert.vertex[0] = sideTris[i].x;
					vert.vertex[1] = sideTris[i].y;
					vert.vertex[2] = sideTris[i].z;
					
					newModel->vertices.push_back(vert);
					newModel->indices.push_back(newModel->vertices.size() - 1);

				}
			}		
			
		}
	}	
	
	
	dTriMeshDataID meshData = dGeomTriMeshDataCreate();
	
	
	//dGeomTriMeshDataBuildSingle(meshData, &newModel->vertexData[0], sizeof(StridedVertex), newModel->vertexData.size(),
	//									 &newModel->indexData[0], newModel->indexData.size() * 3, sizeof(StridedTriangle));
	
	dGeomTriMeshDataBuildSimple(meshData, (const dReal *)&newModel->vertices[0], newModel->vertices.size(), 
										 (const dTriIndex *)&newModel->indices[0], newModel->indices.size());
	
	dGeomTriMeshDataPreprocess(meshData);
	
	dGeomID geomId = dCreateTriMesh(space, meshData, NULL, NULL, NULL);
	
	newModel->setGeom(geomId);
	
	
	return newModel.release();
}


