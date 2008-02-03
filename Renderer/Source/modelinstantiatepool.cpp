/*
 *  modelinstantiatepool.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 2/3/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "modelinstantiatepool.h"
#include "modelinstantiator.h"
#include "modelinstanceproxy.h"

TGen::ModelInstanceProxy * TGen::ModelInstantiatePool::attach(TGen::ModelInstanceProxy * proxy) {
	proxies.push_back(proxy);
	
	return proxy;
}

void TGen::ModelInstantiatePool::instantiateAll(TGen::ModelInstantiator & instantiator) {
	for (int i = 0; i < proxies.size(); ++i)
		proxies[i]->instantiate(instantiator);
}

