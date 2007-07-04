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

#include <string>
#include <map>
#include <vector>

namespace TGen {
	class Renderable;
	class TechniqueList;
	class Renderer;
	
	class Material {
	public:
		Material(TGen::Renderer & renderer, const char * code);
		~Material();
		
		void Link(/*callback*/);
		void Render(TGen::Renderable & renderable, const std::string & mode, int lod);
		void Update(float dt);
		void setMinimumTechnique(int minreqs);
		void setParameter(const std::string & name, const std::vector<std::string> & values);
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
		TGen::Renderer & renderer;
	};
	
} // !TGen

#endif // !_TGEN_MATERIAL_H
