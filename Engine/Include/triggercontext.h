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
		enum SpecialRegisters {
			RegisterSelf = 100,
		};
		
		class TriggerContext {
		public:
			TriggerContext() {
				memset(registers, 0, 10 * sizeof(uint32));
				selfPointer = 0;
				numParameters = 0;
			}
			
			template<typename T>
			void setRegister(int id, T value) {
				switch (id) {
					case RegisterSelf:
						*reinterpret_cast<T *>(&selfPointer) = value;
						break;
						
					default:
						*reinterpret_cast<T *>(&registers[id]) = value;
				}
			}
			
			template<typename T>
			T getRegister(int id) {
				switch (id) {
					case RegisterSelf:
						return reinterpret_cast<T>(&selfPointer);
				}
				
				return reinterpret_cast<T>(&registers[id]);
			}
			
			int getFunctionSymbol() {
				return int(registers[0]);
			}
			
			int getReturnRegister() {
				return int(registers[1]);
			}
			
			uint32 registers[10];
			uint32 selfPointer;
			int numParameters;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_TRIGGERCONTEXT_H

