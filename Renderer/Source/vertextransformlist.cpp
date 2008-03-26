/*
 *  vertextransformlist.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 3/26/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "vertextransformlist.h"

TGen::VertexTransformList::VertexTransformList() {
	
}

TGen::VertexTransformList::~VertexTransformList() {
	for (int i = 0; i < transformers.size(); ++i)
		delete transformers[i];
}


void TGen::VertexTransformList::transform(TGen::Vector3 & vector) const {
	for (int i = 0; i < transformers.size(); ++i)
		transformers[i]->transform(vector);
}

void TGen::VertexTransformList::addTransformer(TGen::VertexTransformer * transformer) {
	transformers.push_back(transformer);
}

void TGen::VertexTransformList::addTransformerFirst(TGen::VertexTransformer * transformer) {
	transformers.insert(transformers.begin(), transformer);
}

TGen::VertexTransformList * TGen::VertexTransformList::clone() const {
	std::auto_ptr<TGen::VertexTransformList> newList(new TGen::VertexTransformList);
	
	for (int i = 0; i < transformers.size(); ++i) {
		newList->addTransformer(transformers[i]->clone());
	}
	
	return newList.release();
}
