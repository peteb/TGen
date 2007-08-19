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

namespace TGen {
	class Face;
	class Camera;
	class Renderer;
	
	class RenderList {
	public:
		RenderList() {}
		virtual ~RenderList() {}
		
		virtual void render(TGen::Renderer & renderer, const TGen::Camera & camera) abstract;
		virtual void addFace(const TGen::Face * face) abstract;
		virtual void sort(const TGen::Camera & camera, const std::string & specialization) abstract;
		virtual void clear() abstract;
		virtual bool needSorting() abstract;
	};
} // !TGen

/*
RenderListCache:
 En samling vb's och ib's i en viss storlek, alignad på batch size. Varje batch har ett inlägg i en lista,
 vbnummer, ibnummer, startib, countib, primitive.
 Det här är absolut snabbaste sättet man kan rendrera på.
 För att kompilera en RenderListCache så sorterar RenderListen faces efter material och försöker göra batches så stora som möjligt.
 Om batchen blir för stor (antalet indices i ib eller vertices i vb) så delas batchen i två.

 Geometries bes att rendrera till batchen, och då kopierar den dit sina vertices och indices bara.
 
 RenderListCache kommer inte snabba upp något typ... level data är redan så optimalt det kan vara .. nästan, sektorer är ju batchbrytare.
 
 RenderListCache måste ha alla vertices i world space. Annars är det ingen vits alls.
 
 Experimentera med det här sen när det finns testdata, ie, en någorlunda värld med modeller som rör sig.

 */

// fixa nu: orientation behandlas som vektor

#endif // !_TGEN_RENDERER_RENDERLIST_H
