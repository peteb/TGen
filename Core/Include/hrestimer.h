/*
 *  hrestimer.h
 *  TGen Core
 *
 *  Created by Peter Backman on 7/24/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_HRESTIMER_H
#define _TGEN_HRESTIMER_H

#include "types.h"

namespace TGen {
	class Time {
	public:	
		Time();
		Time(int secs, int usecs);
		Time(double secs);
		
		operator double() const;
		int getMicroseconds() const;
		int getSeconds() const;
		
		static Time Now();
		
	private:
		int secs, usecs;
	};
	
	void Sleep(const TGen::Time & time);
} // !TGen

#endif // !_TGEN_HRESTIMER_H
