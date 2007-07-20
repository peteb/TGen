/*
 *  scenenode.cpp
 *  The Quaker
 *
 *  Created by Peter Backman on 7/16/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "scenenode.h"
#include "renderlist.h"

SceneNode::SceneNode(const std::string & name) 
	: name(name)
	, updated(true)
	, orientation(0.0f, 0.0f, 1.0f)
	, up(0.0f, 1.0f, 0.0f)
	, parent(NULL)
	, boundingSphere(0.0f)
	, boundingBox(TGen::Vector3(0.0f, 0.0f, 0.0f), 1.0f, 1.0f, 1.0f)
{
	
}

SceneNode::~SceneNode() {
	if (parent)
		parent->RemoveChild(this);
	
	for (int i = 0; i < children.size(); ++i) {
		children[i]->Detached();
		delete children[i];
	}
}

const TGen::Matrix4x4 & SceneNode::getTransform() const {
	return transform;
}

void SceneNode::Update(const TGen::Matrix4x4 & parent, bool parentUpdated) {
	if (updated || parentUpdated) {
		std::cout << "node '" << getName() << "' updated, calculates transform" << std::endl;
		updated = true;
		transform = parent * TGen::Matrix4x4::Translation(-position) * TGen::Matrix4x4::LookInDirection(orientation, up); //TGen::Matrix4x4::LookAt(position, position + orientation, up);
	
		CalculateBV();
	}
	
	for (int i = 0; i < children.size(); ++i) {
		children[i]->Update(transform, updated);
	}

	updated = false;
}

void SceneNode::setPosition(const TGen::Vector3 & position) {
	this->position = position;
	updated = true;
}

void SceneNode::setOrientation(const TGen::Vector3 & orientation) {
	this->orientation = orientation;
	updated = true;
}

const TGen::Vector3 & SceneNode::getPosition() const {
	return position;
}

const TGen::Vector3 & SceneNode::getOrientation() const {
	return orientation;
}

const std::string & SceneNode::getName() const {
	return name;
}

void SceneNode::AddSurface(const Surface & surface) {
	surfaces.push_back(surface);
}

void SceneNode::AddChild(SceneNode * node) {
	children.push_back(node);
	node->Attached(this);
}

void SceneNode::RemoveChild(SceneNode * node) {
	SceneNodeList::iterator iter = children.begin();
	for (; iter != children.end(); ++iter) {
		if (*iter == node) {
			children.erase(iter);
			break;
		}
	}
}

void SceneNode::Attached(SceneNode * parent) {
	if (this->parent)
		throw TGen::RuntimeException("SceneNode::Attached", "this node ('" + getName() + "') is already attached to '" + this->parent->getName() + "'!");
	
	this->parent = parent;
}

void SceneNode::Detached() {
	parent = NULL;
}

void SceneNode::CalculateBV() {
	TGen::Vector3 min(-0.3f, -0.3f, -0.3f), max(0.3f, 0.3f, 0.3f);
	
	for (int i = 0; i < surfaces.size(); ++i) {
		if (surfaces[i].getMin().x < min.x)
			min.x = surfaces[i].getMin().x;
		if (surfaces[i].getMin().y < min.y)
			min.y = surfaces[i].getMin().y;
		if (surfaces[i].getMin().z < min.z)
			min.z = surfaces[i].getMin().z;
		
		if (surfaces[i].getMax().x > max.x)
			max.x = surfaces[i].getMax().x;
		if (surfaces[i].getMax().y > max.y)
			max.y = surfaces[i].getMax().y;
		if (surfaces[i].getMax().z > max.z)
			max.z = surfaces[i].getMax().z;
	}
	
	min = transform.getOrigin() + min;
	max = transform.getOrigin() + max;
	
	boundingSphere = std::max(min.getMagnitude(), max.getMagnitude());
	boundingBox.Calculate(min, max);
}

TGen::Vector3 SceneNode::getGlobalPosition() const {
	return transform.getOrigin();
}

TGen::Vector3 SceneNode::getGlobalOrientation() const {
	return transform.getZ();
}

const TGen::AABB & SceneNode::getAABB() const {
	return boundingBox;
}

scalar SceneNode::getBS() const {
	return boundingSphere;
}

/*void SceneNode::AddSurfaces(RenderList & list, const Camera & camera) const {
	for (int i = 0; i < surfaces.size(); ++i)
		list.AddSurface(&surfaces[i]);
	
	for (int i = 0; i < children.size(); ++i)
		children[i]->AddSurfaces(list, camera);
}*/

void SceneNode::Accept(SceneNodeVisitor & visitor) {
	visitor.Visit(*this);
	
	for (int i = 0; i < children.size(); ++i)
		children[i]->Accept(visitor);
}

void SceneNode::setHej(float size) {
	boundingBox.width = size;
	boundingBox.height = size;
	boundingBox.depth = size;
}

