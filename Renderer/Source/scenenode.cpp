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
#include "geometry.h"
#include "renderlist.h"
#include "modelinstance_new.h"
#include <cassert>

TGen::SceneNode::SceneNode(const std::string & name, const TGen::Vector3 & position)
	: name(name)
	, position(position)
	, up(0.0f, 1.0f, 0.0f)
	, changed(true)
	, parent(NULL)
	, orientation(TGen::Rotation::Identity)
	, autoParent(NULL)
	, doRender(true)
{
		
}

TGen::SceneNode::SceneNode(const std::string & name, const TGen::Vector3 & position, const TGen::Rotation & orientation) 
	: name(name)
	, position(position)
	, orientation(orientation)
	, up(0.0f, 1.0f, 0.0f)
	, changed(true)
	, changedSinceLastCheck(false)
	, parent(NULL)
	, autoParent(NULL)
	, doRender(true)
{
	
}

TGen::SceneNode::SceneNode(const SceneNode & node) 
	: name(node.getName())
	, position(node.getLocalPosition())
	, orientation(node.getLocalOrientation())
	, up(node.up)
	, changed(true)
	, changedSinceLastCheck(false)
	, parent(NULL)
	, autoParent(node.autoParent)
	//, models(node.models)
	, faces(node.faces)
	, doRender(true)
	//, children(node.children)		// TODO: I hope this works, the children shouldn't be shared! It doesn't work.
{
	for (ModelInstanceList::const_iterator iter = node.models.begin(); iter != node.models.end(); ++iter) {	// fixed here though
		models.push_back(TGen::DerefRes(*iter)->clone());
	}
}

																		
																		
TGen::SceneNode::~SceneNode() {
	if (parent)
		parent->removeChild(this);
	
	for (int i = 0; i < models.size(); ++i) {
		if (models[i]->isPureInstance())
			delete models[i];
	}
	
	for (int i = 0; i < children.size(); ++i) {
		children[i]->detached();
		delete children[i];
	}
}

void TGen::SceneNode::update(scalar dt) {
	for (int i = 0; i < models.size(); ++i)
		TGen::DerefRes(models[i])->update();
	
	if ((parent && parent->changed) || this->changed) {			
		//TGen::Quaternion4 front(0.0f, 0.0f, 1.0f);
		//TGen::Quaternion4 result = orientation * front * -orientation;
		//TGen::Vector3 dir = TGen::Vector3(orientation.x, orientation.y, orientation.z);
		
		if (parent)
			this->transform = parent->getTransform() * (TGen::Matrix4x4::Translation(position) * TGen::Matrix4x4(orientation)); //TGen::Matrix4x4::LookInDirection(dir, up));
		else
			this->transform = TGen::Matrix4x4::Translation(position) * TGen::Matrix4x4(orientation); //TGen::Matrix4x4::LookInDirection(dir, up);
		
	/*	std::cout << "X: " << std::string(this->transform.getX()) << std::endl;
		std::cout << "Y: " << std::string(this->transform.getY()) << std::endl;
		std::cout << "Z: " << std::string(this->transform.getZ()) << std::endl;
		std::cout << std::endl;
	*/
				
		this->changed = true;
	}
	
	updateChildren(dt);
	
	if (this->changed) {
		calculateFacesBV();
		calculateWorldBV();
		
		if (autoParent) {
			TGen::SceneNode * newParent = autoParent->getNodeFromPoint(autoParent->getTransform().getInverse() * getWorldPosition());
			if (!newParent) {
				//std::cout << "NOPARENT" << std::endl;	// TODO:
				newParent = autoParent;
			}
			
			if (newParent != parent && newParent && parent) {
				moveTo(newParent);
			}
		}
	}
	
	this->changedSinceLastCheck = this->changed;
	this->changed = false;	
}


void TGen::SceneNode::moveTo(TGen::SceneNode * newParent, bool translate) {
	TGen::SceneNode * oldParent = parent;
	
	if (parent)
		parent->removeChild(this);
	
	newParent->addChild(this);	

	if (translate) {
		TGen::Matrix4x4 globalTransform = oldParent->getTransform() * newParent->getTransform().getInverse();
	
		setPosition(globalTransform * getLocalPosition());
		setOrientation(globalTransform * getLocalOrientation());
	}
}

void TGen::SceneNode::setRender(bool doRender) {
	this->doRender = doRender;
}

int TGen::SceneNode::getNumChildren() const {
	return children.size();
}

const TGen::SceneNode * TGen::SceneNode::getChild(int index) const {
	return children[index];
}


void TGen::SceneNode::updateChildren(scalar dt) {
	for (int i = 0; i < children.size(); ++i) {
		children[i]->update(dt);
	}	
}

const std::string & TGen::SceneNode::getName() const {
	return name;
}

TGen::Vector3 TGen::SceneNode::getLocalPosition() const {
	return position;
}

TGen::Rotation TGen::SceneNode::getLocalOrientation() const {
	return orientation;
}

TGen::Vector3 TGen::SceneNode::getWorldPosition() const {
	return transform.getOrigin();
}

TGen::Rotation TGen::SceneNode::getWorldOrientation() const {
	return TGen::Rotation(transform);
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

TGen::SceneNode * TGen::SceneNode::getParent() {
	return parent;
}

/*const TGen::SceneNode::FaceList & TGen::SceneNode::getFaces() const {
	return faces;
}
*/
TGen::SceneNode::FaceList & TGen::SceneNode::getFaces() {
	return faces;
}

TGen::SceneNode::ModelInstanceList & TGen::SceneNode::getModels() {
	return models;
}


void TGen::SceneNode::setPosition(const TGen::Vector3 & position) {
	this->position = position;
	changed = true;
}

void TGen::SceneNode::setOrientation(const TGen::Rotation & orientation) {
	this->orientation = orientation;
	changed = true;
}

void TGen::SceneNode::addChild(TGen::SceneNode * node) {
	children.push_back(node);
	node->attached(this);
}

TGen::SceneNode * TGen::SceneNode::getChild(const std::string & name) {
	for (SceneNodeList::iterator iter = children.begin(); iter != children.end(); ++iter) {
		if ((*iter)->getName() == name)
			return *iter;
	}
	
	throw TGen::RuntimeException("SceneNode::getChild", "no child named '" + name + "' found!");
}

TGen::SceneNode * TGen::SceneNode::getNode(const std::string & path, bool create) {
	int pos = path.find("/");
	if (pos == std::string::npos)
		pos = path.size();
		
	std::string nodeName = path.substr(0, pos);
	if (nodeName.empty())
		return this;
	
	for (SceneNodeList::iterator iter = children.begin(); iter != children.end(); ++iter) {
		if ((*iter)->getName() == nodeName) {
			if (pos == path.size())
				return (*iter)->getNode("");				
			else
				return (*iter)->getNode(path.substr(pos + 1));
		}
	}
	
	if (create) {	// TODO: create
		
	}
	
	return NULL;
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

void TGen::SceneNode::addFace(const TGen::Face & face) {
	faces.push_back(TGen::Face(face.getGeometry(), face.getMaterialName(), this));
}


void TGen::SceneNode::calculateFacesBV() {
	TGen::Vector3 min, max;
	bool first = true;
	
	for (int i = 0; i < faces.size(); ++i) {
		TGen::Geometry * geom = faces[i].getGeometry();
		assert(geom);
		
		if (first) {
			min = geom->getMin();
			max = geom->getMax();
			first = false;
		}
		else {
			min.x = std::min(geom->getMin().x, min.x);
			min.y = std::min(geom->getMin().y, min.y);
			min.z = std::min(geom->getMin().z, min.z);
		
			max.x = std::max(geom->getMax().x, max.x);
			max.y = std::max(geom->getMax().y, max.y);
			max.z = std::max(geom->getMax().z, max.z);
		}
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

bool TGen::SceneNode::checkChanged() {
	bool ret = changedSinceLastCheck;
	changedSinceLastCheck = false;
	
	return ret;	
}

void TGen::SceneNode::traverse(const TGen::SceneNode::Walker & walker) {
	//if (!doRender /*&& walker.getFlags() & WalkerFollowVisibility*/)
	//	return;
	
	if (walker.pre(*this)) {
		for (int i = 0; i < children.size(); ++i) {
			children[i]->traverse(walker);
		}
	}
	
	walker.post(*this);
}

// TODO: en addClippedFace som tar face + clipping area (planes eller box)
//       spara alla faces som clipped face i lista, sen uppdatera clipping h0 r

bool TGen::SceneNode::fillFaces(TGen::RenderList & list, const TGen::Camera & camera) const {
	//for (int i = 0; i < faces.size(); ++i) {
		//list.addFace(&faces[i]);
	//}
	
	if (!doRender)
		return false;
	
	for (int i = 0; i < models.size(); ++i) {
		TGen::NewModelInstance * model = models[i];

		model = TGen::DerefRes(model);
		model->fillFaces(list, models[i]->getOverridingMaterial(), this, &renderProperties);
	}
	
	return true;
}

bool TGen::SceneNode::fillMeta(TGen::RenderList & list, const TGen::Camera & camera) const {
	if (!doRender)
		return false;
	
	
	for (int i = 0; i < models.size(); ++i)
		list.addMeta(TGen::DerefRes(models[i]), this);
	
	return true;
}

bool TGen::SceneNode::fillUser(TGen::RenderList & list, const TGen::Camera & camera) const {
	
	return true;
}

void TGen::SceneNode::addModel(TGen::NewModelInstance * model) {
	models.push_back(model);
}

TGen::SceneNode * TGen::SceneNode::getNodeFromPoint(const TGen::Vector3 & point) {
	return this;
}

void TGen::SceneNode::setAutoTP(TGen::SceneNode * autoTP) {
	this->autoParent = autoTP;
}

TGen::SceneNode * TGen::SceneNode::clone() {
	return new TGen::SceneNode(*this);
}

TGen::ModelRenderProperties & TGen::SceneNode::getRenderProperties() {
	return renderProperties;
}

void TGen::SceneNode::overrideModelMaterials(TGen::Material * material) {
	for (ModelInstanceList::iterator iter = models.begin(); iter != models.end(); ++iter) {
		(*iter)->setOverridingMaterial(material);
	}
}

