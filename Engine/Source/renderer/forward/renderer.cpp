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
	, currentLightMaterial(NULL)
{
	depthPassMaterial = resources.getMaterial("forward/depth");
	TGen::Rectangle size = TGen::Rectangle(512, 512);
	shadowMap = renderer.createTexture(size, TGen::DEPTH32, TGen::TypeFloat, TGen::TextureNoMipmaps);	
	crapMap = renderer.createTexture(size, TGen::RGBA, TGen::TypeUnsignedByte, TGen::TextureNoMipmaps);
	//lightMap = resources.getTexture("textures/flashlight5.tga");
	
	shadowMapTarget = renderer.createFrameBuffer();
	shadowMapTarget->attachColor(crapMap);
	shadowMapTarget->attachDepth(shadowMap);
}

TGen::Engine::ForwardRenderer::~ForwardRenderer() {
	delete shadowMapTarget;
	delete shadowMap;
	delete crapMap;
}


#include <OpenGL/OpenGL.h>

void TGen::Engine::ForwardRenderer::renderWorld(TGen::Engine::World & world, TGen::Camera * camera, scalar dt) {
	world.prepareLists(camera);
	TGen::RenderList & renderList = world.getRenderList();
	TGen::Engine::LightList & lights = world.getLightList();
	
	renderList.sort(*camera, "default");
	
	renderer.setClearColor(TGen::Color::Black);
	renderer.clearBuffers(TGen::ColorBuffer | TGen::DepthBuffer);
	renderer.setTransform(TGen::TransformProjection, camera->getProjection());
	renderer.setAmbientLight(world.getAmbientLight());		


	renderDepth(renderList, camera);
	

	

	for (int i = 0; i < lights.getNumLights(); ++i) {
		TGen::Engine::Light * light = lights.getLight(i);
		
		renderShadowmap(renderList, light);
		
		shadowMatrix *= camera->getTransform().getInverse();
		
		// STATIC AND DYNAMIC shadows, lampor ska kunna markera att skuggorna inte behövs räknas om alls
		

		renderList.setMaterialOverride(this, 1);
		renderList.setMaterial(NULL);

		currentPass = LightPass;

		renderer.setTexture(5, shadowMap);
		renderer.setTextureTransform(5, shadowMatrix);
		
		light->getLightProperties().position = TGen::Vector4(0.0f, 0.0f, 0.0f, 1.0f);
		light->getLightProperties().spotDirection = TGen::Vector3(0.0f, 0.0f, 1.0f);
		light->getLightProperties().spotExponent = 1.0f;
		light->getLightProperties().spotCutoff = 35.0f;
		
		float radius = light->getLightProperties().calculateAttenuationDistance(1.0f, 1000.0f);
		
		//std::cout << radius << std::endl;
		
		// TODO: olika sorting modes, kolla upp dem. se om man kan sätta det för sky
		
		renderer.setTransform(TGen::TransformProjection, camera->getProjection());
		renderer.setTransform(TGen::TransformWorldView, camera->getTransform() * light->getTransform());
		renderer.setLight(0, light->getLightProperties());

		currentLightMaterial = light->getMaterial();
		
		renderList.renderWithinRadius(renderer, camera->getTransform(), camera->getLod(), camera->getTransform() * light->getTransform() * TGen::Vector3(0.0f, 0.0f, 0.0f), radius);
	}
	//}
}

void TGen::Engine::ForwardRenderer::renderDepth(TGen::RenderList & renderList, TGen::Camera * camera) {
	currentPass = DepthPass;
	
	renderList.setMaterialOverride(this, 1);
	renderList.setMaterial(depthPassMaterial);
		
	renderList.render(renderer, camera->getTransform(), camera->getLod(), "default");	
}

void TGen::Engine::ForwardRenderer::renderShadowmap(TGen::RenderList & renderList, TGen::Engine::Light * light) {
	TGen::Matrix4x4 lightProjection = TGen::Matrix4x4::PerspectiveProjection(TGen::Degree(50.0f), 1.0f, 0.1f, 20.0f);
	TGen::Matrix4x4 prevProjection = renderer.getTransform(TGen::TransformProjection);
	TGen::Rectangle prevView = renderer.getViewport();
	
	currentPass = ShadowPass;
	
	renderList.setMaterialOverride(this, 1);
	renderList.setMaterial(depthPassMaterial);
	
	renderer.setRenderTarget(shadowMapTarget);
	renderer.setViewport(shadowMap->size);
	renderer.setTransform(TGen::TransformProjection, lightProjection);
	renderer.clearBuffers(TGen::DepthBuffer);
	
	TGen::Matrix4x4 viewMat = light->getTransform(); 
	viewMat.setZ(-viewMat.getZ());
	viewMat.invert();


	shadowFrustum = TGen::Frustum(lightProjection, viewMat);

	TGen::LodInfo lod;	
	renderList.render(renderer, viewMat, lod, "default");
	
	shadowMatrix = TGen::Matrix4x4::Bias(TGen::Vector3(0.5f)) 
		* lightProjection 
		* viewMat;
	
	

	
	renderer.setTransform(TGen::TransformProjection, prevProjection);
	renderer.setRenderTarget(NULL);
	renderer.setViewport(prevView);
}


void TGen::Engine::ForwardRenderer::overrideMaterial(TGen::Renderer & renderer, int param) const {	


	if (currentPass == DepthPass) {
		renderer.setColorWrite(false);
		renderer.setDepthFunc(TGen::CompareLessOrEqual);
	}
	else if (currentPass == ShadowPass) {
		renderer.setColorWrite(false);
		renderer.setDepthFunc(TGen::CompareLessOrEqual);

		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);
		
	}
	else if (currentPass == LightPass) {
		renderer.setColorWrite(true);
		renderer.setDepthFunc(TGen::CompareEqual);
		renderer.setBlendFunc(TGen::BlendOne, TGen::BlendOne);
		
		
		// TODO: bra optimering, spara frustum planes från shadowrendreringen och använd dem som user clip planes
		//       när lampan rendreras!
		
	//renderer.setTexture(6, lightMap);
		
		
		if (currentLightMaterial) {
			TGen::Technique * tech = currentLightMaterial->getSpecialization("default");
			TGen::PassList * passes = tech->getPassList(9);
			TGen::Pass * pass = passes->getPass(0);
			const TGen::RenderContext & context = pass->getRenderContext(0);
		 
			TGen::TextureUnit * texunit = context.textureUnits.at(0);
			renderer.setTexture(4, texunit->getTexture());
			renderer.setTextureTransform(4, texunit->transform);
		}
		
		glActiveTexture(GL_TEXTURE5);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LESS );

		glActiveTexture(GL_TEXTURE4);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glActiveTexture(GL_TEXTURE5);
		
		
		/*
		glTexParameteri( GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY );
		glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
		*/
		try {
			//static int hey = 0;
			
			//if (hey++ < 10) {
				renderer.getShaderProgram()->getUniform("shadowMap") = 5;
				renderer.getShaderProgram()->getUniform("lightMap") = 4;
			//}
		}
		catch (...) {
			
		}
		
	}

//	if (currentPass != ShadowPass)
	//	glCullFace(lastCull);

}

