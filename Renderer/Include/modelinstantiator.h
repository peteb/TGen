/*
 *  modelinstantiator.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 2/3/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_MODELINSTANTIATOR_H
#define _TGEN_RENDERER_MODELINSTANTIATOR_H

#include <tgen_core.h>

namespace TGen {
	class NewModelInstance;
	
	class ModelInstantiator {
	public:
		virtual ~ModelInstantiator() {}
		
		virtual TGen::NewModelInstance * instantiateModel(const std::string & name) abstract;
	};
	
} // !TGen

#endif // !_TGEN_RENDERER_MODELINSTANTIATOR_H
