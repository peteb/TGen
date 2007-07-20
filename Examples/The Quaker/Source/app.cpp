/*
 *  app.cpp
 *  The Quaker
 *
 *  Created by Peter Backman on 7/18/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include <iostream>
#include <GLUT/GLUT.h>
#include <tgen_opengl.h>
#include "app.h"
#include "scene.h"
#include "camera.h"
#include "scenenoderenderer.h"
#include "resourcemanager.h"

App * gApp = NULL;

void WindowResize(int width, int height) {
	if (gApp)
		gApp->Resize(TGen::Rectangle(width, height));
}

void WindowRender() {
	if (gApp)
		gApp->Render();
}

void WindowKeyDown(unsigned char key, int x, int y) {
	if (key == 27 && gApp) {
		gApp->Quit();
	}
}

App::App() 
	: windowSize(800, 600) 
	, window(0)
	, run(true)
	, renderer(NULL)
	, scene(NULL)
	, aabbBatch(NULL)
	, time(0.0f)
	, cubeNode(NULL)
	, resources(NULL)
	, myCube(NULL)
{
	std::cout << "[app]: initializing..." << std::endl;
	
	glutInitWindowSize(windowSize.width, windowSize.height);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	window = glutCreateWindow("TGen \"The Quaker\"");
	glutDisplayFunc(WindowRender);		
	glutReshapeFunc(WindowResize);
	glutIdleFunc(WindowRender);
	
	glutKeyboardFunc(WindowKeyDown);
	
	renderer = new TGen::OpenGL::Renderer();
	std::cout << "[app]: created renderer '" << renderer->getCaps().driverName << "'" << std::endl;
	
	aabbBatch = new TGen::Batch<TGen::Vertex3<float>, 2>(renderer, 1000, TGen::PrimitiveLines, TGen::UsageStream);
	
	resources = new ResourceManager(*renderer);
	scene = new Scene();
	camera = new Camera("mycam");
	
	
	resources->LoadMaterials("test.shader");
	
	myCube = new Cube(*renderer);
	
	cubeNode = new SceneNode("cubenode");
	cubeNode->AddSurface(Surface(resources->getMaterial("textures/base_wall/metalfloor_wall_15ow"), myCube));
	
	scene->getSceneRoot()->AddChild(camera);
	scene->getSceneRoot()->AddChild(cubeNode);
	
	camera->setPosition(TGen::Vector3(1.0f, -0.4f, 19.0f));
	camera->setOrientation(TGen::Vector3(0.83f, 0.52f, 0.9f).getNormalized()); //camera->getPosition().getNormalized());

	scene->Update(0.0f);
}

App::~App() {
	std::cout << "[app]: shutting down..." << std::endl;
	
	delete myCube;
	delete aabbBatch;
	delete camera;
	delete scene;

	delete resources;
	delete renderer;
}

bool App::isRunning() const {
	return run;
}

void App::Quit() {
	run = false;
}

void App::Update() {
	if (scene)
		scene->Update(1.0);
}

void App::Resize(const TGen::Rectangle & size) {
	renderer->setViewport(size);
	windowSize = size;
	
	if (camera)
		camera->setAspectRatio(size.width / size.height);
}

void App::Render() {
	time += 0.02f;

	if (resources)
		resources->UpdateMaterials(time);
	
	cubeNode->setOrientation(TGen::Vector3(TGen::Cosine(TGen::Radian(time)) * 1.0f, 0.0f, TGen::Sine(TGen::Radian(time)) * 1.0f));
	//cubeNode->setOrientation(cubeNode->getPosition().getNormalized());
	
	// TODO: något är skumt med orientation och position......
	//camera->setPosition(TGen::Vector3(TGen::Cosine(TGen::Radian(time)) * 15.0f, 0.0f, TGen::Sine(TGen::Radian(time)) * 15.0f));
	//camera->setOrientation(-(camera->getPosition().getNormalized()));

	scene->Update(1.0f);
//	camera->Update(TGen::Matrix4x4::Identity, false);

	renderer->setClearColor(TGen::Color::Black);
	renderer->Clear(TGen::ColorBuffer | TGen::DepthBuffer);
	
	// render nodes ---------------------------------------
	SceneNodeRenderer nodeRenderer(renderList, *camera);
	
	if (scene) scene->getSceneRoot()->Accept(nodeRenderer);
	
	renderList.Render(*renderer, *camera);
	renderList.Clear();
	
	
	// render aabbs ---------------------------------------	
	AABBRenderer aabbRenderer(*aabbBatch, *camera);

	//std::cout << "start batch" << std::endl;
	aabbBatch->BeginBatch();	
	if (scene) scene->getSceneRoot()->Accept(aabbRenderer);
	aabbBatch->EndBatch();
	//std::cout << "end batch" << std::endl;
	
	renderer->setColor(TGen::Color(0.0f, 1.0f, 0.0f, 1.0f));
	renderer->setTransform(TGen::TransformProjection, camera->getProjection());
	renderer->setTransform(TGen::TransformWorldView, camera->getTransform());

	//glDepthFunc(GL_ALWAYS);
	glLineWidth(2.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	aabbBatch->Render(renderer);
	
	glutSwapBuffers();
}

