/*
 *  platform_windows.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 05/01/08.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_PLATFORM_WINDOWS_H
#define _TGEN_ENGINE_PLATFORM_WINDOWS_H

#include "tgen_core.h"
#include "tgen_core.h"

extern "C" {
	void DisplayErrorWindow(const char * title, const char * description);	
}


/*namespace TGen {
	namespace Engine {
		namespace Platform {
			void DisplayExceptionWindow(const TGen::RuntimeException & e, const std::string & title = "");
		} // !Platform
	} // !Engine
} // !TGen
*/

#endif // !_TGEN_ENGINE_PLATFORM_WINDOWS_H
