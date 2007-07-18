/*
 *  renderlist.cpp
 *  The Quaker
 *
 *  Created by Peter Backman on 7/18/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "renderlist.h"
#include "scenenode.h"
#include "camera.h"

RenderList::RenderList() {
	surfaces.reserve(200000);
}

RenderList::~RenderList() {
	
}

void RenderList::Render(TGen::Renderer & renderer, const Camera & camera) {
	std::cout << "surfaces to render: " << surfaces.size() << std::endl;
	
	TGen::Matrix4x4 baseMat = camera.getTransform();
	renderer.setTransform(TGen::TransformProjection, camera.getProjection());
	
	SceneNode * lastNode = NULL;
	
	for (int i = 0; i < surfaces.size(); ++i) {
		SceneNode * thisNode = surfaces[i]->getSceneNode();
		if (thisNode != lastNode) {
			renderer.setTransform(TGen::TransformWorldView, baseMat * thisNode->getTransform());
			lastNode = thisNode;
		}
		
		surfaces[i]->getMaterial()->Render(renderer, *surfaces[i], "default", 9, NULL);
	}
	
	/*TGen::Material * getMaterial();
	void Link(SurfaceLinker & linker);
	void PrepareRender(TGen::Renderer & renderer);
	void Render(TGen::Renderer & renderer);
	SceneNode * getSceneNode() const;
	*/
}

void RenderList::AddSurface(const Surface * surface) {
	surfaces.push_back(surface);
}

void RenderList::Clear() {
	surfaces.clear();
}

// vad behövs för rendrering: transformmatris, material, pekare till metod som rendrerar
// rendrerar = Renderable, transform = SceneNode, material = 
// men vissa geoms kan ha samma transform men olika material?
// minsta del måste vara geom + material.
// en transform har flera geom + material.
// geom + material = surface
// en transform har flera surfaces
// en transform har en AABB
// rättare sagt:
// en scenenode har en transform + AABB + flera surfaces
// Man har en renderable-collection
// sen kan surfaces peka på saker där

// man måste kunna ha samma mesh på olika ställen, med olika AABBs, och olika material
/*
class Surface : public Renderable {
public:
	Surface(materialName, Renderable *)
	getMaterial
	Link(MaterialLinker & linker) // översätter matnamn till mat*
	Prepare
	Render() (renderable->render)
};
 
 det enda slutsteget behöver är transform + surface
 
 */
