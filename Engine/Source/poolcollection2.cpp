/*
 *  poolcollection2.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/27/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "poolcollection2.h"

TGen::Engine::PoolCollection2::PoolCollection2(TGen::Engine::VertexCache2 & creator, TGen::VertexDataSource & dataSource, const TGen::VertexStructure & vertstruct, ushort usage)
	: creator(creator)
	, dataSource(dataSource)
	, vertexStructure(vertstruct)
	, usage(usage)
{
}

