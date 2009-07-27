/*
 *  q3maploader.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/27/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#include "q3bsp/q3maploader.h"
#include "q3bsp/q3map.h"
#include "filesystem.h"
#include "file.h"
#include "log.h"

#include <tgen_core.h>

TGen::Engine::Q3MapLoader::Q3MapLoader(TGen::Engine::StandardLogs & logs, TGen::Engine::Filesystem & filesystem)
	: logs(logs)
	, filesystem(filesystem)
{

}


TGen::Engine::Q3Map * TGen::Engine::Q3MapLoader::createMap(const std::string & name, const std::string & filename, const TGen::VertexTransformer & transformer) {
	TGen::auto_ptr<TGen::Engine::Q3Map> map = new TGen::Engine::Q3Map(name);
	TGen::auto_ptr<TGen::Engine::File> file = filesystem.openRead(filename);
	
	logs.info["bsp3"] << "loading map from '" << filename << "'..." << TGen::endl;
	
	
	exit(33);
	
	return map.release();
}
