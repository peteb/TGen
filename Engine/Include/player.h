/*
 *  player.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 4/11/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_PLAYER_H
#define _TGEN_ENGINE_PLAYER_H

namespace TGen {
	class FPSCamera;
	
	namespace Engine {
		class Player {
		public:
			Player();
			
			TGen::FPSCamera * headCam;
		};
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_PLAYER_H

