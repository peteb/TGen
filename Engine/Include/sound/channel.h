/*
 *  channel.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/2/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_CHANNEL_H
#define _TGEN_ENGINE_CHANNEL_H

#include <string>
#include <tgen_core.h>
#include "fmod/fmod.hpp"
#include "triggerable.h"
#include "symbols.h"

namespace TGen {
	class Vector3;
	
	namespace Engine {
		namespace Sound {
			class Channel : public TGen::Engine::Triggerable {
			public:
				Channel(FMOD::Channel * channel);
				virtual ~Channel();
				
				void trigger(TGen::Engine::TriggerContext & context, TriggerMode mode);
				
				void setLoop(bool loop);
				void set3D(bool enable);
				void setPaused(bool paused);
				bool isPlaying();
				bool isPaused();
				void setVolume(float volume);
				
				void set3DAttributes(const TGen::Vector3 & position, const TGen::Vector3 & velocity);
				void set3DMinMaxDistance(scalar minDistance, scalar maxDistance);
				
				void reset();
				void setUpdateVelocity(bool noVelocity);
				
			private:
				FMOD::Channel * channel;
				bool updateVelocity;
				
				static TGen::Engine::Symbol symbolsSetUpdateVelocity;
 			};
			
		} // !Sound
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_CHANNEL_H

