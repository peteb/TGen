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
		class ObjectInterface {
		public:
			virtual ~ObjectInterface() {}
			
			virtual TGen::Vector3 getPosition() const abstract;
			virtual TGen::Rotation getOrientation() const abstract;			
			virtual TGen::Vector3 getVelocity() const abstract;
			virtual TGen::Matrix4x4 getSpaceTransform() const {return TGen::Matrix4x4::Identity; }
			
			virtual void setPosition(const TGen::Vector3 & pos) abstract;
			virtual void setOrientation(const TGen::Rotation & orientation) abstract;			
			
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_COMPONENTINTERFACES_H

