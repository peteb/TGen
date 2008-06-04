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

namespace TGen {
	namespace Engine {
		namespace Sound {
			class Channel {
			public:
				Channel(const std::string & resource);
				
			private:
				std::string resourceName;
				
			};
			
		} // !Sound
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_CHANNEL_H

