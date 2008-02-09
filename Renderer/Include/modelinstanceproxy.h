/*
 *  modelinstanceproxy.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 2/3/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_MODELINSTANCEPROXY_H
#define _TGEN_RENDERER_MODELINSTANCEPROXY_H

#include "modelinstance_new.h"
#include <string>

namespace TGen {
	class ModelInstantiator;
	
	class ModelInstanceProxy : public TGen::NewModelInstance {
	public:
		ModelInstanceProxy(const std::string & name, const std::string & materialName = "", const std::string & materialNamePostfix = "");
		~ModelInstanceProxy();
		
		TGen::NewModelInstance * operator *();
		void instantiate(TGen::ModelInstantiator & instantiator);
		bool isPureInstance() const;
		
	private:
		TGen::NewModelInstance * boundInstance;
	};
	
} // !TGen

#endif // !_TGEN_RENDERER_MODELINSTANCEPROXY_H
