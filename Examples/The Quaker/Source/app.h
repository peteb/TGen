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
#include <SDL/SDL.h>
#include "renderlist.h"
#include "cube.h"
#include "bsp.h"

void WindowResize(int width, int height);
void WindowRender();	
void WindowKeyDown(unsigned char key, int x, int y);
void WindowMouseMove(int x, int y);
void WindowKeyUp(unsigned char key, int x, int y);

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
	void MouseMove(const SDL_MouseMotionEvent & motion);
	void Render();
	void Quit();
	void HandleMouseMove(const TGen::Vector2 & delta);
	void KeyDown(const SDL_keysym & keysym);
	void KeyUp(const SDL_keysym & keysym);
	
	TGen::Time stats[10];
	
private:
	TGen::Batch<TGen::Vertex3<float>, 2> * aabbBatch;

	ResourceManager * resources;
	SceneNode * cubeNode;
	BSPTree * level;
	TGen::Vector2 lastMousePos;
	TGen::Renderer * renderer;
	TGen::Rectangle windowSize;
	RenderList renderList;
	Camera * camera;
	Scene * scene;
	int window, frames;
	bool run;
	TGen::Time lastFrame, lastUpdate, lastStateUpdate;
	double fps, accumFps;
	scalar time;
	
	bool moveForward, moveBack, moveLeft, moveRight;
	
	Cube * myCube;
};

#endif // !_THEQUAKER_APP_H
