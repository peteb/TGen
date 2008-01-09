/*
 *  gameinputmapper.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 1/9/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_GAMEINPUTMAPPER_H
#define _TGEN_ENGINE_GAMEINPUTMAPPER_H

#include "inputeventresponder.h"

namespace TGen {
	namespace Engine {
		class GameInputMapper : public TGen::Engine::InputEventResponder {
		public:
			GameInputMapper();
			~GameInputMapper();
			
			void onBinaryEvent(TGen::Engine::InputDevice & device, int id, TGen::Engine::StateEvent state);
			void onTextEvent(TGen::Engine::InputDevice & device, const std::string & text);
			void onVectorEvent(TGen::Engine::InputDevice & device, int id, const TGen::Vector3 & vector);
			
		private:
			std::string text;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_GAMEINPUTMAPPER_H
