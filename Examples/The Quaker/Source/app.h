/*
 *  app.h
 *  The Quaker
 *
 *  Created by Peter Backman on 7/18/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _THEQUAKER_APP_H
#define _THEQUAKER_APP_H

#include <tgen_graphics.h>
#include "renderlist.h"
#include "cube.h"
#include "bsp.h"

void WindowResize(int width, int height);
void WindowRender();	
void WindowKeyDown(unsigned char key, int x, int y);

class Scene;
class Camera;
class ResourceManager;

class App {
public:
	App();
	~App();	
	
	bool isRunning() const;
	void Update();
	void Resize(const TGen::Rectangle & size);
	void Render();
	void Quit();
	TGen::Time stats[10];
	
private:
	TGen::Batch<TGen::Vertex3<float>, 2> * aabbBatch;

	ResourceManager * resources;
	SceneNode * cubeNode;
	BSPTree * level;
	TGen::Renderer * renderer;
	TGen::Rectangle windowSize;
	RenderList renderList;
	Camera * camera;
	Scene * scene;
	int window, frames;
	bool run;
	TGen::Time lastFrame, lastUpdate;
	double fps, accumFps;
	scalar time;
	

	Cube * myCube;
};

#endif // !_THEQUAKER_APP_H
