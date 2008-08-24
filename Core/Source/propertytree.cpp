/*
 *  propertytree.cpp
 *  TGen Core
 *
 *  Created by Peter Backman on 9/1/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "propertytree.h"
#include "error.h"

TGen::PropertyTree::PropertyTree(const std::string & name)
	: name(name)
{		
}

void TGen::PropertyTree::addProperty(const TGen::PropertyTree::Property & property) {
	PropertyMap::iterator iter = properties.find(property.first);
	if (iter == properties.end())
		properties.insert(property);
	else
		iter->second = property.second;
}

void TGen::PropertyTree::addNode(const TGen::PropertyTree & tree) {
	nodes.push_back(tree);
}

const TGen::PropertyTree::ConstProperty & TGen::PropertyTree::getProperty(const std::string & name) const {
	std::string::size_type slashPos = name.find("/");
	
	if (slashPos != std::string::npos) {	// this is a path
		std::string nodeName = name.substr(0, slashPos);
		std::string rest = name.substr(slashPos + 1, name.length() - slashPos - 1);

		return getNode(nodeName).getProperty(rest);
	}
	
	PropertyMap::const_iterator iter = properties.find(name);
	if (iter != properties.end())
		return *iter;
	
	throw TGen::RuntimeException("PropertyTree::getProperty", "property '" + name + "' not found");
}



const TGen::PropertyTree & TGen::PropertyTree::getNode(const std::string & name) const {
	for (int i = 0; i < nodes.size(); ++i) {
		if (nodes[i].getName() == name)
			return nodes[i];
	}
	
	throw TGen::RuntimeException("PropertyTree::getNode", "node '" + name + "' not found");
}


TGen::PropertyTree & TGen::PropertyTree::getNode(const std::string & name, const AttributeList & attributes) {
	// same as above but all attributes in "attributes" MUST match
	
	for (int i = 0; i < nodes.size(); ++i) {
		if (nodes[i].getName() == name) {
			if (nodes[i].getNumAttributes() == attributes.size()) {
				bool attributesMatch = true;
				
				for (int a = 0; a < attributes.size(); ++a) {
					if (nodes[i].getAttribute(a) != attributes[a]) {
						attributesMatch = false;
						break;
					}
				}
				
				if (attributesMatch)
					return nodes[i];
			}			
		}		
	}

	throw TGen::RuntimeException("PropertyTree::getNode", "node '" + name + "' with ") << attributes.size() << " attributes not found";
}

		  

TGen::PropertyTree & TGen::PropertyTree::getNode(const std::string & name) {
	for (int i = 0; i < nodes.size(); ++i) {
		if (nodes[i].getName() == name)
			return nodes[i];
	}
	
	throw TGen::RuntimeException("PropertyTree::getNode", "node '" + name + "' not found");	
}

std::string TGen::PropertyTree::getProperty(const std::string & name, const std::string & defaultValue) const {
	PropertyMap::const_iterator iter = properties.find(name);
	if (iter == properties.end())
		return defaultValue;
	
	return iter->second;
}

bool TGen::PropertyTree::hasProperty(const std::string & name) const {
	return (properties.find(name) != properties.end());
}

bool TGen::PropertyTree::hasNode(const std::string & name) const {
	for (int i = 0; i < nodes.size(); ++i) {
		if (nodes[i].getName() == name)
			return true;
	}
	
	return false;
}

std::string TGen::PropertyTree::getName() const {
	return name;
}

void TGen::PropertyTree::setName(const std::string & name) {
	this->name = name;
}

int TGen::PropertyTree::getNumProperties() const {
	return properties.size();
}

int TGen::PropertyTree::getNumNodes() const {
	return nodes.size();
}

const TGen::PropertyTree & TGen::PropertyTree::getNode(int num) const {
	return nodes[num];
}

TGen::PropertyTree & TGen::PropertyTree::getNode(int num) {
	return nodes[num];
}

const TGen::PropertyTree::PropertyMap & TGen::PropertyTree::getProperties() const {
	return properties;
}

const TGen::PropertyTree::ConstProperty & TGen::PropertyTree::operator [] (const std::string & name) const {
	return getProperty(name);
}

void TGen::PropertyTree::addAttribute(const std::string & attr) {
	attributes.push_back(attr);
}

int TGen::PropertyTree::getNumAttributes() const {
	return attributes.size();
}

std::string TGen::PropertyTree::getAttribute(int id) const {
	if (id >= attributes.size())
		throw TGen::RuntimeException("PropertyTree::getAttribute", "index out of bounds");
	
	return attributes[id];
}

