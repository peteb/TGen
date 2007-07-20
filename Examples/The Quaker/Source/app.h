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

void WindowResize(int width, int height);
void WindowRender();	
void WindowKeyDown(unsigned char key, int x, int y);

class Scene;
class Camera;

class App {
public:
	App();
	~App();	
	
	bool isRunning() const;
	void Update();
	void Resize(const TGen::Rectangle & size);
	void Render();
	void Quit();
	
private:
	TGen::Batch<TGen::Vertex3<float>, 2> * aabbBatch;
	
	SceneNode * cubeNode;
	TGen::Renderer * renderer;
	TGen::Rectangle windowSize;
	RenderList renderList;
	Camera * camera;
	Scene * scene;
	int window;
	bool run;
	scalar time;
};

#endif // !_THEQUAKER_APP_H
