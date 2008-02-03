/*
 *  tgen_renderer.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/10/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_RENDERER_H
#define _TGEN_RENDERER_RENDERER_H

#include "geometry.h"
#include "lodgeometry.h"
#include "staticgeometry.h"
#include "meshgeometry.h"
#include "mesh.h"
#include "animatingmeshgeometry.h"
#include "camera.h"
#include "fpscamera.h"
#include "scenenode.h"
#include "face.h"
#include "materialsource.h"
#include "meshsource.h"
#include "printscene.h"
#include "facelinker.h"
#include "renderlist.h"
#include "renderfill.h"
#include "basicrenderlist.h"
#include "meshgeomlinklist.h"

#include "md3struct.h"
#include "md3parser.h"
#include "md3model.h"

#include "md5parser.h"
#include "md5mesh.h"
#include "md5file.h"
#include "md5tokenizer.h"
#include "md5submesh.h"

#include "model_new.h"
#include "modelinstance_new.h"
#include "mesh_new.h"
#include "meshinstance_new.h"
#include "face_new.h"
#include "modelinstanceproxy.h"
#include "modelinstantiator.h"
#include "modelinstantiatepool.h"

namespace TGen {
	bool isRendererDebug();
}

#endif // !_TGEN_RENDERER_RENDERER_H
