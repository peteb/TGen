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
	
	enum MaterialSort {
		MaterialSortOpaque = 10,
		MaterialSortTransparent = 30,
	};
	
	class MaterialLinkCallback {
	public:
		virtual ~MaterialLinkCallback() {}
		
		virtual TGen::ShaderProgram * getShaderProgram(const std::string & name) abstract;
		virtual TGen::Texture * getTexture(const std::string & name) abstract;
		virtual int getTextureType(const std::string & name) abstract;
	};
	
	class Material {
	public:
		Material(const std::string & name);
		~Material();
		
		std::string getName() const;
		void link(MaterialLinkCallback & callback);
		bool isLinked() const;
		void render(TGen::Renderer & renderer, const TGen::Renderable & renderable, const std::string & mode, int lod, TGen::Texture ** textureTypes, TGen::ShaderVariableUpdater * varupdater = NULL);
		void update(scalar time);
		void setMaximumTechnique(int minreqs);
		void setParameter(const std::string & name, const std::vector<std::string> & values);
		void setSpecialization(const std::string & name, TGen::TechniqueList * techniques);
		TGen::Technique * getSpecialization(const std::string & name);
		int getTimesRendered() const;
		
		std::vector<std::string> & getParameter(const std::string & name);

		typedef std::map<int, TGen::TechniqueList *> TechniqueListMap;
		typedef std::map<int, TGen::TechniqueList *> SpecializationList;

		const SpecializationList & getSpecializations() const;
		
	private:
		//typedef std::map<std::string, int> SpecializationMap;
		typedef std::map<std::string, std::vector<std::string> > ParameterMap;
		
		//int getSpecializationID(const std::string & name);
		
		//static SpecializationMap specializations;  // map specialization name to id		
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
