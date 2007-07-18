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
	
	scene = new Scene();

	camera = new Camera("mycam");
	scene->getSceneRoot()->AddChild(camera);

	camera->setPosition(TGen::Vector3(0.0f, 0.0f, -5.0f));
	camera->setOrientation(TGen::Vector3(0.0f, 0.0f, 1.0f));

	scene->Update(0.0f);
}

App::~App() {
	std::cout << "[app]: shutting down..." << std::endl;
	
	delete camera;
	delete scene;
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
}

void App::Render() {
	renderer->setClearColor(TGen::Color::Black);
	renderer->Clear(TGen::ColorBuffer | TGen::DepthBuffer);
	
	
	if (scene)
		scene->AddSurfaces(renderList, *camera);
	
	renderList.Render(*renderer, *camera);
	renderList.Clear();
	
	glutSwapBuffers();
}

