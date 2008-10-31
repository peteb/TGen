/*
 *  devicecollection.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 1/7/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_DEVICECOLLECTION_H
#define _TGEN_ENGINE_DEVICECOLLECTION_H

#include <vector>
#include "inputdevice.h"

namespace TGen {	
	namespace Engine {
		class InputEventResponder;
		class TextOutputer;
		
		class DeviceCollection {
		public:
			DeviceCollection();
			~DeviceCollection();
			
			void addDevice(TGen::Engine::InputDevice * device);
			void dispatchEvents(TGen::Engine::InputEventResponder & responder);
			void printDevices(TGen::Engine::TextOutputer & out);
			void enterMode(TGen::Engine::InputDeviceMode mode);
			void reset();
			
		private:
			typedef std::vector<TGen::Engine::InputDevice *> DeviceList;
			
			DeviceList devices;
			TGen::Engine::InputDeviceMode mode;
		};
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_DEVICECOLLECTION_H
