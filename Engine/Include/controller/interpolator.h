/*
 *  interpolator.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/3/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_CONTROLLER_INTERPOLATOR_H
#define _TGEN_ENGINE_CONTROLLER_INTERPOLATOR_H

#include "component.h"
#include <tgen_core.h>
#include <tgen_math.h>

namespace TGen {
	namespace Engine {
		class WorldObject;
		
		namespace Controller {
			class InterpolatorScript;
			
			// TODO: move this to controllers.... this is actually a frakking controller!
			
			class Interpolator : public TGen::Engine::Component {
			public:	
				Interpolator(const std::string & name);
				virtual ~Interpolator();
				
				void link(const TGen::Engine::ComponentLinker & linker);
				void update(scalar dt);
				void setPositionDelegate(const std::string & delegateName);
				void addKeypoint(const TGen::Vector3 & point);
				void setSpeed(float speed);
				void setScriptInterface(TGen::Engine::Controller::InterpolatorScript * scriptInterface);
				void setEnabled(bool enabled);
				bool getEnabled() const;
				
			private:
				TGen::Engine::Controller::InterpolatorScript * scriptInterface;

				TGen::Engine::WorldObject * positionDelegate;
				std::string delegateName;
				float time, speed;
				bool enabled;
				
				std::vector<TGen::Vector3> keypoints;
			};
			
		} // !Controller
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_CONTROLLER_INTERPOLATOR_H

