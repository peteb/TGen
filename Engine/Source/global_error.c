/*
 *  global_error.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/19/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "global_error.h"

static char lastErrorMessage[1024];

char * getLastErrorMessage() {
	return lastErrorMessage;
}
