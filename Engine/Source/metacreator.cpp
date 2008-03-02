/*
 *  metacreator.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 2/17/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "metacreator.h"
#include <tgen_math.h>
#include <tgen_graphics.h>

void TGen::Engine::MetaCreator::writeAxes(const TGen::Matrix4x4 & coordsys, TGen::VertexStream & stream) {
	TGen::Vector3 origin = coordsys.getOrigin();
	TGen::Vector3 x = origin + coordsys.getX();
	TGen::Vector3 y = origin + coordsys.getY();
	TGen::Vector3 z = origin + coordsys.getZ();
	
	typedef TGen::JoinVertexElements2<TGen::Vertex3<float>, TGen::Color4<float> > MetaLineDecl;
	
	MetaLineDecl::Type axes[6];
	axes[0].x = origin.x;
	axes[0].y = origin.y;
	axes[0].z = origin.z;
	axes[0].r = 1.0f;
	axes[0].g = 0.0f;
	axes[0].b = 0.0f;
	axes[0].a = 1.0f;
	
	axes[1].x = x.x;
	axes[1].y = x.y;
	axes[1].z = x.z;
	axes[1].r = 1.0f;
	axes[1].g = 0.0f;
	axes[1].b = 0.0f;
	axes[1].a = 1.0f;
	
	
	axes[2].x = origin.x;
	axes[2].y = origin.y;
	axes[2].z = origin.z;
	axes[2].r = 0.0f;
	axes[2].g = 1.0f;
	axes[2].b = 0.0f;
	axes[2].a = 1.0f;
	
	axes[3].x = y.x;
	axes[3].y = y.y;
	axes[3].z = y.z;
	axes[3].r = 0.0f;
	axes[3].g = 1.0f;
	axes[3].b = 0.0f;
	axes[3].a = 1.0f;
	
	
	axes[4].x = origin.x;
	axes[4].y = origin.y;
	axes[4].z = origin.z;
	axes[4].r = 0.0f;
	axes[4].g = 0.0f;
	axes[4].b = 1.0f;
	axes[4].a = 1.0f;
	
	axes[5].x = z.x;
	axes[5].y = z.y;
	axes[5].z = z.z;
	axes[5].r = 0.0f;
	axes[5].g = 0.0f;
	axes[5].b = 1.0f;
	axes[5].a = 1.0f;
	
	for (int i = 0; i < 6; ++i)
		stream.writeVertex(&axes[i]);

}