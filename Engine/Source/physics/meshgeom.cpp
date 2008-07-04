/*
 *  meshgeom.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/18/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "physics/meshgeom.h"
#include <iostream>
#include <tgen_math.h>
#include <tgen_core.h>
#include "ode/ode.h"

using TGen::PropertyTree;

TGen::Engine::Physics::MeshGeom::MeshGeom(const std::string & name, dSpaceID space, const TGen::PropertyTree & vertices, const TGen::PropertyTree & indices, const TGen::PropertyTree & normals)
	: TGen::Engine::Physics::Geom(name)
{
	std::cout << "verts: " << vertices.getNumProperties() << std::endl;
	std::cout << "indices: " << indices.getNumProperties() << std::endl;
	std::cout << "normals: " << normals.getNumProperties() << std::endl;
	
	if (normals.getNumProperties() > 0 && normals.getNumProperties() != vertices.getNumProperties())
		throw TGen::RuntimeException("Physics::MeshGeom", "I need one normal per vertex or no normals at all!");
	
	vertexData.reset(new StridedVertex[vertices.getNumProperties()]);
	indexData.reset(new StridedTriangle[indices.getNumProperties() / 3]);
	normalData.reset(new StridedNormal[normals.getNumProperties()]);
	
	int vertexNum = 0;
	
	std::cout << "building vertices" << std::endl;
	
	for (PropertyTree::PropertyMap::const_iterator iter = vertices.getProperties().begin(); iter != vertices.getProperties().end(); ++iter) {
		TGen::Vector3 coord = TGen::Vector3::Parse(iter->second);
	
		vertexData.get()[vertexNum].vertex[0] = coord.x;
		vertexData.get()[vertexNum].vertex[1] = coord.y;
		vertexData.get()[vertexNum].vertex[2] = coord.z;
		
		vertexNum++;
	}
	
	int normalNum = 0;
	
	for (PropertyTree::PropertyMap::const_iterator iter = normals.getProperties().begin(); iter != normals.getProperties().end(); ++iter) {
		TGen::Vector3 coord = TGen::Vector3::Parse(iter->second);
		
		normalData.get()[normalNum].normal[0] = coord.x;
		normalData.get()[normalNum].normal[1] = coord.y;
		normalData.get()[normalNum].normal[2] = coord.z;
		
		normalNum++;
	}
	
	int indexNum = 0;
	int triangleNum = 0;
	
	std::cout << "building indices" << std::endl;
	
	for (PropertyTree::PropertyMap::const_iterator iter = indices.getProperties().begin(); iter != indices.getProperties().end(); ++iter) {
		uint num = TGen::lexical_cast<uint>(iter->second);
		
		indexData.get()[triangleNum].indices[indexNum] = num - 1;
		
		indexNum++;
		
		if (indexNum >= 3) {
			indexNum = 0;
			triangleNum++;
		}
	}
	
	std::cout << "creating trimesh data" << std::endl;
	
	dTriMeshDataID meshData = dGeomTriMeshDataCreate();
	
	if (normals.getNumProperties() == 0) {
		dGeomTriMeshDataBuildSingle(meshData, vertexData.get(), sizeof(StridedVertex), vertices.getNumProperties(),
												indexData.get(), indices.getNumProperties(), sizeof(StridedTriangle));
	}
	else {
		dGeomTriMeshDataBuildSingle1(meshData, vertexData.get(), sizeof(StridedVertex), vertices.getNumProperties(),
											 indexData.get(), indices.getNumProperties(), sizeof(StridedTriangle), normalData.get());		
	}
	
	dGeomTriMeshDataPreprocess(meshData);
	
	dGeomID geomId = dCreateTriMesh(space, meshData, NULL, NULL, NULL);
	setGeomId(geomId);
	
	std::cout << "created " << dGeomTriMeshGetTriangleCount(geomId) << " triangles" << std::endl;
}

