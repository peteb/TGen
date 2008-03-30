/*
 *  node.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/5/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "scene/node.h"

TGen::Engine::Scene::Node::Node(const std::string & name, TGen::SceneNode * sceneNode) 
	: TGen::Engine::Component(name)
	, sceneNode(sceneNode)
{
	
}

TGen::Engine::Scene::Node::~Node() {
	
}

TGen::SceneNode * TGen::Engine::Scene::Node::getSceneNode() {
	return sceneNode;
}

/*
Fysiksystemet steppas, alla objekt som flyttats (dvs inte har kommit i vila) triggrar sina update-events vilket
 SceneNode-komponenten har reggat och då uppdateras det i det systemet, utan att triggra en update åt andra hållet,
 SceneNode uppdateras manuellt -> update till physcomponent, updatering i physsystemet. physsystemet har sista talan
 (scene -> phys *step* phys -> scene) -> sound & logic    man måste separera, går inte att ha direkt-events... flaggor!
 scenesystem.update();
 physsystem.updateFromScene();
 physsystem.update();
 scenesystem.updateFromPhys(); // nodes är fortfarande markerade updated
 soundsystem.update();
 logicsystem.update();
 scenesystem.resetFlags();
 Det här kommer inte lösa problemet med att scenenoden puttar ett objekt från A till B och phys puttar tillbaka till A, och scenenoden markeras fortfarande som updated
 SceneNodeComponent reggar listener på scenenoden om när den blir uppdaterad
 */