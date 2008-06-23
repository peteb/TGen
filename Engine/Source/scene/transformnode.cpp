/*
 *  transformnode.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/24/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "scene/transformnode.h"
#include "scene/nodetransformer.h"

TGen::Engine::Scene::TransformNode::TransformNode(const std::string & name)
	: TGen::SceneNode(name)
	, age(0.0)
{
}

TGen::Engine::Scene::TransformNode::TransformNode(const TransformNode & node) 
	: TGen::SceneNode(node)
	, age(node.age)
{
	for (int i = 0; i < node.transformers.size(); ++i)
		transformers.push_back(node.transformers[i]->clone());
}

TGen::Engine::Scene::TransformNode::~TransformNode() {
	for (int i = 0; i < transformers.size(); ++i)
		delete transformers[i];
}


void TGen::Engine::Scene::TransformNode::addPositionTransformer(const TGen::PropertyTree & properties) {
	std::string type = properties.getProperty("type", "unknown");
	std::string axis = properties.getProperty("axis", "0 1 0");
	
	std::auto_ptr<TGen::ScalarGenerator> generator(createWaveGenerator(properties));

	transformers.push_back(new TGen::Engine::Scene::NodePositionWaveTransformer(TGen::lexical_cast<bool>(properties.getProperty("relative", "false")),
																										 TGen::Vector3::Parse(axis),
																										 generator));
}

void TGen::Engine::Scene::TransformNode::addOrientationTransformer(const TGen::PropertyTree & properties) {
	std::string type = properties.getProperty("type", "unknown");
	std::string axis = properties.getProperty("axis", "0 1 0");
	
	std::auto_ptr<TGen::ScalarGenerator> generator;
	
	if (type == "sine" || type == "square" || type == "triangle" || type == "sawtooth" || type == "invsawtooth" || type == "const")
		generator.reset(createWaveGenerator(properties));
	else
		throw TGen::RuntimeException("Scene::TransformNode::addOrientationTransformer", "invalid type '" + type + "'");
	
	transformers.push_back(new TGen::Engine::Scene::NodeOrientationWaveTransformer(TGen::lexical_cast<bool>(properties.getProperty("relative", "false")),
																										 TGen::Vector3::Parse(axis),
																										 generator));
}

TGen::WaveGenerator * TGen::Engine::Scene::TransformNode::createWaveGenerator(const TGen::PropertyTree & properties) const {
	WaveType type;
	std::string strType = properties.getProperty("type", "unknown");
	
	if (strType == "sine")
		type = TGen::WaveSine;
	else if (strType == "square")
		type = TGen::WaveSquare;
	else if (strType == "triangle")
		type = TGen::WaveTriangle;
	else if (strType == "sawtooth")
		type = TGen::WaveSawtooth;
	else if (strType == "invsawtooth")
		type = TGen::WaveInverseSawtooth;
	else if (strType == "const")
		type = TGen::WaveConst;
	else
		throw TGen::RuntimeException("Scene::TransformNode::createWaveGenerator", "invalid wave type: '" + strType + "'");
	
	return new TGen::WaveGenerator(type, 
											 TGen::lexical_cast<scalar>(properties.getProperty("base", "0")),
											 TGen::lexical_cast<scalar>(properties.getProperty("amplitude", "1")),
											 TGen::lexical_cast<scalar>(properties.getProperty("phase", "0")),
											 TGen::lexical_cast<scalar>(properties.getProperty("frequency", "1")));
}


void TGen::Engine::Scene::TransformNode::update(scalar dt) {
	age += dt;
	
	for (int i = 0; i < transformers.size(); ++i) {
		transformers[i]->transform(*this, age);
	}
	
	TGen::SceneNode::update(dt);
}

TGen::Engine::Scene::TransformNode * TGen::Engine::Scene::TransformNode::clone() {
	return new TGen::Engine::Scene::TransformNode(*this);
}

