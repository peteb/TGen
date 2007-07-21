/*
 *  scenenoderenderer.cpp
 *  The Quaker
 *
 *  Created by Peter Backman on 7/19/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "scenenoderenderer.h"
#include "renderlist.h"

AABBRenderer::AABBRenderer(TGen::Batch<TGen::Vertex3<float>, 2> & batch, Camera & camera)
	: batch(batch)
	, camera(camera)
{
	
}

void AABBRenderer::Visit(SceneNode & node) {
	TGen::Vertex3<float>::Type data[2];
	TGen::Vector3 corners[8];
	node.getWorldAABB().getCorners(corners);

	// bottom
	data[0] = corners[0];
	data[1] = corners[1];
	batch.WritePrimitive(data);
	
	data[0] = corners[1];
	data[1] = corners[2];
	batch.WritePrimitive(data);
	
	data[0] = corners[2];
	data[1] = corners[3];
	batch.WritePrimitive(data);
	
	data[0] = corners[3];
	data[1] = corners[0];
	batch.WritePrimitive(data);
	
	// TOP
	data[0] = corners[4];
	data[1] = corners[5];
	batch.WritePrimitive(data);

	data[0] = corners[5];
	data[1] = corners[6];
	batch.WritePrimitive(data);
	
	data[0] = corners[6];
	data[1] = corners[7];
	batch.WritePrimitive(data);
	
	data[0] = corners[7];
	data[1] = corners[4];
	batch.WritePrimitive(data);

	// SIDES
		
	data[0] = corners[0];
	data[1] = corners[4];
	batch.WritePrimitive(data);

	data[0] = corners[1];
	data[1] = corners[5];
	batch.WritePrimitive(data);

	data[0] = corners[2];
	data[1] = corners[6];
	batch.WritePrimitive(data);

	data[0] = corners[3];
	data[1] = corners[7];
	batch.WritePrimitive(data);
	
	data[0] = node.getTransform().getOrigin();
	data[1] = node.getTransform() * TGen::Vector3(0.0f, 0.0f, 1.0f);
	batch.WritePrimitive(data);
	
	//for (int i = 0; i < 8; ++i)
	//	std::cout << std::string(corners[i]) << std::endl;

}

SceneNodeRenderer::SceneNodeRenderer(RenderList & list, Camera & camera, int maxSurfaces)
	: list(list)
	, camera(camera)
	, maxSurfaces(maxSurfaces)
{
}

void SceneNodeRenderer::Visit(SceneNode & node) {
	for (int i = 0; i < node.surfaces.size() && i < maxSurfaces; ++i)
		list.AddSurface(&node.surfaces[i]);
}

