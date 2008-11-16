/*
 *  meshinstance_new.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 2/3/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_MESHINSTANCE_NEW_H
#define _TGEN_RENDERER_MESHINSTANCE_NEW_H

#include <string>
#include <tgen_graphics.h>
#include "face_new.h"

namespace TGen {
	class Material;
	class MaterialSource;
	
	class NewMeshInstance : public TGen::Renderable, public TGen::ShaderVariableUpdater {
	public:
		NewMeshInstance(const std::string & materialName, const std::string & materialNamePostfix);
		virtual ~NewMeshInstance();
		
		virtual void update() {}
		TGen::NewMeshInstance * instantiate() const;
		void linkMaterial(TGen::MaterialSource & source);
		void unlinkMaterial();
		std::string getMaterialName() const;
		virtual void updateShaderVariable(TGen::ShaderVariable & var, const std::string & name) {}
	
		virtual TGen::Vector3 getMax() const;
		virtual TGen::Vector3 getMin() const;
		virtual TGen::Vector3 getOrigin() const;
		TGen::Material * getMaterial() const;

		TGen::Vector3 min, max, origin;

	protected:
		std::string materialNamePostfix;		
		
	private:
		TGen::Material * material;
		std::string materialName;
	}; 
	
	
} // !TGen

#endif // !_TGEN_RENDERER_MESHINSTANCE_NEW_H
