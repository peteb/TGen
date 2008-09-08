/*
 *  worldobject.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/30/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_WORLDOBJECT_H
#define _TGEN_ENGINE_WORLDOBJECT_H

#include "triggerable.h"
#include "symbols.h"

#include <tgen_math.h>

namespace TGen {
	namespace Engine {
		class WorldObject : public TGen::Engine::Triggerable {
		public:
			virtual ~WorldObject() {}
			
			virtual TGen::Vector3 getPosition() const abstract;
			virtual TGen::Rotation getOrientation() const abstract;			
			virtual TGen::Vector3 getVelocity() const abstract;
			
			virtual void setPosition(const TGen::Vector3 & pos) abstract;
			virtual void setOrientation(const TGen::Rotation & orientation) abstract;	
			
			void trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode);
			
		private:
			static TGen::Engine::Symbol symbolTranslateTo;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_WORLDOBJECT_H

