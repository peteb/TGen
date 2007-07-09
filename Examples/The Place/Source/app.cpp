/*
 *  app.cpp
 *  The Place
 *
 *  Created by Peter Backman on 7/9/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "app.h"
#include <GLUT/GLUT.h>
#include <iostream>
#include <tgen_opengl.h>
#include "resourcemanager.h"
#include "world.h"

App * app = NULL;

void WindowReshape(int width, int height) {
	if (app)
		app->Reshape(TGen::Rectangle(width, height));
}

void WindowRender() {
	if (app)
		app->Render();
}

void WindowClose() {
	if (app)
		app->Quit();
}

void WindowKeyDown(unsigned char key, int x, int y) {
	if (key == 27 && app) {
		app->Quit();
	}
}


App::App() : windowSize(800, 600), run(true), renderer(NULL), world(NULL), resources(NULL), lastUpdate(0.0) {
	std::cout << "[app]: initializing..." << std::endl;
	
	glutInitWindowSize(windowSize.width, windowSize.height);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	window = glutCreateWindow("TGen \"The Place\"");
	glutDisplayFunc(WindowRender);		
	glutReshapeFunc(WindowReshape);
	glutIdleFunc(WindowRender);
	glutWMCloseFunc(WindowClose);
	glutKeyboardFunc(WindowKeyDown);
	
	dInitODE();

	renderer = new TGen::OpenGL::Renderer();
	resources = new ResourceManager(*renderer);
	
	resources->LoadMaterials("floor.material");
	
	world = new World;
	world->LoadData(*renderer, *resources);
}

App::~App() {
	std::cout << "[app]: shutting down..." << std::endl;
	
	delete world;
	delete resources;
	delete renderer;
	
	dCloseODE();
	glutDestroyWindow(window);
}

bool App::isRunning() const {
	return run;
}

void App::Quit() {
	run = false;
}

void App::Reshape(const TGen::Rectangle & size) {
	renderer->setViewport(size);
	windowSize = size;
}

void App::Render() {
	renderer->setClearColor(TGen::Color::White);
	renderer->Clear(TGen::ColorBuffer | TGen::DepthBuffer);

	renderer->setTransform(TGen::TransformProjection, TGen::Matrix4x4::PerspectiveProjection(60.0f, windowSize.width / windowSize.height, 0.1f, 100.0f));
	//renderer->setTransform(TGen::TransformWorldView, TGen::Matrix4x4::Translation(TGen::Vector3(0.0f, 0.0f, -6.0f)) * TGen::Matrix4x4::Rotation(TGen::Vector3(0.0f, 1.0f, 0.0f), TGen::Radian(0.0f)));
	
	
	if (world)
		world->Render(*renderer);
	
	glutSwapBuffers();
}

void App::Update() {
	scalar timeNow = scalar(glutGet(GLUT_ELAPSED_TIME)) / 1000.0;
	scalar updateDelta = timeNow - lastUpdate;
	
	if (resources)
		resources->UpdateMaterials(timeNow);
	
	if (world)
		world->Update(updateDelta);

	lastUpdate = timeNow;
}

