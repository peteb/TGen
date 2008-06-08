/*
 *  convert.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/8/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_CONVERT_H
#define _TGEN_ENGINE_CONVERT_H

#include "fmod/fmod.hpp"

namespace TGen {
	class Vector3;
	
	namespace Engine {
		namespace Sound {
			void TVecToFVec(const TGen::Vector3 & inParam, FMOD_VECTOR & outParam);
			
		} // !Sound
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_CONVERT_H

