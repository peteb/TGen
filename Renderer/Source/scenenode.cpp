/*
 *  scenenode.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/13/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "scenenode.h"
#include "face.h"

TGen::SceneNode::SceneNode(const std::string & name, const TGen::Vector3 & position, const TGen::Quaternion4 & orientation)
	: name(name)
	, position(position)
	, orientation(orientation)
	, up(0.0f, 1.0f, 0.0f)
	, changed(true)
	, parent(NULL)
{
		
}

TGen::SceneNode::~SceneNode() {
	if (parent)
		parent->removeChild(this);
	
	for (int i = 0; i < children.size(); ++i) {
		children[i]->detached();
		delete children[i];
	}
}

void TGen::SceneNode::update() {
	if ((parent && parent->changed) || this->changed) {			
		TGen::Quaternion4 front(0.0f, 0.0f, 1.0f);
		TGen::Quaternion4 result = orientation * front * -orientation;
		
		if (parent)
			this->transform = parent->getTransform() * TGen::Matrix4x4::Translation(-position) * TGen::Matrix4x4::LookInDirection(result, up);
		else
			this->transform = TGen::Matrix4x4::Translation(-position) * TGen::Matrix4x4::LookInDirection(result, up);
		
		this->changed = true;
	}
	
	updateChildren();

	if (this->changed) {
		calculateFacesBV();
		calculateWorldBV();
	}
	
	this->changed = false;	
}

void TGen::SceneNode::updateChildren() {
	for (int i = 0; i < children.size(); ++i) {
		children[i]->update();
	}	
}

const std::string & TGen::SceneNode::getName() const {
	return name;
}

TGen::Vector3 TGen::SceneNode::getLocalPosition() const {
	return position;
}

TGen::Quaternion4 TGen::SceneNode::getLocalOrientation() const {
	return orientation;
}

TGen::Vector3 TGen::SceneNode::getWorldPosition() const {
	return transform.getOrigin();
}

TGen::Quaternion4 TGen::SceneNode::getWorldOrientation() const {
	return transform.getZ();
}

const TGen::Matrix4x4 & TGen::SceneNode::getTransform() const {
	return transform;
}

const TGen::AABB & TGen::SceneNode::getLocalBoundingBox() const {
	return localBoundingBox;
}

const TGen::AABB & TGen::SceneNode::getWorldBoundingBox() const {
	return worldBoundingBox;
}

const TGen::Sphere & TGen::SceneNode::getLocalBoundingSphere() const {
	return localBoundingSphere;
}

const TGen::Sphere & TGen::SceneNode::getWorldBoundingSphere() const {
	return worldBoundingSphere;
}

void TGen::SceneNode::addChild(TGen::SceneNode * node) {
	children.push_back(node);
	node->attached(this);
}

void TGen::SceneNode::removeChild(TGen::SceneNode * node) {
	SceneNodeList::iterator iter = children.begin();
	for (; iter != children.end(); ++iter) {
		if (*iter == node) {
			children.erase(iter);
			node->detached();
			break;
		}
	}	
}

void TGen::SceneNode::addFace(TGen::Face * face) {
	faces.push_back(face);
}


void TGen::SceneNode::calculateFacesBV() {
	TGen::Vector3 min, max;
	
	for (int i = 0; i < faces.size(); ++i) {
		min.x = std::min(faces[i]->getMin().x, min.x);
		min.y = std::min(faces[i]->getMin().y, min.y);
		min.z = std::min(faces[i]->getMin().z, min.z);
		
		max.x = std::max(faces[i]->getMax().x, max.x);
		max.y = std::max(faces[i]->getMax().y, max.y);
		max.z = std::max(faces[i]->getMax().z, max.z);
	}
	
	TGen::AABB local(min, max);
	local += getChildrenBoundingBox();
	
	localBoundingBox = local;
	localBoundingSphere.calculate(min, max);
}

TGen::AABB TGen::SceneNode::getChildrenBoundingBox() const {
	TGen::AABB bbTotal;
	
	if (!children.empty()) {
		bbTotal = children[0]->getLocalBoundingBox();
		
		for (int i = 1; i < children.size(); ++i) {
			bbTotal += children[i]->getLocalBoundingBox();
		}
	}
	
	return bbTotal;
}

void TGen::SceneNode::calculateWorldBV() { // a generic solution that works on any localBoundingBox
	TGen::Vector3 min = transform.getOrigin();
	TGen::Vector3 max = transform.getOrigin();
	
	TGen::Vector3 corners[8];
	localBoundingBox.getCorners(corners);
	
	for (int i = 0; i < 8; ++i) {
		corners[i] = transform * corners[i];
		
		min.x = std::min(corners[i].x, min.x);
		min.y = std::min(corners[i].y, min.y);
		min.z = std::min(corners[i].z, min.z);
		
		max.x = std::max(corners[i].x, max.x);
		max.y = std::max(corners[i].y, max.y);
		max.z = std::max(corners[i].z, max.z);
	}
	
	worldBoundingBox.calculate(min, max);
	worldBoundingSphere.calculate(min, max);
}

void TGen::SceneNode::attached(TGen::SceneNode * parent) {
	if (this->parent)
		throw TGen::RuntimeException("SceneNode::Attached", "this node ('" + getName() + "') is already attached to '" + this->parent->getName() + "'!");
		
	this->parent = parent;
}

void TGen::SceneNode::detached() {
	parent = NULL;
}

bool TGen::SceneNode::hasChanged() const {
	return changed;
}

void TGen::SceneNode::traverse(const TGen::SceneNode::Walker & walker) {
	if (walker.pre(*this)) {
		for (int i = 0; i < children.size(); ++i) {
			children[i]->traverse(walker);
		}
	}
	
	walker.post(*this);
}
