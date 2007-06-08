/*
 *  platform.h
 *  tgen
 *
 *  Created by Peter Backman on 6/6/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_PLATFORM_H
#define _TGEN_PLATFORM_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN__) || defined(WINDOWS)
	#define WIN32_LEAN_AND_MEAN
	#define _PLATFORM_WINDOWS

#elif defined(__APPLE__)
	#define _PLATFORM_OSX
#endif

#endif // !_TGEN_PLATFORM_H
