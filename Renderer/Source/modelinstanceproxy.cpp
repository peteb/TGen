/*
 *  modelinstanceproxy.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 2/3/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "modelinstanceproxy.h"
#include "modelinstantiator.h"
#include <tgen_core.h>

TGen::ModelInstanceProxy::ModelInstanceProxy(const std::string & name, const std::string & materialName, const std::string & materialNamePostfix)
	: TGen::NewModelInstance(name, materialName, materialNamePostfix)
	, boundInstance(NULL)
{

}

TGen::ModelInstanceProxy::~ModelInstanceProxy() {
	if (boundInstance && boundInstance->isPureInstance())
		delete boundInstance;
}

TGen::NewModelInstance * TGen::ModelInstanceProxy::operator *() {
	if (!boundInstance)
		throw TGen::RuntimeException("ModelInstanceProxy::deref", "failed to dereference proxy, not linked!");
	
	return boundInstance;
}

void TGen::ModelInstanceProxy::instantiate(TGen::ModelInstantiator & instantiator) {
	boundInstance = instantiator.instantiateModel(name);
	materialNamePostfix = boundInstance->getMaterialNamePostfix();
	
	std::cout << "INSTANTIATED to " << boundInstance->getName() << std::endl;
}

bool TGen::ModelInstanceProxy::isPureInstance() const {
	return true;
}
