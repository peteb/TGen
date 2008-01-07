/*
 *  inputdevice.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 1/7/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_INPUTDEVICE_H
#define _TGEN_ENGINE_INPUTDEVICE_H

#include <tgen_core.h>

namespace TGen {
	namespace Engine {		
		class InputEventResponder;
		
		struct DeviceControls {
			DeviceControls();
			
			int numAxes;
			int numButtons;
			int numBalls;
			int numHats;
		};
		
		class InputDevice {
		public:
			InputDevice(const std::string & name, int id);
			virtual ~InputDevice();
			
			virtual void dispatchEvents(TGen::Engine::InputEventResponder & responder) abstract;
			virtual std::string getDeviceName() abstract;
			const DeviceControls & getControls() const;
			std::string getName() const;
			int getId() const;
			
		protected:
			std::string name;
			int id;
			DeviceControls controls;
		};
		
	} // !Engine	
} // !TGen

#endif // !_TGEN_ENGINE_INPUTDEVICE_H
