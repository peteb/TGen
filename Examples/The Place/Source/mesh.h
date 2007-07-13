/*
 *  mesh.h
 *  The Place
 *
 *  Created by Peter Backman on 7/13/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_THEPLACE_MESH_H
#define _TGEN_THEPLACE_MESH_H

#include <string>

class Mesh {
public:
	Mesh(const std::string & name);
	
private:
	std::string name;
};

#endif // !_TGEN_THEPLACE_MESH_H
