/*
 *  componentinterfaces.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/26/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_COMPONENTINTERFACES_H
#define _TGEN_ENGINE_COMPONENTINTERFACES_H

namespace TGen {
	namespace Engine {		
		class WeaponInterface {
		public:
			virtual ~WeaponInterface() {}
			
			virtual void beginFire(int mode) abstract;
			virtual void endFire(int mode) abstract;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_COMPONENTINTERFACES_H

