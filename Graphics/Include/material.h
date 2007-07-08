/*
 *  material.h
 *  TGen Graphics
 *
 *  Created by Peter Backman on 7/4/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_MATERIAL_H
#define _TGEN_MATERIAL_H

#include "tgen_core.h"
#include <string>
#include <map>
#include <vector>

namespace TGen {
	class Renderable;
	class TechniqueList;
	class Renderer;
	class ShaderProgram;
	class Texture;
	
	class MaterialLinkCallback {
	public:
		virtual ~MaterialLinkCallback() {}
		
		virtual TGen::ShaderProgram * getShaderProgram(const std::string & name) abstract;
		virtual TGen::Texture * getTexture(const std::string & name) abstract;
	};
	
	class Material {
	public:
		Material(const std::string & name);
		~Material();
		
		std::string getName() const;
		void Link(MaterialLinkCallback & callback);
		void Render(TGen::Renderer & renderer, TGen::Renderable & renderable, const std::string & mode, int lod);
		void Update(scalar time);
		void setMaximumTechnique(int minreqs);
		void setParameter(const std::string & name, const std::vector<std::string> & values);
		void setSpecialization(const std::string & name, TGen::TechniqueList * techniques);
		
		std::vector<std::string> & getParameter(const std::string & name);
		
	private:
		typedef std::map<int, TGen::TechniqueList *> TechniqueListMap;
		typedef std::map<std::string, int> SpecializationMap;
		typedef std::map<std::string, std::vector<std::string> > ParameterMap;
		
		int getSpecializationID(const std::string & name);
		
		static SpecializationMap specializations;  // map specialization name to id		
		ParameterMap parameters;
		TechniqueListMap techniques;
		int minimumTechnique;
		std::string name;
	};
	
} // !TGen

#endif // !_TGEN_MATERIAL_H
