/*
 *  propertytree.h
 *  TGen Core
 *
 *  Created by Peter Backman on 9/1/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_PROPERTYTREE_H
#define _TGEN_PROPERTYTREE_H

#include <string>
#include <vector>
#include <map>

namespace TGen {
	class PropertyTree {
	public:	
		typedef std::pair<std::string, std::string> Property;
		typedef std::pair<const std::string, std::string> ConstProperty;
		typedef std::map<std::string, std::string> PropertyMap;
		
		PropertyTree(const std::string & name = "root");
		
		void addProperty(const Property & property);
		void addNode(const PropertyTree & tree);
		const ConstProperty & getProperty(const std::string & name) const;
		const PropertyTree & getNode(const std::string & name) const;
		PropertyTree & getNode(const std::string & name);
		int getNumProperties() const;
		const PropertyMap & getProperties() const;
		
		std::string getName() const;
		
	private:
		typedef std::vector<PropertyTree> TreeList;
		
		TreeList nodes;
		PropertyMap properties;
		std::string name;
	};
	
	
} // !TGen

#endif // !_TGEN_PROPERTYTREE_H

