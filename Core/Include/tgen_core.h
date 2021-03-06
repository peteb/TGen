/*
 *  tgen_core.h
 *  TGen Core
 *
 *  Created by Peter Backman on 6/30/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_CORE_H
#define _TGEN_CORE_H

#include "error.h"
#include "types.h"
#include "platform.h"
#include "tokenizer.h"
#include "cast.h"
#include "stringman.h"
#include "hrestimer.h"
#include "timeinterval.h"
#include "debug.h"
#include "endian.h"
#include "symboltable.h"
#include "propertytree.h"
#include "propertytreeparser.h"
#include "streams.h"
#include "langutil.h"

namespace TGen {
	bool isCoreDebug();
}

#endif // !_TGEN_CORE_H
