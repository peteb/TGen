/*
 *  app.h
 *  The Place
 *
 *  Created by Peter Backman on 7/9/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_THEPLACE_APP_H
#define _TGEN_THEPLACE_APP_H

#include "tgen_graphics.h"

void WindowReshape(int width, int height);
void WindowRender();
void WindowClose();
void WindowKeyDown(unsigned char key, int x, int y);

class World;
class ResourceManager;

class App {
public:
	App();
	~App();
	
	void Reshape(const TGen::Rectangle & size);	
	void Render();
	void Update();
	bool isRunning() const;
	void Quit();
	
private:
	TGen::Renderer * renderer;
	TGen::Rectangle windowSize;

	ResourceManager * resources;
	World * world;
	
	int window;
	bool run;
	scalar lastUpdate;

};

#endif // !_TGEN_THEPLACE_APP_H
