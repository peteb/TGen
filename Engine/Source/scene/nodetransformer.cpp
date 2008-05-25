/*
 *  nodetransformer.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/25/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "scene/nodetransformer.h"
#include "scene/transformnode.h"

TGen::Engine::Scene::NodeTransformer::NodeTransformer(bool relative)
	: relative(relative)
{
}


TGen::Engine::Scene::NodePositionWaveTransformer::NodePositionWaveTransformer(bool relative, const TGen::Vector3 & axis, std::auto_ptr<TGen::ScalarGenerator> & generator)	
	: TGen::Engine::Scene::NodeTransformer(relative)
	, generator(generator)
	, axis(axis)
{
	
}

void TGen::Engine::Scene::NodePositionWaveTransformer::transform(TGen::Engine::Scene::TransformNode & node, scalar time) {
	scalar value = generator->getValue(time);
	
	if (relative)
		node.setPosition(node.getLocalPosition() + axis * value);
	else
		node.setPosition(axis * value);
}


TGen::Engine::Scene::NodeOrientationWaveTransformer::NodeOrientationWaveTransformer(bool relative, const TGen::Vector3 & axis, std::auto_ptr<TGen::ScalarGenerator> & generator)	
	: TGen::Engine::Scene::NodeTransformer(relative)
	, generator(generator)
	, axis(axis)
{
	
}

void TGen::Engine::Scene::NodeOrientationWaveTransformer::transform(TGen::Engine::Scene::TransformNode & node, scalar time) {
	scalar value = generator->getValue(time);
	
	TGen::Rotation rotMat;
	rotMat = TGen::Rotation::Rotation(axis, TGen::UnitAngle(value));
	
	if (relative)
		node.setOrientation(node.getLocalOrientation() * rotMat);
	else
		node.setOrientation(rotMat);
}
