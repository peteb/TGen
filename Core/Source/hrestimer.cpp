/*
 *  hrestimer.cpp
 *  TGen Core
 *
 *  Created by Peter Backman on 7/24/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "hrestimer.h"
#include "platform.h"
#include <cmath>

#ifndef _PLATFORM_WIN
#include <sys/time.h>
#include <unistd.h>
#else
#include <windows.h>
#endif

TGen::Time::Time()
	: secs(0)
	, usecs(0)
{
	
}

TGen::Time::Time(int secs, int usecs)
	: secs(secs)
	, usecs(usecs)
{
}

TGen::Time::Time(double secs) {
	this->secs = int(secs);
	double rounded = int(floor(secs));
	this->usecs = int((secs - rounded) * 1000000.0);
}

TGen::Time::operator double() const {
	return double(secs) + double(usecs) * 0.000001;
}

int TGen::Time::getMicroseconds() const {
	return usecs;
}

int TGen::Time::getSeconds() const {
	return secs;
}

TGen::Time TGen::Time::Now() {
	#ifndef _PLATFORM_WIN
		timeval t;
		gettimeofday(&t, NULL);
		return TGen::Time(t.tv_sec, t.tv_usec);
	#else
		int64 frequency, ticks;		// om det inte funkar... fixa long long freq, ticks istället       eller __int64
		if (!QueryPerformanceFrequency((LARGE_INTEGER *)&frequency))
			frequency = 1000;
		
		if (!QueryPerformanceCounter((LARGE_INTEGER *)&ticks))					// TODO: kanske måste använda CLK_TCK.... KANSKE
			ticks = GetTickCount();
		
		double seconds = double(ticks) / double(frequency);
		int rounded = int(floor(seconds));
		
		return TGen::Time(rounded, int((seconds - double(rounded)) * 1000000.0));
	#endif	
}

void TGen::Sleep(const TGen::Time & time) {
	#ifdef _PLATFORM_WIN
		::Sleep(static_cast<DWORD>(double(time) * 1000.0));
	#else
		usleep(time.getMicroseconds());
		sleep(time.getSeconds());
	#endif	
}

