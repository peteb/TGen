/*
 *  registertypes.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/22/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_REGISTERTYPES_H
#define _TGEN_ENGINE_REGISTERTYPES_H

namespace TGen {
	namespace Engine {
		enum RegisterDatatype {
			TypeNULL = 0,
			TypeInt = 1,
			TypeScalar = 2,
		};
		
		
		template<typename T> class RegisterType {public: RegisterType() : type(TypeInt) {} RegisterDatatype type; };
		
		template<> class RegisterType<TGen::scalar> {public: RegisterType() : type(TypeScalar) {} RegisterDatatype type; };
		

		// undersök varför fysiken fungerar annorlunda på olika höjd på lådor, kan åka längre in
		template<typename T>
		T RegisterConvert(uint32 value, char type) {
			if (RegisterType<T>().type == type)
				return TGen::union_cast<T>(value);
			
			switch (type) {
				case TypeScalar:
					return T(TGen::union_cast<scalar>(value));
					
				default:
					return T(TGen::union_cast<int>(value));
			}
		}
		
		//template<> class RegisterType<int> {public: RegisterType() {} static TypeName type; };
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_REGISTERTYPES_H

