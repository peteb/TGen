/*
 *  renderlist.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/14/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

//// summary ///////////////////////////////////////////////////////////////////////////////////////
// Faces are added to a RenderList when rendering, and then sorted. There is no need to re-fill
// the list between multiple renders if the camera hasn't moved or the specialization mode
// is the same. The camera position is the most significant change, a specialization mode change
// only means that the sorting will be the same as with the first specialization. If this
// is a problem, set one global sort level for the material, independent of specializations and LODs.

// A render list can be "compiled" into one or many vertex- and indexbuffers (depending on what's
// possible), if the scene is deemed to rarely change. (RenderListCache) 
// Note that this makes a perfect snapshot of the renderlist which has to be completely updated
// if sorting and/or lods or other frame-to-frame changes start to show. If you bind a RenderList
// to a RenderListCache the RenderList can calculate the difference between the two.
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _TGEN_RENDERER_RENDERLIST_H
#define _TGEN_RENDERER_RENDERLIST_H

#include <vector>
#include <tgen_core.h>
#include <tgen_math.h>
#include "metawriter.h"

namespace TGen {
	class NewFace;
	class Camera;
	class Renderer;
	class VertexStream;
	class SceneNode;
	class MaterialOverride;
	class Material;

	class LodInfo;
	
	class RenderList : public TGen::MetaWriter {
	public:
		RenderList() : override(NULL), materialOverride(NULL) {}
		virtual ~RenderList() {}
		
		struct UserInfo {
			UserInfo(void * data, int value)
				: data(data)
				, value(value)
			{
			}
			
			void * data;
			int value;
		};
		
		void setMaterialOverride(TGen::MaterialOverride * override, int param);
		void setMaterial(TGen::Material * material);
		
		//virtual void writeMeta(uint metaType, const TGen::Matrix4x4 & modelview, TGen::VertexStream & stream) {}
		virtual void render(TGen::Renderer & renderer, const TGen::Matrix4x4 & baseMat, const TGen::LodInfo & lod, const std::string & specialization) abstract;
		virtual void renderWithinRadius(TGen::Renderer & renderer, const TGen::Matrix4x4 & baseMat, const TGen::LodInfo & lod, const TGen::Vector3 & pos, scalar radius) abstract;
		
		virtual void addMeta(TGen::MetaWriter * metaWriter, const TGen::SceneNode * node) abstract;
		virtual void addFace(const TGen::NewFace & face) abstract;
		virtual void addUser(void * user, int id) abstract;
		virtual int getNumFaces() abstract;
		virtual int getNumUserInfo() abstract;
		virtual UserInfo & getUserInfo(int id) abstract;
		virtual void sort(const TGen::Camera & camera, const std::string & specialization) abstract;
		virtual void clear() abstract;
		virtual bool needSorting() abstract;
		
	protected:
		TGen::MaterialOverride * override;
		int overrideParam;
		TGen::Material * materialOverride;
	};
} // !TGen

/*
RenderListCache:
 En samling vb's och ib's i en viss storlek, alignad p� batch size. Varje batch har ett inl�gg i en lista,
 vbnummer, ibnummer, startib, countib, primitive.
 Det h�r �r absolut snabbaste s�ttet man kan rendrera p�.
 F�r att kompilera en RenderListCache s� sorterar RenderListen faces efter material och f�rs�ker g�ra batches s� stora som m�jligt.
 Om batchen blir f�r stor (antalet indices i ib eller vertices i vb) s� delas batchen i tv�.

 Geometries bes att rendrera till batchen, och d� kopierar den dit sina vertices och indices bara.
 
 RenderListCache kommer inte snabba upp n�got typ... level data �r redan s� optimalt det kan vara .. n�stan, sektorer �r ju batchbrytare.
 
 RenderListCache m�ste ha alla vertices i world space. Annars �r det ingen vits alls.
 
 Experimentera med det h�r sen n�r det finns testdata, ie, en n�gorlunda v�rld med modeller som r�r sig.

 */

// fixa nu: orientation behandlas som vektor

#endif // !_TGEN_RENDERER_RENDERLIST_H
