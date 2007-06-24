/*
 *  types.h
 *  tgen
 *
 *  Created by Peter Backman on 6/6/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_TYPES_H
#define _TGEN_TYPES_H

typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef signed int int32;
typedef signed short int16;
typedef signed char int8;
typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;
typedef float scalar;

#define abstract =0

namespace TGen {
	enum FormatType {
		TypeFloat = 1,
		TypeDouble,
		TypeInt,
		TypeShort,
		TypeByte,
		TypeUnsignedInt,
		TypeUnsignedShort,
		TypeUnsignedByte,
		TypeNone,
	};
	
	int FormatTypeSize(FormatType type);
	
	template<typename T> class TGenDataType {public: };
	template<> class TGenDataType<float> {public: TGenDataType() : type(TypeFloat) {} FormatType type; };
	template<> class TGenDataType<double> {public: TGenDataType() : type(TypeDouble) {} FormatType type; };
	template<> class TGenDataType<int> {public: TGenDataType() : type(TypeInt) {} FormatType type; };
	template<> class TGenDataType<short> {public: TGenDataType() : type(TypeShort) {} FormatType type; };
	template<> class TGenDataType<unsigned char> {public: TGenDataType() : type(TypeUnsignedByte) {} FormatType type; };
	template<> class TGenDataType<char> {public: TGenDataType() : type(TypeByte) {} FormatType type; };
	template<> class TGenDataType<unsigned short> {public: TGenDataType() : type(TypeUnsignedShort) {} FormatType type; };
	template<> class TGenDataType<unsigned int> {public: TGenDataType() : type(TypeUnsignedInt) {} FormatType type; };
	template<> class TGenDataType<void> {public: TGenDataType() : type(TypeNone) {} FormatType type; };
	
} // !TGen	

#endif // !_TGEN_TYPES_H
