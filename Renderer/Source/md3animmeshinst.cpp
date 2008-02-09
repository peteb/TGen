/*
 *  md3animmeshinst.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 2/8/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "md3animmeshinst.h"

TGen::MD3::AnimatingMeshInstance::AnimatingMeshInstance(const std::string & materialName, 
																		  const std::string & materialNamePostfix, 
																		  TGen::MD3::AnimatingMesh const & base)
	: TGen::NewMeshInstance(materialName, materialNamePostfix)
	, base(base)
{

}

TGen::MD3::AnimatingMeshInstance::~AnimatingMeshInstance() {
	
}
