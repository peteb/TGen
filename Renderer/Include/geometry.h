/*
 *  geometry.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/12/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

//// summary ///////////////////////////////////////////////////////////////////////////////////////
// Geometry is the shape of a face.
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _TGEN_RENDERER_GEOMETRY_H
#define _TGEN_RENDERER_GEOMETRY_H

#include <tgen_graphics.h>

namespace TGen {
	class Camera;
	class ShaderVariable;
	
	class Subface : public TGen::Renderable {
	public:	
		virtual ~Subface() {}
	
		virtual void preRender(TGen::Renderer & renderer) const abstract;
		virtual void render(TGen::Renderer & renderer) const abstract;
		
	};
	
	class Geometry : public TGen::Renderable, public TGen::ShaderVariableUpdater {
	public:	
		Geometry() {}
		virtual ~Geometry();
		
		typedef std::vector<TGen::Subface *> SubfaceList;
		
		virtual void preRender(TGen::Renderer & renderer) const abstract;		// set vb's, ib's, etc.
		virtual void render(TGen::Renderer & renderer) const abstract;
		virtual void update(const TGen::Camera & camera, scalar distance, scalar time) {}	// LOD, billboards, etc. called on geoms in render list, animating meshes
		
		virtual TGen::Vector3 getMax() const abstract;
		virtual TGen::Vector3 getMin() const abstract;
		virtual TGen::Vector3 getOrigin() const abstract;
		
		virtual std::string getDefaultMaterial() const {return ""; }	// throw istället kanske? BORT KANSKE!
		virtual void updateShaderVariable(TGen::ShaderVariable & var, const std::string & name) {}

		virtual const SubfaceList * getLeaves() const;
		void addLeaf(TGen::Subface * leaf);
		
	protected:
		SubfaceList leaves;
	};
	
	// TODO: md3-meshes anger ju material. faces drar material från geometry om facens material är ""
} // !TGen

#endif // !_TGEN_RENDERER_GEOMETRY_H
