/*
 *  modelinstantiatepool.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 2/3/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_MODELINSTANTIATEPOOL_H
#define _TGEN_RENDERER_MODELINSTANTIATEPOOL_H

#include <vector>

namespace TGen {
	class ModelInstanceProxy;
	class ModelInstantiator;
	
	// TODO: onRemove
	class ModelInstantiatePool {
	public:
		TGen::ModelInstanceProxy * attach(TGen::ModelInstanceProxy * proxy);
		void instantiateAll(TGen::ModelInstantiator & instantiator);
		
	private:
		typedef std::vector<TGen::ModelInstanceProxy *> ProxyList;
		ProxyList proxies;
	};

} // !TGen

#endif // !_TGEN_RENDERER_MODELINSTANTIATEPOOL_H
