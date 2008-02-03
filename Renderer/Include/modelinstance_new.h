/*
 *  modelinstance_new.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 2/3/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_MODELINSTANCE_NEW_H
#define _TGEN_RENDERER_MODELINSTANCE_NEW_H

#include <vector>
#include <tgen_core.h>
#include "modeljoint.h"

// ModelInstance OWNS MeshInstances, when the ModelInstance dies, the MeshInstances die too

namespace TGen {
	class MaterialSource;
	class NewMeshInstance;
	class RenderList;
	class SceneNode;
	
	class NewModelInstance {
	public:
		NewModelInstance(const std::string & name);
		virtual ~NewModelInstance();
		
		typedef std::vector<TGen::NewMeshInstance *> MeshList;

		virtual void update();
		virtual bool isPureInstance() const abstract;
		virtual TGen::ModelJoint getJoint(const std::string & name) const;		
		virtual TGen::NewModelInstance * operator *() {return this; }
		
		void linkMaterial(TGen::MaterialSource & source);
		void unlinkMaterial();
		void addMeshInstance(TGen::NewMeshInstance * instance);
		void fillFaces(TGen::RenderList & list, TGen::SceneNode const * node);
		
		MeshList & getMeshes();
		std::string getName() const;
		
	protected:
		MeshList meshes;
		std::string name;
	};
	
} // !TGen

#endif // !_TGEN_RENDERER_MODELINSTANCE_NEW_H
