/*
 *  inputeventresponder.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 1/7/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_INPUTEVENTRESPONDER_H
#define _TGEN_ENGINE_INPUTEVENTRESPONDER_H

#include <string>

namespace TGen {
	class Vector3;
	
	namespace Engine {
		class InputDevice;
		
		enum StateEvent {
			StateDown = 1,
			StateUp,
			StatePressing,			
		};
		
		class InputEventResponder {
		public:
			virtual ~InputEventResponder() {}
			
			virtual void onBinaryEvent(TGen::Engine::InputDevice & device, int id, TGen::Engine::StateEvent state) {}
			virtual void onTextEvent(TGen::Engine::InputDevice & device, const std::string & text) {}
			virtual void onVectorEvent(TGen::Engine::InputDevice & device, int id, const TGen::Vector3 & vector) {}
		};
		
	} // !Engine	
} // !TGen

#endif // !_TGEN_ENGINE_INPUTEVENTRESPONDER_H
