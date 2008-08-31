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

#include "registertypes.h"

namespace TGen {
	namespace Engine {
		enum SpecialRegisters {
			RegisterParameters = 3,
			RegisterSelf = 100,
			RegisterObject = 2,
			ReservedParameters = 4,
			MaxRegisters = 15,
		};
		

		class TriggerContext {
		public:
			TriggerContext() {
				memset(registers, 0, MaxRegisters * sizeof(uint32));
				memset(registerTypes, 0, MaxRegisters * sizeof(char));

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
						registerTypes[id] = TGen::Engine::RegisterType<T>().type;
				
				}
				
			}
			
			template<typename T>
			void setReturn(T value) {
				setRegister<T>(getReturnRegister(), value);
			}
			
			void invalidateCall() {
				setRegister<int>(0, -1);	
			}
						
			template<typename T>
			T getRegister(int id) {
				switch (id) {
					case RegisterSelf:
						return (T)selfPointer;
				}
				
				//return (T)registers[id];
				//TGenAssert(registerTypes[id]);
				
				return RegisterConvert<T>(registers[id], registerTypes[id]);
			}
			
			template<typename T>
			T getRegisterPtr(int id) {
				return (T)registers[id];
			}
			
			template<typename T>
			T * getRegisterReference(int id) {
				/*RegisterDatatype type = RegisterType<T>().type;
				
				if (type != registerTypes[id]) {
					registers[id] = RegisterConvert<T>(registers[id], registerTypes[id]);
					registerTypes[id] = type;
				}*/
				
				return reinterpret_cast<T *>(&registers[id]);
			}
			
			template<typename T>
			T getParameter(int id) {
				return getRegister<T>(id + TGen::Engine::RegisterParameters);
			}
			
			template<typename T>
			T getParameterPtr(int id) {
				return getRegisterPtr<T>(id + TGen::Engine::RegisterParameters);				
			}
			
			template<typename T>
			void setParameter(int id, T value) {
				setRegister(id + TGen::Engine::RegisterParameters, value);
			}
			
			int getFunctionSymbol() {
				return int(registers[0]);
			}
			
			int getReturnRegister() {
				return int(registers[1]);
			}
			
						
			uint32 registers[MaxRegisters];
			char registerTypes[MaxRegisters];
			uint32 selfPointer;
			int numParameters;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_TRIGGERCONTEXT_H

