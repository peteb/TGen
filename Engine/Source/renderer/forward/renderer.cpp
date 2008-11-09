/*
 *  renderer.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 11/6/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "renderer/newrenderer/renderer.h"
#include "world.h"
#include <tgen_renderer.h>
#include <tgen_graphics.h>
#include "light.h"

TGen::Engine::NewRenderer::NewRenderer(TGen::Renderer & renderer)
	: TGen::Engine::WorldRenderer(renderer)
{
}

TGen::Engine::NewRenderer::~NewRenderer() {
	
}

// TODO: ett deffat värde för output-shader, riktiga shadern väljs beroende på vilken renderer man har
/*
	$output
	$lightpos
	$lightdir
 
 */

#include <OpenGL/OpenGL.h>

void TGen::Engine::NewRenderer::renderWorld(TGen::Engine::World & world, TGen::Camera * camera, scalar dt) {
	world.prepareLists(camera);
	TGen::RenderList & renderList = world.getRenderList();
	TGen::Engine::LightList & lights = world.getLightList();
	
	renderList.sort(*camera, "default");
	
	renderer.setClearColor(TGen::Color::Black);
	renderer.clearBuffers(TGen::ColorBuffer | TGen::DepthBuffer);
	renderer.setAmbientLight(world.getAmbientLight());

	//std::cout << "render faces: " << std::dec <<  renderList.getNumFaces() << " user info: " << renderList.getNumUserInfo() << std::endl;
	
	for (int i = 0; i < lights.getNumLights(); ++i) {
		TGen::Engine::Light * light = lights.getLight(i);
		
		light->getLightProperties().position = TGen::Vector4(0.0f, 0.0f, 0.0f, 1.0f); //light->getWorldPosition();
		
		//std::cout << "light " << i << " " << std::string(light->getWorldPosition()) << std::endl;
		
		renderer.setTransform(TGen::TransformWorldView, camera->getTransform() * light->getTransform());
		renderer.setLight(i, light->getLightProperties());
	}
	
	/*glPolygonMode(GL_FRONT, GL_LINES);
	glPolygonMode(GL_BACK, GL_FILL);
	glDisable(GL_CULL_FACE);
	*/
	
	// TODO: renderList.render(overrideMaterial), use ONLY that material
	/*
	 depthPass:
	 renderList.render(renderer, *camera, "default", depthPassMaterial);
	 
	 renderList.overrideDepthPass(DepthEqual);
	 
	 renderList.render(renderer, *camera, "default", NULL);
	 
	 
	 
	 */
	
	renderList.render(renderer, *camera, "default");

}
