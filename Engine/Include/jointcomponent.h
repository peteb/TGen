/*
 *  jointcomponent.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 2/29/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_JOINTCOMPONENT_H
#define _TGEN_ENGINE_JOINTCOMPONENT_H

#include <ode/ode.h>
#include "component.h"
#include <tgen_math.h>

namespace TGen {
	namespace Engine {
		class BodyComponent;
		
		class JointComponent : public TGen::Engine::Component {
		public:
			JointComponent(const std::string & name, dJointID jointId, const std::string & attachTo);
			~JointComponent();
			
			void setAnchor(const TGen::Vector3 & anchor);
			void linkLocally(TGen::Engine::Entity & entity);
			void linkGlobally(TGen::Engine::EntityList & entities);
			
		private:
			void setSimAnchor(const TGen::Vector3 & anchor);
			
			std::string attachTo;
			
			TGen::Engine::BodyComponent * body1, * body2;
			TGen::Vector3 anchor;
			
			dJointID jointId;
		};
	}	
} // !TGen

#endif // !_TGEN_ENGINE_JOINTCOMPONENT_H
