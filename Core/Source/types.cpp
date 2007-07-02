/*
 *  types.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/19/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "types.h"

int TGen::FormatTypeSize(FormatType type) {
	switch (type) {
		case TGen::TypeFloat:
			return sizeof(float);
			
		case TGen::TypeDouble:
			return sizeof(double);
			
		case TGen::TypeInt:
			return sizeof(int);
			
		case TGen::TypeShort:
			return sizeof(short);
			
		case TGen::TypeByte:
			return sizeof(char);
			
		case TGen::TypeUnsignedInt:
			return sizeof(unsigned int);
			
		case TGen::TypeUnsignedShort:
			return sizeof(unsigned short);
			
		case TGen::TypeUnsignedByte:
			return sizeof(unsigned char);
			
		case TGen::TypeNone:
			return 0;		
	}
	
	return 0;
}
