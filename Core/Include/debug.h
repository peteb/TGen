/*
 *  debug.h
 *  TGen Core
 *
 *  Created by Peter Backman on 7/25/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_DEBUG_H
#define _TGEN_DEBUG_H

#define DEBUG_OUT std::cout
#define DEBUG_LFFLUSH std::endl

#ifdef _DEBUG
#define DEBUG_EXEC(x) x
#else
#define DEBUG_EXEC(x)
#endif

#define DEBUG_PRINT(x) DEBUG_EXEC(DEBUG_OUT << x << DEBUG_LFFLUSH)

#endif // !_TGEN_DEBUG_H
