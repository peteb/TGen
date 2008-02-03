/*
 *  model_new.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 2/3/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_MODEL_NEW_H
#define _TGEN_RENDERER_MODEL_NEW_H

#include <tgen_core.h>
#include "modelinstance_new.h"

namespace TGen {
	class NewMesh;
	
	class NewModel : public TGen::NewModelInstance {
	public:
		NewModel(const std::string & name);
		virtual ~NewModel();
		
		virtual TGen::NewModelInstance * instantiate() abstract;
	};	
	
} // !TGen

#endif // !_TGEN_RENDERER_MODEL_NEW_H
