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
	class Technique;
	class ShaderVariableUpdater;
	class MaterialOverride;
	
	enum MaterialSort {
		MaterialSortOpaque = 10,
		MaterialSortTransparent = 30,
	};
	
	class MaterialLinkCallback;
	class MaterialRenderMetadata;
	
	class Material {
	public:
		Material(const std::string & name);
		~Material();
		
		std::string getName() const;
		void link(TGen::MaterialLinkCallback & callback);
		bool isLinked() const;

		void render(TGen::Renderer & renderer, const TGen::Renderable & renderable, const MaterialRenderMetadata & metadata);
		
		void update(scalar time);
		void setMaximumTechnique(int minreqs);
		void setParameter(const std::string & name, const std::vector<std::string> & values);
		void setSpecialization(const std::string & name, TGen::TechniqueList * techniques);
		TGen::Technique * getSpecialization(const std::string & name);
		TGen::TechniqueList * getTechniqueList(const std::string & name);
		
		int getTimesRendered() const;
		
		std::vector<std::string> & getParameter(const std::string & name);

		typedef std::map<int, TGen::TechniqueList *> TechniqueListMap;
		typedef std::map<int, TGen::TechniqueList *> SpecializationList;

		const SpecializationList & getSpecializations() const;
		
	private:
		typedef std::map<std::string, std::vector<std::string> > ParameterMap;
		

		static TGen::SymbolTable specializations;
		ParameterMap parameters;
		TechniqueListMap techniques;
		int minimumTechnique, sortLevel;
		std::string name;
		bool linked;
		int timesRendered;
	};
	
} // !TGen

#endif // !_TGEN_MATERIAL_H
