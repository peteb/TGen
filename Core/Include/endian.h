/*
 *  endian.h
 *  TGen Core
 *
 *  Created by Peter Backman on 8/5/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENDIAN_H
#define _TGEN_ENDIAN_H

#include "types.h"

#define ByteSwap32(x) ((((x) & 0x000000FF) << 24) + (((x) & 0x0000FF00) << 8) + (((x) & 0x00FF0000) >> 8) + (((x) & 0xFF000000) >> 24))
#define ByteSwap16(x) (((x) >> 8) | ((x) << 8))

#ifdef __BIG_ENDIAN__
#define SwapLocalLe32(x) ((x) = ByteSwap32(x))
#else
#define SwapLocalLe32(x)
#endif

#ifdef __LITTLE_ENDIAN__
#define SwapLocalBe32(x) ((x) = ByteSwap32(x))
#else
#define SwapLocalBe32(x)
#endif

#ifdef __BIG_ENDIAN__
#define SwapLocalLe16(x) ((x) = ByteSwap16(x))
#else
#define SwapLocalLe16(x)
#endif

#ifdef __LITTLE_ENDIAN__
#define SwapLocalBe16(x) ((x) = ByteSwap16(x))
#else
#define SwapLocalBe16(x)
#endif

namespace TGen {
	/*void SwapLocalLe(uint32 & var);
	void SwapLocalLe(uint16 & var);
	void SwapLocalBe(uint32 & var);
	void SwapLocalBe(uint16 & var);
	void SwapLocalLe(int32 & var);
	void SwapLocalLe(int16 & var);
	void SwapLocalBe(int32 & var);
	void SwapLocalBe(int16 & var);
	*/
	
	
} // !TGen

#endif // !_TGEN_ENDIAN_H
