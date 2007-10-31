/*
 *  timeinterval.cpp
 *  TGen Core
 *
 *  Created by Peter Backman on 10/31/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "timeinterval.h"
#include "platform.h"

#ifndef _PLATFORM_WIN
#include <sys/time.h>
#include <unistd.h>
#else
#include <windows.h>
#endif

TGen::TimeInterval TGen::TimeInterval::Forever(0.0f, true, false);
TGen::TimeInterval TGen::TimeInterval::Zero(0.0f, false, true);

TGen::TimeInterval::TimeInterval(const TGen::Time & time, bool forever, bool zero)
	: time(time)
	, forever(forever)
	, zero(zero)
{
}

bool TGen::TimeInterval::isForever() const {
	return forever;
}

bool TGen::TimeInterval::isZero() const {
	return zero;
}

const TGen::Time & TGen::TimeInterval::getTime() const {
	return time;
}

bool TGen::TimeInterval::inInterval(const TGen::Time & time) const {
	if (forever)
		return true;
	if (zero)
		return false;
	
	return double(time) <= double(this->time);
}

void TGen::Sleep(const TGen::TimeInterval & interval) {
	if (interval.isZero())
		return;
	
	TGen::Time time = interval.getTime();
	
#ifdef _PLATFORM_WIN
	if (interval.isForever()) {
		while (1) {
			::Sleep(static_cast<DWORD>(10000000));
		}
	}
	else {
		::Sleep(static_cast<DWORD>(double(time) * 1000.0));
	}
#else
	if (interval.isForever()) {
		while (1) {
			sleep(100000);
		}
	}
	else {
		usleep(time.getMicroseconds() + time.getSeconds() * 1000000);
		//usleep(time.getMicroseconds());
		//sleep(time.getSeconds());
	}
#endif	
}