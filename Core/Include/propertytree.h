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
		typedef std::vector<std::string> AttributeList;
		
		PropertyTree(const std::string & name = "root");
		
		void addAttribute(const std::string & attr);
		
		void addProperty(const Property & property);
		void addNode(const PropertyTree & tree);

		const ConstProperty & getProperty(const std::string & name) const;
		const PropertyTree & getNode(const std::string & name) const;
		PropertyTree & getNode(const std::string & name, const AttributeList & attributes);
		
		const PropertyTree & getNode(int num) const;
		PropertyTree & getNode(int num);

		PropertyTree & getNode(const std::string & name);
		int getNumProperties() const;
		int getNumNodes() const;
		int getNumAttributes() const;
		std::string getAttribute(int id) const;
		
		bool hasNode(const std::string & name) const;
		bool hasProperty(const std::string & name) const;
		std::string getProperty(const std::string & name, const std::string & defaultValue) const;
		
		const PropertyMap & getProperties() const;
		const ConstProperty & operator [] (const std::string & name) const;
		
		std::string getName() const;
		void setName(const std::string & name);
		
	private:
		typedef std::vector<PropertyTree> TreeList;
		
		TreeList nodes;
		PropertyMap properties;
		std::string name;
		AttributeList attributes;
	};
	
	
} // !TGen

#endif // !_TGEN_PROPERTYTREE_H

