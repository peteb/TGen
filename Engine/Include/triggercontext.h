/*
 *  triggercontext.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/13/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_TRIGGERCONTEXT_H
#define _TGEN_ENGINE_TRIGGERCONTEXT_H

namespace TGen {
	namespace Engine {
		class TriggerContext {
		public:
			TriggerContext() {
				memset(registers, 0, 10 * sizeof(uint32));
			}
			
			template<typename T>
			void setRegister(int id, T value) {
				*reinterpret_cast<T *>(&registers[id]) = value;
			}
			
			template<typename T>
			T getRegister(int id) {
				return reinterpret_cast<T>(&registers[id]);
			}
			
			uint32 registers[10];
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_TRIGGERCONTEXT_H

