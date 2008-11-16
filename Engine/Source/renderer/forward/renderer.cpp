/*
 *  renderer.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 11/6/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "renderer/forward/renderer.h"
#include "world.h"
#include <tgen_renderer.h>
#include <tgen_graphics.h>
#include <tgen_math.h>
#include "light.h"
#include "resourcemanager.h"

TGen::Engine::ForwardRenderer::ForwardRenderer(TGen::Renderer & renderer, TGen::Engine::ResourceManager & resources)
	: TGen::Engine::WorldRenderer(renderer)
	, depthPassMaterial(NULL)
{
	depthPassMaterial = resources.getMaterial("forward/depth");
	TGen::Rectangle size = TGen::Rectangle(2048, 2048);
	shadowMap = renderer.createTexture(size, TGen::DEPTH24, TGen::TypeUnsignedByte, TGen::TextureNoMipmaps);	
	crapMap = renderer.createTexture(size, TGen::RGBA, TGen::TypeUnsignedByte, TGen::TextureNoMipmaps);
	
	shadowMapTarget = renderer.createFrameBuffer();
	shadowMapTarget->attachColor(crapMap);
	shadowMapTarget->attachDepth(shadowMap);
}

TGen::Engine::ForwardRenderer::~ForwardRenderer() {
	delete shadowMapTarget;
	delete shadowMap;
	delete crapMap;
}

void TGen::Engine::ForwardRenderer::renderWorld(TGen::Engine::World & world, TGen::Camera * camera, scalar dt) {
	//std::cout << "DT: " << dt * 1000.0 << std::endl;
	
	world.prepareLists(camera);
	TGen::RenderList & renderList = world.getRenderList();
	TGen::Engine::LightList & lights = world.getLightList();
	
	renderList.sort(*camera, "default");
	
	static float sumDt = 0.0;
	sumDt += dt;
	
	
	renderer.clearBuffers(TGen::ColorBuffer | TGen::DepthBuffer);

	renderer.setTransform(TGen::TransformProjection, camera->getProjection());


	renderList.setMaterialOverride(this, 1);
	renderList.setMaterial(depthPassMaterial);

	TGen::Rectangle view = renderer.getViewport();
	
	{
		currentPass = ShadowPass;
		
		renderer.setRenderTarget(shadowMapTarget);
		renderer.clearBuffers(TGen::DepthBuffer);
	
		renderer.setViewport(shadowMap->size);
		TGen::Matrix4x4 viewMat = lights.getLight(0)->getTransform(); 
		viewMat.setZ(-viewMat.getZ());
		viewMat.invert();
	
		renderList.render(renderer, viewMat, camera->getLod(), "default");
		
		TGen::Matrix4x4 biasMatrix(0.5f, 0.0f, 0.0f, 0.5f,
											0.0f, 0.5f, 0.0f, 0.5f,
											0.0f, 0.0f, 0.5f, 0.5f,
											0.0f, 0.0f, 0.0f, 1.0f);
				

		shadowMatrix = camera->getProjection() * viewMat * camera->getTransform().getInverse();

	}
	
	currentPass = DepthPass;

	renderer.setRenderTarget(NULL);
	renderer.setViewport(view);
	

//	if (!(int(sumDt) % 3)) {
	renderer.setClearColor(TGen::Color::Black);
	renderer.setAmbientLight(world.getAmbientLight());
	
	// DEPTH PASS FIRST
	
	
	renderList.render(renderer, camera->getTransform(), camera->getLod(), "default");
	
	// här kan man rendrera portalerna också för att få occlusion info
	// TODO: hur synkat är det med fysiken? man borde göra grafiken, sen fysiken, fast spelar nog ingen stor roll
	
	
	
	
	
	
	
	
	currentPass = LightPass;
	
	renderList.setMaterialOverride(this, 1);
	renderList.setMaterial(NULL);
	
	for (int i = 0; i < lights.getNumLights(); ++i) {
		TGen::Engine::Light * light = lights.getLight(i);
		
		light->getLightProperties().position = TGen::Vector4(0.0f, 0.0f, 0.0f, 1.0f); //light->getWorldPosition();
		
		float radius = 0.0f;
		
		for (float a = 0.0f; ; a += 0.1f) {
			if (1.0f / (light->getLightProperties().constantAttenuation +
								light->getLightProperties().linearAttenuation * a +
							light->getLightProperties().quadraticAttenuation * a * a) < 0.1) {
				radius = a;
				break;
			}
		}
		
		// TODO: olika sorting modes, kolla upp dem. se om man kan sätta det för sky
		
		renderer.setTransform(TGen::TransformWorldView, camera->getTransform() * light->getTransform());
		renderer.setLight(0, light->getLightProperties());

		// TODO: renderList.only render everything within radius from point
		
		//renderList.render(renderer, *camera, "default");
		
		renderList.renderWithinRadius(renderer, camera->getTransform(), camera->getLod(), camera->getTransform() * light->getTransform() * TGen::Vector3(0.0f, 0.0f, 0.0f), radius);
	}
	//}
}

#include <OpenGL/OpenGL.h>

void TGen::Engine::ForwardRenderer::overrideMaterial(TGen::Renderer & renderer, int param) const {	
	glDisable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(0.0, 0.0);
	//glCullFace(GL_BACK);
	static int lastCull = GL_BACK;
	
	glDisable(GL_CULL_FACE);
	
	glPolygonMode(GL_BACK, GL_POINT);
	glPolygonMode(GL_FRONT, GL_FILL);
	
	if (currentPass == DepthPass) {
		renderer.setColorWrite(false);
		renderer.setDepthFunc(TGen::CompareLessOrEqual);
	}
	else if (currentPass == ShadowPass) {
		renderer.setColorWrite(false);
		renderer.setDepthFunc(TGen::CompareLessOrEqual);

		glPolygonOffset(0.015, 0.0);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glGetIntegerv(GL_CULL_FACE_MODE, &lastCull);
		
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glPolygonMode(GL_BACK, GL_FILL);
		glPolygonMode(GL_FRONT, GL_POINT);
		
	}
	else if (currentPass == LightPass) {
		renderer.setColorWrite(true);
		renderer.setDepthFunc(TGen::CompareEqual);
		renderer.setBlendFunc(TGen::BlendOne, TGen::BlendOne);
		renderer.setTexture(5, shadowMap);
		renderer.setTextureTransform(5, shadowMatrix);
		
		//glIsTexture(1);
		
		glActiveTexture(GL_TEXTURE5);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		
		glTexParameteri( GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LESS );
		//glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
		try {
			renderer.getShaderProgram()->getUniform("shadowMap") = 5;
		}
		catch (...) {
			
		}
		
	}

	if (currentPass != ShadowPass)
		glCullFace(lastCull);

}

