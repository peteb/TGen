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


App::App() : windowSize(800, 600), run(true), renderer(NULL), world(NULL), resources(NULL) {
	std::cout << "[app]: initializing..." << std::endl;
	
	glutInitWindowSize(windowSize.width, windowSize.height);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	window = glutCreateWindow("TGen ThePlace");
	glutDisplayFunc(WindowRender);		
	glutReshapeFunc(WindowReshape);
	glutIdleFunc(WindowRender);
	glutWMCloseFunc(WindowClose);
	glutKeyboardFunc(WindowKeyDown);
	
	renderer = new TGen::OpenGL::Renderer();
	resources = new ResourceManager(*renderer);
	
	resources->LoadMaterials("floor.material");
	
	world = new World;
}

App::~App() {
	std::cout << "[app]: shutting down..." << std::endl;
	
	delete world;
	delete resources;
	delete renderer;
	
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
	
	glutSwapBuffers();
}


