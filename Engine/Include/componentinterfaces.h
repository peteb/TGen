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
		class WorldObject {
		public:
			virtual ~WorldObject() {}
			
			virtual TGen::Vector3 getPosition() const abstract;
			virtual TGen::Rotation getOrientation() const abstract;			
			virtual TGen::Vector3 getVelocity() const abstract;
			
			virtual void setPosition(const TGen::Vector3 & pos) abstract;
			virtual void setOrientation(const TGen::Rotation & orientation) abstract;			
			
		};
		
		class WeaponInterface {
		public:
			virtual ~WeaponInterface() {}
			
			virtual void beginFire(int mode) abstract;
			virtual void endFire(int mode) abstract;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_COMPONENTINTERFACES_H

