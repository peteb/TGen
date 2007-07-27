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
#include <fstream>
#include <tgen_opengl.h>
#include "app.h"
#include "scene.h"
#include "camera.h"
#include "scenenoderenderer.h"
#include "resourcemanager.h"
#include "bsploader.h"

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
	, level(NULL)
	, lastUpdate(0.0)
	, frames(0)
	, accumFps(0.0)
{
	std::cout << "[app]: initializing..." << std::endl;
	
	glutInitWindowSize(windowSize.width, windowSize.height);
	glutInitDisplayMode(GLUT_RGB |  GLUT_DEPTH);
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
	
	std::cout << "[app]: loading bsp..." << std::endl;
	BSPLoader bspLoader;
	
	std::ifstream file;
	file.open("q3dm17.bsp", std::ios::in | std::ios::binary);
	bspLoader.Parse(file);
	file.close();

	resources->LoadMaterials("test.shader");
	// FÖRST: fixa kurvan. sen fixa en index buffer bara
	
	level = bspLoader.CreateTree(*renderer, *resources);
	
	if (!level)
		throw TGen::RuntimeException("App::App", "failed to load bsp!");
	
	scene->getSceneRoot()->AddChild(level);	
	
	myCube = new Cube(*renderer);
	
	cubeNode = new SceneNode("cubenode");
	cubeNode->AddSurface(Surface(resources->getMaterial("textures/base_wall/metalfloor_wall_15ow"), myCube));
	
	scene->getSceneRoot()->AddChild(camera);
	scene->getSceneRoot()->AddChild(cubeNode);
	
	TGen::Vector3 base(-100.0f, 0.0f, 100.0f); //(100.0f, 50.0f, 470.0f);
	TGen::Vector3 lookAt(-100.0f, 0.0f, 0.0f);
	TGen::Vector3 position(0.0f, 130.0f, -250.0f);	// (0.0f, 160.0f, 0.0f);
	
	lookAt += base;
	position += base;
	
	camera->setPosition(position);
	//camera->setOrientation(TGen::Vector3(0.0f, 0.0f, 1.0f));
	camera->setOrientation((lookAt - position).getNormalized());
	//camera->setOrientation(TGen::Vector3(0.83f, 0.52f, 0.9f).getNormalized()); //camera->getPosition().getNormalized());
	camera->setUp(TGen::Vector3(0.0f, 1.0f, 0.0f));
	
	camera->Update(TGen::Matrix4x4::Identity, false);
	scene->Update(0.0f);
	
	glEnable(GL_COLOR_MATERIAL);
	
	GLfloat amb[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat pos[] = {0.0f, 200.0f, 0.0f};
	GLfloat dif[] = {0.0f, 0.0f, 0.0f, 1.0f};
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, amb);
	glLightfv(GL_LIGHT0, GL_SPECULAR, amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, amb);
	
	/*//glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_POSITION, pos);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, dif);
	//glLightfv(GL_LIGHT1, GL_SPECULAR, dif);*/
	glPointSize(90.0f);
	
	
	renderer->Clear(TGen::ColorBuffer);

	lastFrame = TGen::Time::Now();






}

App::~App() {
	std::cout << "[app]: shutting down..." << std::endl;
	
	for (int i = 0; i < 4; ++i)
		std::cout << "[app]: stat " << i << ": " << std::fixed << stats[i] << std::endl;
	
	delete level;
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
	TGen::Time thisFrame = TGen::Time::Now();
	
	double dt = double(thisFrame) - double(lastFrame);
	lastFrame = thisFrame;
	
	time += dt;
	fps = 1 / dt;
	
	accumFps += fps;
	
	if (thisFrame - lastUpdate >= 1.0) {
		std::cout << "accum fps avg: " << accumFps / double(frames) << " frames: " << frames << " last: " << TGen::lexical_cast<std::string>(fps).c_str() << std::endl;
		accumFps = 0.0;
		frames = 0;
		lastUpdate = thisFrame;
	}
	
	if (resources)
		resources->UpdateMaterials(time);
	
	//cubeNode->setOrientation(TGen::Vector3(TGen::Cosine(TGen::Radian(time)) * 1.0f, 0.0f, TGen::Sine(TGen::Radian(time)) * 1.0f));
	//cubeNode->setPosition(cubeNode->getOrientation().getNormalized() * 1.0f + TGen::Vector3(-2.0f, 0.0f, 0.0f));
	
	// TODO: något är skumt med orientation......
	//camera->setOrientation(TGen::Vector3(TGen::Cosine(TGen::Radian(time)), 0.0f, TGen::Sine(TGen::Radian(time))));
	//camera->setOrientation(-(camera->getPosition().getNormalized()));

	TGen::Time start = TGen::Time::Now();
	scene->Update(1.0f);
	stats[0] = TGen::Time::Now() - start;
	
	//renderer->setClearColor(TGen::Color::Black);
	renderer->Clear(TGen::DepthBuffer);
	


	// render nodes ---------------------------------------
	SceneNodeRenderer nodeRenderer(renderList, *camera); //(time > 5.0f ? (time - 5.0f) * 100.0f : 0));
	
	if (scene) scene->getSceneRoot()->Accept(nodeRenderer);
	
	renderList.Sort();
	renderList.Render(*renderer, *camera);
	renderList.Clear();
	stats[2] = TGen::Time::Now() - start;
	
	
	
	
	
	
	
	start = TGen::Time::Now();
	// render aabbs ---------------------------------------	
	/*AABBRenderer aabbRenderer(*aabbBatch, *camera);

	aabbBatch->BeginBatch();	
	if (scene) scene->getSceneRoot()->Accept(aabbRenderer);
	aabbBatch->EndBatch();
	
	renderer->setColor(TGen::Color(0.0f, 1.0f, 0.0f, 1.0f));
	renderer->setTransform(TGen::TransformProjection, camera->getProjection());
	renderer->setTransform(TGen::TransformWorldView, camera->getTransform());

	//glDepthFunc(GL_ALWAYS);
	glLineWidth(2.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	aabbBatch->Render(renderer);
	*/
	
	stats[3] = TGen::Time::Now() - start;
	
	glutSwapBuffers();
	frames++;
}

