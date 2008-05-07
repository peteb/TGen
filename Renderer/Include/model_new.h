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
	class VertexDataSource;
	
	class NewModel : public TGen::NewModelInstance {
	public:
		NewModel(const std::string & name, const std::string & materialName, const std::string & materialNamePostfix);
		virtual ~NewModel();
		
		virtual TGen::NewModelInstance * instantiate(TGen::VertexDataSource & source) abstract;
	};	
	
} // !TGen

#endif // !_TGEN_RENDERER_MODEL_NEW_H
