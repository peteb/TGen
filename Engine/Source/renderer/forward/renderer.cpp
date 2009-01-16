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
	renderer.setRenderTarget(NULL);
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
	TGen::Matrix4x4 lightProjection = calculateLightProjection(*light);
	TGen::Matrix4x4 viewMat = calculateLightModelView(light->getTransform() * transform);

	shadowMatrix = TGen::Matrix4x4::Bias(TGen::Vector3(0.5f)) * lightProjection * viewMat * camera->getTransform().getInverse();
	shadowFrustumMat = lightProjection * viewMat;

	TGen::Rectangle scissorBox;
	
	if (!calculateFrustumBox(scissorBox, shadowFrustumMat, camera->getProjection(), camera->getTransform()))
		return;
	
	{
		TGen::Matrix4x4 prevProjection = renderer.getTransform(TGen::TransformProjection);
	
		renderer.setTransform(TGen::TransformProjection, lightProjection);
		renderShadowmap(renderList, light, viewMat);
		renderer.setTransform(TGen::TransformProjection, prevProjection);
	}
	
	
	
	
	
	
	glEnable(GL_SCISSOR_TEST);
	glScissor(scissorBox.getMin().x, scissorBox.getMin().y, scissorBox.getMax().x - scissorBox.getMin().x, scissorBox.getMax().y - scissorBox.getMin().y);
	
	
	
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

	renderList.renderWithinRadius(renderer, camera->getTransform(), camera->getLod(), camera->getTransform() * light->getTransform() * TGen::Vector3(0.0f, 0.0f, 0.0f), radius);	
	
	glDisable(GL_SCISSOR_TEST);
}

TGen::Matrix4x4 TGen::Engine::ForwardRenderer::calculateLightProjection(const TGen::Engine::Light & light) {
	return TGen::Matrix4x4::PerspectiveProjection(TGen::Degree(30.0), 1.0f, 0.5f, 15.0f);
}

TGen::Matrix4x4 TGen::Engine::ForwardRenderer::calculateLightModelView(const TGen::Matrix4x4 & lightTransform) {
	TGen::Matrix4x4 viewMat = lightTransform; 
	viewMat.setZ(-viewMat.getZ());
	viewMat.invert();

	return viewMat;
}

bool TGen::Engine::ForwardRenderer::calculateFrustumBox(TGen::Rectangle & outRectangle, const TGen::Matrix4x4 & frustumTransform, const TGen::Matrix4x4 & cameraProj, const TGen::Matrix4x4 & cameraTransform) {
	// TODO: allow possibility to render frustum in other function, but not through here. this should remain static
	
	TGen::Matrix4x4 shadowInverse = frustumTransform.getInverse();
	
	TGen::Vector4 nearPlane[4];
	TGen::Vector4 farPlane[4];
	
	nearPlane[0] = shadowInverse * TGen::Vector4(-1.0f, 1.0f, -1.0f, 1.0f);
	nearPlane[1] = shadowInverse * TGen::Vector4( 1.0f, 1.0f, -1.0f, 1.0f);
	nearPlane[2] = shadowInverse * TGen::Vector4( 1.0f, -1.0f, -1.0f, 1.0f);
	nearPlane[3] = shadowInverse * TGen::Vector4(-1.0f, -1.0f, -1.0f, 1.0f);
	
	farPlane[0] = shadowInverse * TGen::Vector4(-1.0f, 1.0f, 1.0f, 1.0f);
	farPlane[1] = shadowInverse * TGen::Vector4( 1.0f, 1.0f, 1.0f, 1.0f);
	farPlane[2] = shadowInverse * TGen::Vector4( 1.0f, -1.0f, 1.0f, 1.0f);
	farPlane[3] = shadowInverse * TGen::Vector4(-1.0f, -1.0f, 1.0f, 1.0f);
	
	for (int i = 0; i < 4; ++i) {
		nearPlane[i] /= nearPlane[i].w;
		farPlane[i] /= farPlane[i].w;
	}
	
	TGen::Frustum frustum = calculateFrustum(frustumTransform);
	TGen::Vector3 cameraPos = cameraTransform.getInverse().getOrigin(); // * TGen::Vector3::Zero;

	//if (frustum.intersects(cameraPos))
	//	std::cout << "hey" << std::endl;

	
	
	std::vector<std::pair<TGen::Vector3, TGen::Vector3> > edges, fixedEdges;
	
	for (int i = 0; i < 4; ++i)
		edges.push_back(std::make_pair(nearPlane[i], farPlane[i]));
	
	for (int i = 0; i < 4; ++i) {
		int next = (i < 3 ? i + 1 : 0);
		edges.push_back(std::make_pair(nearPlane[i], nearPlane[next]));
	}
	
	for (int i = 0; i < 4; ++i) {
		int next = (i < 3 ? i + 1 : 0);
		edges.push_back(std::make_pair(farPlane[i], farPlane[next]));
	}
	
	
	
	TGen::Vector4 projed[8];
	TGen::Vector3 min, max;
	
	int count = 0;
	
	
	// calculate screenspace 
	for (int i = 0; i < 4; ++i) {
		TGen::Vector4 hey1 = TGen::Vector4(cameraProj * cameraTransform * nearPlane[i]);
		TGen::Vector4 hey2 = TGen::Vector4(cameraProj * cameraTransform * farPlane[i]);
		
		
		if (nearPlane[i].z >= -1.0f)
			count++;
		if (farPlane[i].z >= -1.0f)
			count++;
		
		projed[i * 2 + 0] = hey1;
		projed[i * 2 + 1] = hey2;
	}
	
	bool firstMin = true, firstMax = true;
	
	TGen::Frustum camFrustum = calculateFrustum(cameraProj * cameraTransform);
	
	for (int i = 0; i < edges.size(); ++i) {
		TGen::Vector3 p1 = edges[i].first;
		TGen::Vector3 p2 = edges[i].second;

		
		bool add = false;

		TGen::Vector3 np1 = p1, np2 = p2;
		
		for (int a = 0; a < camFrustum.getNumPlanes(); ++a) {
			const TGen::Plane3 plane = camFrustum.getPlane(a);
			
			if ((plane.getDistanceTo(np1) >= 0.0f) != (plane.getDistanceTo(np2) >= 0.0f)) {
				TGen::Vector3 newP1, newP2;

				if (plane.getDistanceTo(np1) >= 0.0f) {		// p1 is on the visible side
					newP1 = np1;
					newP2 = plane.intersectRay(np1, np2 - np1);
				}
				else {				// p2 is on the visible side
					newP1 = np2;
					newP2 = plane.intersectRay(np2, np1 - np2);
				}
				
				np1 = newP1;
				np2 = newP2;
				
				add = true;
			}

		}
		
		//if (!camFrustum.intersects(np1) && !camFrustum.intersects(np2))
		//	add = false;
		int icount1 = 0, icount2 = 0;
		
		for (int b = 0; b < camFrustum.getNumPlanes(); ++b) {
			if (camFrustum.getPlane(b).getDistanceTo(np1) >= 0.0f)
				icount1++;
			if (camFrustum.getPlane(b).getDistanceTo(np2) >= 0.0f)
				icount2++;
		}
		
		//if (icount1 < 5 && icount2 < 5)
		//	add = false;
		
		if (add)
			fixedEdges.push_back(std::make_pair(np1, np2));
	}
		
	
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf((GLfloat *)cameraTransform.elements);

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf((GLfloat *)cameraProj.elements);

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		 
		 glDepthFunc(GL_ALWAYS);
		 glActiveTexture(GL_TEXTURE0);
		 glDisable(GL_TEXTURE_2D);
		 glDisable(GL_LIGHTING);
		 glDisable(GL_BLEND);
		 glDisable(GL_COLOR_MATERIAL);
		 glColorMask(true, true, true, true);
		 
		glUseProgram(0);
		 
		 glLineWidth(1.0f);
	
		
		
		
		glPointSize(4.0f);
		glEnable(GL_POINT_SMOOTH);
		glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
		
		glBegin(GL_POINTS);
		
		glColor3f(0.0f, 1.0f, 0.0f);
		
		for (int i = 0; i < fixedEdges.size(); ++i) {
			//glColor3f(float((i + 1) & 0x3) / 4.0f, float(i >> 2 & 0x3) / 3.0f, float(i >> 4 & 0xC) / 3.0f);
			glVertex3fv(&fixedEdges[i].first.x);
			glVertex3fv(&fixedEdges[i].second.x);
		}
		
		glEnd();
		
		glBegin(GL_LINES);
		
		for (int i = 0; i < fixedEdges.size(); ++i) {
			//glColor3f(float(i & 0x3) / 3.0f, float(i >> 2 & 0x3) / 3.0f, float(i >> 4 & 0xC) / 3.0f);
			glVertex3fv(&fixedEdges[i].first.x);
			glVertex3fv(&fixedEdges[i].second.x);
		}
		
		
		
		glEnd();
		 glPopAttrib();
	 
	}
	
	// bugg i prob #1... om en edge helt är utanför skärmen försvinner den ju! inte bra ibland!
	
	for (int i = 0; i < fixedEdges.size(); ++i) {
		TGen::Vector4 projected[2];
		projected[0] = cameraProj * cameraTransform * TGen::Vector4(fixedEdges[i].first);
		projected[1] = cameraProj * cameraTransform * TGen::Vector4(fixedEdges[i].second);
		
		for (int a = 0; a < 2; ++a) {
			projected[a] /= projected[a].w;
			
			if (projected[a].x < -1.2f || projected[a].x > 1.2f || projected[a].y < -1.2f || projected[a].y > 1.2f || projected[a].z < -1.2f || projected[a].z > 1.2f)
				continue;
			
			if (firstMin) {
				min = projected[a];
				firstMin = false;
			}
			else {
				min = TGen::Vector3::Min(min, projected[a]);
			}
			
			if (firstMax) {
				max = projected[a];
				firstMax = false;
			}
			else {
				max = TGen::Vector3::Max(max, projected[a]);
			}
		}
	}
	
	
	min = min * 0.5 + 0.5;
	max = max * 0.5 + 0.5;
	
	
	TGen::Vector2 boxmin = TGen::Vector2(min) * TGen::Vector2(640.0f, 480.0f);
	TGen::Vector2 boxmax = TGen::Vector2(max) * TGen::Vector2(640.0f, 480.0f);
	
	outRectangle = TGen::Rectangle(boxmin, boxmax);
	
	if (boxmax.x == boxmin.x || boxmax.y == boxmin.y) {
		//std::cout << "no area " << std::string(boxmax) << " min " << std::string(boxmin) << std::endl;
		return false;
	}
	
	// TODO: gör en funktion som räknar ut det här, som går ur hela alltet för den lampan om den inte syns
	
	/*static int hey = 0;
	 
	 if (hey == 0) {
	 hey++;
	 }
	 else {
	 hey--;
	 return;
	 }*/
	
	
	
	
	return true;
}

TGen::Frustum TGen::Engine::ForwardRenderer::calculateFrustum(const TGen::Matrix4x4 & transform) {
	TGen::Matrix4x4 inverseTransform = transform.getInverse();
	
	TGen::Vector4 nearPlane[4];
	TGen::Vector4 farPlane[4];
	
	nearPlane[0] = inverseTransform * TGen::Vector4(-1.0f, 1.0f, -1.0f, 1.0f);
	nearPlane[1] = inverseTransform * TGen::Vector4( 1.0f, 1.0f, -1.0f, 1.0f);
	nearPlane[2] = inverseTransform * TGen::Vector4( 1.0f, -1.0f, -1.0f, 1.0f);
	nearPlane[3] = inverseTransform * TGen::Vector4(-1.0f, -1.0f, -1.0f, 1.0f);
	
	farPlane[0] = inverseTransform * TGen::Vector4(-1.0f, 1.0f, 1.0f, 1.0f);
	farPlane[1] = inverseTransform * TGen::Vector4( 1.0f, 1.0f, 1.0f, 1.0f);
	farPlane[2] = inverseTransform * TGen::Vector4( 1.0f, -1.0f, 1.0f, 1.0f);
	farPlane[3] = inverseTransform * TGen::Vector4(-1.0f, -1.0f, 1.0f, 1.0f);
	
	for (int i = 0; i < 4; ++i) {
		nearPlane[i] /= nearPlane[i].w;
		farPlane[i] /= farPlane[i].w;
	}
	
	TGen::Frustum frustum;
	frustum.nearPlane = TGen::Plane3(nearPlane[0], nearPlane[1], nearPlane[2]);
	frustum.farPlane = TGen::Plane3(farPlane[2], farPlane[1], farPlane[0]);
	frustum.leftPlane = TGen::Plane3(farPlane[3], farPlane[0], nearPlane[0]);
	frustum.rightPlane = TGen::Plane3(nearPlane[1], farPlane[1], farPlane[2]);
	frustum.bottomPlane = TGen::Plane3(farPlane[2], farPlane[3], nearPlane[2]);
	frustum.topPlane = TGen::Plane3(nearPlane[1], nearPlane[0], farPlane[1]);

	return frustum;
}

void TGen::Engine::ForwardRenderer::renderDepth(TGen::RenderList & renderList, TGen::Camera * camera) {
	currentPass = DepthPass;
	
	renderList.setMaterialOverride(this, 1);
	renderList.setMaterial(depthPassMaterial);
		
	renderList.render(renderer, camera->getTransform(), camera->getLod(), "default");	
}

void TGen::Engine::ForwardRenderer::renderShadowmap(TGen::RenderList & renderList, TGen::Engine::Light * light, const TGen::Matrix4x4 & transform) {
	TGen::Rectangle prevView = renderer.getViewport();
	
	currentPass = ShadowPass;
	
	renderList.setMaterialOverride(this, 1);
	renderList.setMaterial(depthPassMaterial);
	
	
	renderer.setRenderTarget(shadowMapTarget);
	renderer.setViewport(shadowMap->size);
	
	renderer.clearBuffers(TGen::DepthBuffer);
	


	// TODO: lightDirection, spotCutoff, shadow direction
	// TODO: skicka in en transform till funktionen som multiplicerar viewMat
	// TODO: kolla mot near clip plane i shadern. rätt clip plane distance!
	
	TGen::LodInfo lod;	
	renderList.render(renderer, transform, lod, "default");
	
	
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
		renderer.setDepthWrite(true);
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

