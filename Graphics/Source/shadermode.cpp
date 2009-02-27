/*
 *  shadermode.cpp
 *  TGen Graphics
 *
 *  Created by Peter Backman on 1/7/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#include "shadermode.h"
#include "material.h"
#include "shadervarupdater.h"
#include "materiallinkcallback.h"

TGen::ShaderMode::ShaderMode(const std::string & name)
	: name(name)
	, shader(NULL)
{
}

void TGen::ShaderMode::link(TGen::MaterialLinkCallback & callback) {
	if (name == "fixed")
		shader = NULL;
	else
		shader = callback.getShaderProgram(name);
	
	for (ShaderUpdaterList::iterator iter = shaderUpdaters.begin(); iter != shaderUpdaters.end(); ) {
		try {
			(*iter)->link(shader);
			++iter;
		}
		catch (const TGen::RuntimeException & error) {
			std::cout << "Error while linking shader updaters: \"" + std::string(error.what()) + "\", removed variable" << std::endl;
			
			delete *iter;
			iter = shaderUpdaters.erase(iter);
		}
	}
}

void TGen::ShaderMode::update() {			// TODO: only update current shader.    AND save shader mode names that are being used
	//for (ShaderUpdaterList::iterator iter = shaderUpdaters.begin(); iter != shaderUpdaters.end(); ++iter)
		//(*iter)->update();	
}

void TGen::ShaderMode::addShaderUpdater(TGen::ShaderUpdater * updater) {
	shaderUpdaters.push_back(updater);
}

TGen::ShaderProgram * TGen::ShaderMode::getShader() const {
	return shader;
}

const std::string & TGen::ShaderMode::getName() const {
	return name;
}
