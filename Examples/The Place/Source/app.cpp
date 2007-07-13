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
#include "player.h"

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
	else if (app) {
		app->KeyDown(key);
	}
}

void WindowKeyUp(unsigned char key, int x, int y) {
	if (app) {
		app->KeyUp(key);
	}
}

void WindowSpecKeyDown(int key, int x, int y) {
	if (app)
		app->SpecKeyDown(key);	
}

void WindowSpecKeyUp(int key, int x, int y) {
	if (app)
		app->SpecKeyUp(key);
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
	glutKeyboardUpFunc(WindowKeyUp);
	glutSpecialFunc(WindowSpecKeyDown);
	glutSpecialUpFunc(WindowSpecKeyUp);
	
	dInitODE();

	renderer = new TGen::OpenGL::Renderer();
	resources = new ResourceManager(*renderer);
	
	resources->LoadMaterials("floor.material");
	resources->LoadMaps("meshes.map");
	
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

void App::KeyDown(unsigned char key) {
	switch (key) {
		case 'w':
			world->getPlayer()->moveForward = true;
			break;
		
	}
}

void App::KeyUp(unsigned char key) {
	switch (key) {
		case 'w':
			world->getPlayer()->moveForward = false;
			break;
	}
}

void App::SpecKeyDown(int key) {
	switch (key) {
		case GLUT_KEY_LEFT:
			world->getPlayer()->spinLeft = true;
			break;

		case GLUT_KEY_RIGHT:
			world->getPlayer()->spinRight = true;
			break;	
	}
	
	
}

void App::SpecKeyUp(int key) {
	switch (key) {
		case GLUT_KEY_LEFT:
			world->getPlayer()->spinLeft = false;
			break;
			
		case GLUT_KEY_RIGHT:
			world->getPlayer()->spinRight = false;
			break;	
	}
}


void App::Reshape(const TGen::Rectangle & size) {
	renderer->setViewport(size);
	windowSize = size;
}

void App::Render() {
	renderer->setClearColor(TGen::Color::Black);
	renderer->Clear(TGen::ColorBuffer | TGen::DepthBuffer);

	renderer->setTransform(TGen::TransformProjection, TGen::Matrix4x4::PerspectiveProjection(60.0f, windowSize.width / windowSize.height, 0.5f, 100.0f));
	//renderer->setTransform(TGen::TransformWorldView, TGen::Matrix4x4::Translation(TGen::Vector3(0.0f, 2.0f, -6.0f)));
	
	
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

