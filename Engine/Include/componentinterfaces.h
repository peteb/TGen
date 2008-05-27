/*
 *  componentinterfaces.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/26/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_COMPONENTINTERFACES_H
#define _TGEN_ENGINE_COMPONENTINTERFACES_H

#include <tgen_math.h>

namespace TGen {
	namespace Engine {
		class PosOrientInterface {
		public:
			virtual ~PosOrientInterface() {}
			
			virtual TGen::Vector3 getPosition() const abstract;
			virtual TGen::Rotation getOrientation() const abstract;
		};

		
		class VelocityInterface {
		public:
			virtual ~VelocityInterface() {}
			
			virtual TGen::Vector3 getVelocity() const abstract;
		};
		
		class ObjectInterface : public PosOrientInterface, public VelocityInterface {
		public:
			virtual ~ObjectInterface() {}
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_COMPONENTINTERFACES_H

