/*
 *  types.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/19/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "types.h"
#include "error.h"

int TGen::FormatTypeSize(FormatType type) {
	switch (type) {
		case TGen::TypeFloat:
			return sizeof(float);
			
		case TGen::TypeDouble:
			return sizeof(double);
			
		case TGen::TypeInt:
			return sizeof(int32);
			
		case TGen::TypeDoubleInt:
			return sizeof(int64);
			
		case TGen::TypeShort:
			return sizeof(int16);
			
		case TGen::TypeByte:
			return sizeof(int8);
			
		case TGen::TypeUnsignedDoubleInt:
			return sizeof(uint64);
			
		case TGen::TypeUnsignedInt:
			return sizeof(uint32);
			
		case TGen::TypeUnsignedShort:
			return sizeof(uint16);
			
		case TGen::TypeUnsignedByte:
			return sizeof(uint8);
			
		case TGen::TypeNone:
			return 0;		
			
		case TGen::TypeBool:
			return sizeof(bool);
	}
	
	throw TGen::RuntimeException("FormatTypeSize", "invalid type");
}
