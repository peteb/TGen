/*
 *  types.h
 *  tgen
 *
 *  Created by Peter Backman on 6/6/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_CORE_TYPES_H
#define _TGEN_CORE_TYPES_H

#define abstract =0
#define PACKED __attribute__((packed))
#define nullptr 0

#ifdef __GNUG__
#define __nothrow __attribute__((__nothrow__))
#define __noinline __attribute__((__noinline__))
#define __puref __attribute__((const)
#endif

namespace TGen {
	typedef unsigned long long uint64;
	typedef unsigned int uint32;
	typedef unsigned short uint16;
	typedef unsigned char uint8;
	typedef signed int int32;
	typedef signed long long int64;
	typedef signed short int16;
	typedef signed char int8;
	typedef unsigned int uint;
	typedef unsigned short ushort;
	typedef unsigned char uchar;
	typedef float scalar;
	
	template<typename T>
	void OffsetPointer(T & pointer, void * base) {
		pointer = reinterpret_cast<T>(reinterpret_cast<uint8 *>(pointer) + uint(base));
	}
	
	enum FormatType {
		TypeFloat = 1,
		TypeDouble,
		TypeDoubleInt,
		TypeInt,
		TypeShort,
		TypeByte,
		TypeUnsignedDoubleInt,
		TypeUnsignedInt,
		TypeUnsignedShort,
		TypeUnsignedByte,
		TypeBool,
		TypeNone,
		TypeEnd = 0xFF,
	};
	
	int FormatTypeSize(FormatType type);
	
	template<typename T> class TGenDataType {public: };
	template<> class TGenDataType<float> {public: TGenDataType() : type(TypeFloat) {} FormatType type; };
	template<> class TGenDataType<double> {public: TGenDataType() : type(TypeDouble) {} FormatType type; };
	template<> class TGenDataType<int> {public: TGenDataType() : type(TypeInt) {} FormatType type; };
	template<> class TGenDataType<long long> {public: TGenDataType() : type(TypeDoubleInt) {} FormatType type; };
	template<> class TGenDataType<short> {public: TGenDataType() : type(TypeShort) {} FormatType type; };
	template<> class TGenDataType<unsigned char> {public: TGenDataType() : type(TypeUnsignedByte) {} FormatType type; };
	template<> class TGenDataType<char> {public: TGenDataType() : type(TypeByte) {} FormatType type; };
	template<> class TGenDataType<unsigned short> {public: TGenDataType() : type(TypeUnsignedShort) {} FormatType type; };
	template<> class TGenDataType<unsigned int> {public: TGenDataType() : type(TypeUnsignedInt) {} FormatType type; };
	template<> class TGenDataType<unsigned long long> {public: TGenDataType() : type(TypeUnsignedDoubleInt) {} FormatType type; };
	template<> class TGenDataType<void> {public: TGenDataType() : type(TypeNone) {} FormatType type; };
	template<> class TGenDataType<bool> {public: TGenDataType() : type(TypeBool) {} FormatType type; };
	
} // !TGen	

#endif // !_TGEN_CORE_TYPES_H
