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
	properties.insert(property);	
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

TGen::PropertyTree & TGen::PropertyTree::getNode(const std::string & name) {
	for (int i = 0; i < nodes.size(); ++i) {
		if (nodes[i].getName() == name)
			return nodes[i];
	}
	
	throw TGen::RuntimeException("PropertyTree::getNode", "node '" + name + "' not found");	
}

std::string TGen::PropertyTree::getName() const {
	return name;
}
