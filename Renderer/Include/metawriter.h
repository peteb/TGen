/*
 *  metawriter.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 2/16/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_METAWRITER_H
#define _TGEN_RENDERER_METAWRITER_H

namespace TGen {
	class VertexStream;
	
	enum MetaType {
		MetaNormals = 1,
		MetaTBN = 2,
		MetaJoints = 3,
		MetaPortals = 4,
	};
	
	// writes vertices to a vertex stream, used for rendering metadata like joints, aabbs, portal visibility, normals, etc
	class MetaWriter {
	public:
		virtual ~MetaWriter() {}
		
		virtual void writeMeta(uint metaType, const TGen::Matrix4x4 & transform, TGen::VertexStream & stream) {}
		
	};
	
} // !TGen

#endif // !_TGEN_RENDERER_METAWRITER_H
