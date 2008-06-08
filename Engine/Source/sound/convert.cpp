/*
 *  convert.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/8/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "sound/convert.h"
#include <tgen_math.h>

void TGen::Engine::Sound::TVecToFVec(const TGen::Vector3 & inParam, FMOD_VECTOR & outParam) {
	outParam.x = inParam.x;
	outParam.y = inParam.y;
	outParam.z = inParam.z;
}
