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
	, objectBoundingSphere(0.0f)
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
	Surface newSurface = surface;
	newSurface.Attached(this);
	surfaces.push_back(newSurface);
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
	CalculateSurfacesObjectBV();
	CalculateWorldBV();
}

void SceneNode::CalculateSurfacesObjectBV() {
	TGen::Vector3 min, max;
	
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
	
	objectBoundingSphere = std::max(min.getMagnitude(), max.getMagnitude());
	objectBoundingBox.Calculate(min, max);	
}

void SceneNode::CalculateWorldBV() {
	TGen::Vector3 min, max;
	TGen::Vector3 corners[8];
	objectBoundingBox.getCorners(corners);
	
	for (int i = 0; i < 8; ++i) {
		corners[i] = transform * corners[i];
		if (corners[i].x < min.x)
			min.x = corners[i].x;
		if (corners[i].y < min.y)
			min.y = corners[i].y;
		if (corners[i].z < min.z)
			min.z = corners[i].z;
		
		if (corners[i].x > max.x)
			max.x = corners[i].x;
		if (corners[i].y > max.y)
			max.y = corners[i].y;
		if (corners[i].z > max.z)
			max.z = corners[i].z;
		
	}
	
	worldBoundingBox.Calculate(min, max);
}

TGen::Vector3 SceneNode::getWorldPosition() const {
	return transform.getOrigin();
}

TGen::Vector3 SceneNode::getWorldOrientation() const {
	return transform.getZ();
}

const TGen::AABB & SceneNode::getObjectAABB() const {
	return objectBoundingBox;
}

const TGen::AABB & SceneNode::getWorldAABB() const {
	return worldBoundingBox;
}

scalar SceneNode::getObjectBS() const {
	return objectBoundingSphere;
}

scalar SceneNode::getWorldBS() const {
	return 0.0f;
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

