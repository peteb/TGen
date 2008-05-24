/*
 *  meshdecl.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/19/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_MESHDECL_H
#define _TGEN_ENGINE_MESHDECL_H

namespace TGen {
	namespace Engine {
		namespace Physics {
			
			struct StridedVertex {
				dVector3 vertex;
			};
			
			struct StridedNormal {
				dVector3 normal;
			};
			
			struct StridedTriangle {
				int indices[3];
			};
			
		} // !Physics
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_MESHDECL_H

