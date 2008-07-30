/*
 *  joint.h
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
		namespace Physics {
			class Body;
		
			class Joint : public TGen::Engine::Component {
			public:
				Joint(const std::string & name, dJointID jointId);
				~Joint();
				
				void setLink1(const std::string & link1Name);
				void setLink2(const std::string & link2Name);

				void setAnchor(const TGen::Vector3 & anchor);
				void link(const TGen::Engine::ComponentLinker & linker);
				
			private:
				void setSimAnchor(const TGen::Vector3 & anchor);
				
				std::string link1Name, link2Name;
				
				TGen::Engine::Physics::Body * body1, * body2;
				TGen::Vector3 anchor;
				
				dJointID jointId;
			};
		} // !Physics
	}	
} // !TGen

#endif // !_TGEN_ENGINE_JOINTCOMPONENT_H
