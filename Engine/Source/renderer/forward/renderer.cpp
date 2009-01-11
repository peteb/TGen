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
	
	glDisable(GL_SCISSOR_TEST);
	renderer.setClearColor(TGen::Color::Black);
	renderer.clearBuffers(TGen::ColorBuffer | TGen::DepthBuffer);
	renderer.setTransform(TGen::TransformProjection, camera->getProjection());
	renderer.setAmbientLight(world.getAmbientLight());		


	renderDepth(renderList, camera);
	

	

	for (int i = 0; i < lights.getNumLights(); ++i) {
		TGen::Engine::Light * light = lights.getLight(i);
		
		uint dirs = light->getDirections();
		
		if (dirs & DirPosX)
			renderLight(light, TGen::Matrix4x4::RotationY(TGen::Degree(90.0f)), renderList, camera);
		if (dirs & DirNegX)
			renderLight(light, TGen::Matrix4x4::RotationY(TGen::Degree(-90.0f)), renderList, camera);
			
		if (dirs & DirPosZ)
			renderLight(light, TGen::Matrix4x4::Identity, renderList, camera);
		if (dirs & DirNegZ)
			renderLight(light, TGen::Matrix4x4::RotationY(TGen::Degree(-180.0f)), renderList, camera);
		
		if (dirs & DirPosY)
			renderLight(light, TGen::Matrix4x4::RotationX(TGen::Degree(-90.0f)), renderList, camera);
		if (dirs & DirNegY)
			renderLight(light, TGen::Matrix4x4::RotationX(TGen::Degree(90.0f)), renderList, camera);
		
		// TODO: calculate all intersecting points of the shadow frustum, then construct bounding box in screen space
		//      if all points are outside ordinary frustum range, don't draw light at all. don't even calculate shadow map
		
	}
	//}
}

void TGen::Engine::ForwardRenderer::renderLight(TGen::Engine::Light * light, const TGen::Matrix4x4 & transform, TGen::RenderList & renderList, TGen::Camera * camera) {
	renderShadowmap(renderList, light, transform);
	
	

	shadowMatrix *= camera->getTransform().getInverse();
	shadowFrustumMat *= camera->getTransform().getInverse();
	
	TGen::Matrix4x4 shadowInverse = shadowFrustumMat.getInverse();
	
	TGen::Vector4 nearPlane[4];
	TGen::Vector4 farPlane[4];
	
	nearPlane[0] = shadowInverse * TGen::Vector3(-1.0f, 1.0f, -1.0f);
	nearPlane[1] = shadowInverse * TGen::Vector3( 1.0f, 1.0f, -1.0f);
	nearPlane[2] = shadowInverse * TGen::Vector3( 1.0f, -1.0f, -1.0f);
	nearPlane[3] = shadowInverse * TGen::Vector3(-1.0f, -1.0f, -1.0f);

	farPlane[0] = shadowInverse * TGen::Vector3(-1.0f, 1.0f, 1.0f);
	farPlane[1] = shadowInverse * TGen::Vector3( 1.0f, 1.0f, 1.0f);
	farPlane[2] = shadowInverse * TGen::Vector3( 1.0f, -1.0f, 1.0f);
	farPlane[3] = shadowInverse * TGen::Vector3(-1.0f, -1.0f, 1.0f);
	
	
	TGen::Vector4 projed[8];
	
	TGen::Vector3 min, max;
	
	TGen::Matrix4x4 trans = camera->getProjection();

	int count = 0;
	
	for (int i = 0; i < 4; ++i) {
		TGen::Vector4 hey1 = TGen::Vector4(trans * nearPlane[i]);
		TGen::Vector4 hey2 = TGen::Vector4(trans * farPlane[i]);

		
		if (nearPlane[i].z < -1.0f)
			count++;
		if (farPlane[i].z < -1.0f)
			count++;
		
		projed[i * 2 + 0] = hey1;
		projed[i * 2 + 1] = hey2;
	}

	bool firstMin = true, firstMax = true;

	if (count == 0) {
		//std::cout << "outside" << std::endl;
		//return;
	}
	else {
		//std::cout << "Inside: " << count << std::endl;
	}
	
	for (int i = 0; i < 8; ++i) {
		projed[i] /= projed[i].w;
		

		
		projed[i].x = TGen::Clamp(projed[i].x, -1.0f, 1.0f);
		projed[i].y = TGen::Clamp(projed[i].y, -1.0f, 1.0f);
		projed[i].z = TGen::Clamp(projed[i].z, -1.0f, 1.0f);
		

		if (!firstMin) {
			min.x = std::min(min.x, projed[i].x);
			min.y = std::min(min.y, projed[i].y);
			min.z = std::min(min.z, projed[i].z);
		}
		else {
			min = projed[i];
			firstMin = false;
		}
		
		if (!firstMax) {
			max.x = std::max(max.x, projed[i].x);
			max.y = std::max(max.y, projed[i].y);
			max.z = std::max(max.z, projed[i].z);

		}
		else {
			max = projed[i];
			firstMax = false;
		}
		
	}
	

	
	
	min = min * 0.5 + 0.5;
	max = max * 0.5 + 0.5;
	
	TGen::Vector2 boxmin = TGen::Vector2(min) * TGen::Vector2(640.0f, 480.0f);
	TGen::Vector2 boxmax = TGen::Vector2(max) * TGen::Vector2(640.0f, 480.0f);
	
	TGen::Vector2 delta = boxmax - boxmin;
	if (boxmax.x == 0.0f || boxmax.y == 0.0f) {
		//std::cout << "no area" << std::endl;
		return;
	}
	
	
	glEnable(GL_SCISSOR_TEST);
	glScissor(boxmin.x,  boxmin.y, boxmax.x - boxmin.x, boxmax.y - boxmin.y);
	
	
//	std::cout << std::string(boxmin) << " = " << std::string(boxmax) << std::endl;
	
	renderList.setMaterialOverride(this, 1);
	renderList.setMaterial(NULL);
	
	currentPass = LightPass;
	
	renderer.setTexture(5, shadowMap);
	renderer.setTextureTransform(5, shadowMatrix);
	
	light->getLightProperties().position = TGen::Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	light->getLightProperties().spotDirection = transform * TGen::Vector3(0.0f, 0.0f, 1.0f);
	light->getLightProperties().spotExponent = 1.0f;
	light->getLightProperties().spotCutoff = 35.0f;
	
	float radius = light->getLightProperties().calculateAttenuationDistance(1.0f, 1000.0f);
	
	//std::cout << radius << std::endl;
	
	// TODO: olika sorting modes, kolla upp dem. se om man kan sätta det för sky
	
	renderer.setTransform(TGen::TransformProjection, camera->getProjection());
	renderer.setTransform(TGen::TransformWorldView, camera->getTransform() * light->getTransform());
	renderer.setLight(0, light->getLightProperties());
	
	currentLightMaterial = light->getMaterial();

	/*glPushAttrib(GL_ALL_ATTRIB_BITS);
	
	glDepthFunc(GL_ALWAYS);
	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glDisable(GL_COLOR_MATERIAL);
	glColorMask(true, true, true, true);
	
	renderer.setShaderProgram(NULL);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glLineWidth(2.0f);
	
	glBegin(GL_LINE_LOOP);
	glColor3f(1.0f, 0.0f, 0.0f);
	
	for (int i = 0; i < 4; ++i) {
		glVertex3f(nearPlane[i].x, nearPlane[i].y, nearPlane[i].z);
	}	

	glEnd();
	glBegin(GL_LINE_LOOP);

	
	glColor3f(1.0f, 1.0f, 0.0f);
	
	for (int i = 0; i < 4; ++i) {
		glVertex3f(farPlane[i].x, farPlane[i].y, farPlane[i].z);
	}	
	
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.0f, 1.0f, 0.0f);
	
	for (int i = 0; i < 4; ++i) {
		glVertex3f(nearPlane[i].x, nearPlane[i].y, nearPlane[i].z);
		glVertex3f(farPlane[i].x, farPlane[i].y, farPlane[i].z);
		
	}
	
	glEnd();
	glPopAttrib();
	*/
	
	renderList.renderWithinRadius(renderer, camera->getTransform(), camera->getLod(), camera->getTransform() * light->getTransform() * TGen::Vector3(0.0f, 0.0f, 0.0f), radius);	
	
	glDisable(GL_SCISSOR_TEST);
}

void TGen::Engine::ForwardRenderer::renderDepth(TGen::RenderList & renderList, TGen::Camera * camera) {
	currentPass = DepthPass;
	
	renderList.setMaterialOverride(this, 1);
	renderList.setMaterial(depthPassMaterial);
		
	renderList.render(renderer, camera->getTransform(), camera->getLod(), "default");	
}

void TGen::Engine::ForwardRenderer::renderShadowmap(TGen::RenderList & renderList, TGen::Engine::Light * light, const TGen::Matrix4x4 & transform) {
	TGen::Matrix4x4 lightProjection = TGen::Matrix4x4::PerspectiveProjection(TGen::Degree(30.0), 1.0f, 0.5f, 5.0f);
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
	viewMat *= transform; 
	viewMat.setZ(-viewMat.getZ());
	viewMat.invert();

	// TODO: lightDirection, spotCutoff, shadow direction
	// TODO: skicka in en transform till funktionen som multiplicerar viewMat
	// TODO: kolla mot near clip plane i shadern. rätt clip plane distance!
	
	shadowFrustum = TGen::Frustum(lightProjection, viewMat);

	TGen::LodInfo lod;	
	renderList.render(renderer, viewMat, lod, "default");
	
	shadowMatrix = TGen::Matrix4x4::Bias(TGen::Vector3(0.5f)) 
		* lightProjection 
		* viewMat;
	
	shadowFrustumMat = lightProjection * viewMat;

	
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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LESS );

		glActiveTexture(GL_TEXTURE4);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
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

