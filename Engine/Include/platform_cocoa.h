/*
 *  platform_cocoa.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/19/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_PLATFORM_COCOA_H
#define _TGEN_ENGINE_PLATFORM_COCOA_H

#include "tgen_core.h"

extern "C" {	
	void DisplayErrorWindow(const char * title, const char * description);	// have to be exported on every platform
}

namespace TGen {
	namespace Engine {
		namespace Platform {
			void DisplayExceptionWindow(const TGen::RuntimeException & e, const std::string & title = "");
		} // !Platform
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_PLATFORM_COCOA_H
