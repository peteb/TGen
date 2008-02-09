/*
 *  md3struct.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/20/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_MD3STRUCT_H
#define _TGEN_RENDERER_MD3STRUCT_H

#include <tgen_core.h>
#include <vector>
#include <tgen_graphics.h>

#ifdef PACKED
#undef PACKED
#define PACKED
#endif

namespace TGen {
	namespace MD3 {
		
		// constants /////////////////////////////////////////////////////////////////////////////////////
		static const int MAX_QPATH = 64;
		static const int MAX_SHADERS = 256;
		static const int MAX_VERTS = 4096;
		static const int MAX_TRIANGLES = 8192;
		static const uint MAGIC = 0x33504449;
		
		// datatypes /////////////////////////////////////////////////////////////////////////////////////
		typedef unsigned char	U8;
		typedef signed short	S16;
		typedef signed int		S32;
		typedef float			F32;
		
		struct VEC3 {
			F32 x, y, z;
		} PACKED;
		
		
		// filestructure /////////////////////////////////////////////////////////////////////////////////
		struct Frame;
		struct Tag;
		struct Shader;
		struct Triangle;
		struct TexCoord;
		struct Vertex;
		struct Surface;
		
		struct Surface {
			S32 ident;
			U8 name[MAX_QPATH];
			S32 flags;
			S32 num_frames;
			S32 num_shaders;
			S32 num_verts;
			S32 num_triangles;
			Triangle * triangles;
			Shader * shaders;
			TexCoord * texCoords;
			Vertex * vertices;
			Surface * next;
		} PACKED;
		
		struct Header {
			S32 ident;
			S32 version;
			U8 name[MAX_QPATH];
			S32 flags;
			S32 num_frames;
			S32 num_tags;
			S32 num_surfaces;
			S32 num_skins;
			Frame * frames;		// S32 ofs_frames
			Tag * tags;
			Surface * first_surface;
			void * eof;
		} PACKED;
		
		struct Frame {
			VEC3 min_bounds;
			VEC3 max_bounds;
			VEC3 local_origin;
			F32 radius;
			U8 name[16];
		} PACKED;
		
		struct Tag {
			U8 name[MAX_QPATH];
			VEC3 origin;
			VEC3 axis[3];
		} PACKED;
		


		struct Shader {
			U8 name[MAX_QPATH];
			S32 shader_index;
		} PACKED;
		
		struct Triangle {
			S32 indices[3];
		} PACKED;
		
		struct TexCoord {
			F32 st[2];
		} PACKED;
		
		struct Vertex {
			S16 x, y, z, normal;
		} PACKED;	
		
		typedef std::vector<TGen::MD3::Surface *> SurfaceList;
		
		typedef TGen::TexCoord2<float, 0> TexCoordDecl;
		typedef TGen::Vertex3<float> VertexCoordDecl;
		typedef TGen::Normal3<float> NormalDecl;
		typedef TGen::VertexAttribute3<40, float> SecondVertexCoordDecl;
		typedef TGen::VertexAttribute3<41, float> SecondNormalDecl;
		
		typedef TGen::JoinVertexElements3<VertexCoordDecl, NormalDecl, TexCoordDecl>			VertexDecl;
		typedef TGen::JoinVertexElements2<VertexCoordDecl, NormalDecl >							FrameVertexDecl;	
		typedef TGen::JoinVertexElements5<TexCoordDecl, VertexCoordDecl, NormalDecl, SecondVertexCoordDecl, SecondNormalDecl> DoubleVertexDecl;
		typedef TGen::Index<unsigned int> IndexDecl;

	} // !MD3	
} // !TGen

#endif // !_TGEN_RENDERER_MD3STRUCT_H

