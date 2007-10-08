/*
 *  view.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 10/8/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_VIEW_H
#define _TGEN_ENGINE_VIEW_H

namespace TGen {
	namespace Engine {
		class GameView {
		public:
			GameView();
			virtual ~GameView();
			
			//const TGen::Matrix4x4 & getProjection();
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_VIEW_H
