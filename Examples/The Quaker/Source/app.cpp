/*
 *  app.cpp
 *  The Quaker
 *
 *  Created by Peter Backman on 7/18/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include <iostream>
//#include <GLUT/GLUT.h>
#include <fstream>
#include <tgen_opengl.h>
#include "app.h"
#include "scene.h"
#include "camera.h"
#include "scenenoderenderer.h"
#include "resourcemanager.h"
#include "bsploader.h"

App * gApp = NULL;

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
	, moveForward(false)
	, moveBack(false)
	, moveLeft(false)
	, moveRight(false)
{
	std::cout << "[app]: initializing..." << std::endl;
	
	/*glutInitWindowSize(windowSize.width, windowSize.height);
	glutInitDisplayMode(GLUT_RGB |  GLUT_DEPTH);
	window = glutCreateWindow("TGen \"The Quaker\"");
	glutDisplayFunc(WindowRender);		
	glutReshapeFunc(WindowResize);
	glutIdleFunc(WindowRender);
	glutPassiveMotionFunc(WindowMouseMove);
	glutMotionFunc(WindowMouseMove);
	
	glutKeyboardFunc(WindowKeyDown);
	glutKeyboardUpFunc(WindowKeyUp);
	*/
		
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
	
	level = bspLoader.CreateTree(*renderer, *resources);
	
	//if (!level)
	//	throw TGen::RuntimeException("App::App", "failed to load bsp!");
	
	scene->getSceneRoot()->AddChild(level);	
	
	myCube = new Cube(*renderer);
	
	cubeNode = new SceneNode("cubenode");
	cubeNode->AddSurface(Surface(resources->getMaterial("textures/base_wall/metalfloor_wall_15ow"), myCube));
	
	scene->getSceneRoot()->AddChild(camera);
	scene->getSceneRoot()->AddChild(cubeNode);
	
	TGen::Vector3 base(-100.0f, 0.0f, 100.0f); //(100.0f, 50.0f, 470.0f);
	TGen::Vector3 lookAt(-100.0f, 0.0f, 0.0f);
	TGen::Vector3 position(0.0f, 130.0f, -250.0f);	// (0.0f, 160.0f, 0.0f);
	
	//lookAt += base;
	//position += base;
	
	/*camera->setPosition(position);
	//camera->setOrientation(TGen::Vector3(0.0f, 0.0f, 1.0f));
	camera->setOrientation((lookAt - position).getNormalized());
	//camera->setOrientation(TGen::Vector3(0.83f, 0.52f, 0.9f).getNormalized()); //camera->getPosition().getNormalized());
	camera->setUp(TGen::Vector3(0.0f, 1.0f, 0.0f));
	*/
	camera->setPosition(TGen::Vector3(0.0f, 2.0f, 3.0f));
	camera->setOrientation(TGen::Quaternion4(0.0f, 0.0f, -1.0f));
	camera->setUp(TGen::Vector3(0.0f, 1.0f, 0.0f));
	camera->setNear(0.1);
	camera->setFar(100.0);
	
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
	//if (lastMousePos.getMagnitude() > 0.1)
	//	glutWarpPointer(windowSize.width / 2.0, windowSize.height / 2.0);
	
	if (scene)
		scene->Update(1.0);
	
	
	if (camera) {
				
		TGen::Quaternion4 forward = camera->forward; //getOrientation().getNormalized();
		forward.y = -forward.y;
		

		TGen::Vector3 perpend = -TGen::Vector3::CrossProduct(forward, TGen::Vector3(0.0f, 1.0f, 0.0f)).Normalize();

		//TGen::Quaternion4 perpend = TGen::Quaternion4::Rotation(, TGen::Degree(90)) * forward;
		TGen::Vector3 position = camera->getPosition();
		perpend.Normalize();
		
		scalar speed = 10.0;
		
		if (moveForward)
			position += TGen::Vector3(forward) / speed;
		if (moveBack)
			position -= TGen::Vector3(forward) / speed;
		if (moveRight)
			position += TGen::Vector3(perpend) / speed;
		if (moveLeft)
			position -= TGen::Vector3(perpend) / speed;
		
		
		/*if (moveForward)
			camera->orientationY -= 0.1f;
		if (moveBack)
			camera->orientationY += 0.1f;
		if (moveRight)
			camera->orientationX += 0.1f;
		if (moveLeft)
			camera->orientationX -= 0.1f;
		*/
		
		camera->setPosition(position);
	}
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
	
	//camera->setOrientation(TGen::Vector3(TGen::Cosine(TGen::Radian(time)), 0.0f, TGen::Sine(TGen::Radian(time))));
	//camera->setOrientation(-(camera->getPosition().getNormalized()));

	static scalar rot = 0.0f;
	rot += dt;
	
	TGen::Quaternion4 newOrientation = cubeNode->getOrientation();
	newOrientation = TGen::Quaternion4(0.0f, 0.0f, 1.0f);
	newOrientation *= TGen::Quaternion4::Rotation(TGen::Vector3(0.0f, 1.0f, 0.0f), TGen::Radian(rot));

	newOrientation *= TGen::Quaternion4::Rotation(TGen::Vector3(1.0f, 0.0f, 0.0f), TGen::Radian(rot / 10.0));
	
	//newOrientation.Normalize();
	//newOrientation *= TGen::Quaternion4::Rotation(TGen::Vector3(0.0f, 1.0f, 0.0f), TGen::Degree(90 * dt));

	cubeNode->setOrientation(newOrientation);
	
	
	/*newOrientation = cubeNode->getOrientation();
	newOrientation *= TGen::Quaternion4::Rotation(TGen::Vector3(0.0f, 1.0f, 0.0f), TGen::Degree(90 * dt));
	cubeNode->setOrientation(newOrientation);
	*/
	
	//scene->Update(1.0f);
	
	//renderer->setClearColor(TGen::Color::Black);
	renderer->Clear(TGen::DepthBuffer | TGen::ColorBuffer);
	


	// render nodes ---------------------------------------
	SceneNodeRenderer nodeRenderer(renderList, *camera); //(time > 5.0f ? (time - 5.0f) * 100.0f : 0));
	
	if (scene) scene->getSceneRoot()->Accept(nodeRenderer);
	
	renderList.Sort();
	renderList.Render(*renderer, *camera);
	renderList.Clear();
	
	
	
	
	
	
	// render aabbs ---------------------------------------	
	AABBRenderer aabbRenderer(*aabbBatch, *camera);

	aabbBatch->BeginBatch();	
	if (scene) scene->getSceneRoot()->Accept(aabbRenderer);
	aabbBatch->EndBatch();
	
	renderer->setColor(TGen::Color(0.0f, 1.0f, 0.0f, 1.0f));
	renderer->setTransform(TGen::TransformProjection, camera->getProjection());
	renderer->setTransform(TGen::TransformWorldView, camera->getTransform());

	//glDepthFunc(GL_ALWAYS);
	glLineWidth(5.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	aabbBatch->Render(renderer);

	
	
	SDL_GL_SwapBuffers();
	//glutSwapBuffers();
	frames++;
}

void App::MouseMove(const SDL_MouseMotionEvent & motion) {
	TGen::Vector2 delta(motion.xrel, motion.yrel);
	
	TGen::Vector2 center(windowSize.width / 2.0, windowSize.height / 2.0);
	/*int x = 0, y = 0;
	SDL_GetRelativeMouseState(&x, &y);
	TGen::Vector2 pos(x, y);*/
	HandleMouseMove(delta/* - lastMousePos*/);
	//lastMousePos = pos;
	
	//SDL_WarpMouse(center.x, center.y);
}

void App::HandleMouseMove(const TGen::Vector2 & delta) {
	//const TGen::Vector2 sensibility(0.01, 0.01);
	// TODO: operator * Vector2
	TGen::Vector2 fixed = delta * 0.005;
	
	if (camera) {
		//TGen::Quaternion4 rotation = camera->getOrientation();
		//camera->orientationY *= TGen::Quaternion4::Rotation(TGen::Vector3(1.0f, 0.0f, 0.0f), TGen::Radian(fixed.y));
		//camera->orientationX *= TGen::Quaternion4::Rotation(TGen::Vector3(0.0f, 1.0f, 0.0f), TGen::Radian(-fixed.x));
		camera->orientationX -= fixed.x;
		camera->orientationY += fixed.y;
		//camera->setOrientation(rotation.getNormalized());
	}
}

void App::KeyDown(const SDL_keysym & keysym) {	
	switch (keysym.sym) {
		case SDLK_ESCAPE:
			Quit();
			break;
		
		case SDLK_w:
			moveForward = true;
			break;
			
		case SDLK_s:
			moveBack = true;
			break;
			
		case SDLK_a:
			moveLeft = true;
			break;
			
		case SDLK_d:
			moveRight = true;
			break;
	}
}

void App::KeyUp(const SDL_keysym & keysym) {
	switch (keysym.sym) {
		case SDLK_w:
			moveForward = false;
			break;
			
		case SDLK_s:
			moveBack = false;
			break;		

		case SDLK_a:
			moveLeft = false;
			break;
			
		case SDLK_d:
			moveRight = false;
			break;
	}
}

