/*
 *  arcball.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 4/27/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_ARCBALL_H
#define _TGEN_ENGINE_ARCBALL_H

#include "playercontroller.h"

namespace TGen {
	namespace Engine {
		class Entity;
		
		namespace Controller {
			class Arcball : public TGen::Engine::PlayerController {
			public:	
				Arcball(const std::string & name);
				~Arcball();
				
				void linkLocally(TGen::Engine::Entity & entity);
				void update(scalar dt);
				bool useRelativeView() const;
				
				void setControl(const std::string & controlName);
				
			private:
				TGen::Rotation dragTo(const TGen::Vector3 & end);
				
				TGen::Vector3 mapToSphere(const TGen::Vector3 & vec) const;
				TGen::Vector3 start;
				TGen::Rotation lastRot, thisRot;
				TGen::Camera * camera;
				TGen::SceneNode * node;
				std::string controlName;
			};
			
		} // !Controller
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_ARCBALL_H

