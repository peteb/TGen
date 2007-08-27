/*
 *  version.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/27/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_VERSION_H
#define _TGEN_ENGINE_VERSION_H

namespace TGen {
	bool isEngineDebug();
	
	namespace Engine {
		const unsigned int VersionMajor = 0, VersionMinor = 0, VersionRevision = 1;
		
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_VERSION_H

