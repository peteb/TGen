/*
 *  timeinterval.h
 *  TGen Core
 *
 *  Created by Peter Backman on 10/31/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_TIMEINTERVAL_H
#define _TGEN_TIMEINTERVAL_H

#include "hrestimer.h"

namespace TGen {
	class TimeInterval {		// TODO: TimeInterval with start and stop, start and stop can be flags and stuff
	public:
		TimeInterval(const TGen::Time & time, bool forever = false, bool zero = false);
		
		bool isForever() const;
		bool isZero() const;
		const TGen::Time & getTime() const;
		bool inInterval(const TGen::Time & time) const;
		
		static TimeInterval Forever;
		static TimeInterval Zero;
		
	private:
		TGen::Time time;
		bool forever, zero;
	};
	
	void Sleep(const TGen::TimeInterval & interval);
	
} // !TGen

#endif // !_TGEN_TIMEINTERVAL_H
