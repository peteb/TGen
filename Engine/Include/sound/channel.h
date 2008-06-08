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
#include "fmod/fmod.hpp"

namespace TGen {
	namespace Engine {
		namespace Sound {
			class Channel {
			public:
				Channel(FMOD::Channel * channel);
				virtual ~Channel();
				
				void setLoop(bool loop);
				void reset();
				
			private:
				FMOD::Channel * channel;
 			};
			
		} // !Sound
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_CHANNEL_H

