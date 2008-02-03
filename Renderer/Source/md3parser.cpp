/*
 *  md3parser.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/20/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "md3parser.h"
#include "md3struct.h"
#include "md3model.h"
#include "md3mesh.h"
#include "mesh.h"

TGen::MD3::Parser::Parser() {}
TGen::MD3::Parser::~Parser() {}

TGen::MD3::File::File(TGen::MD3::Header * header, const TGen::MD3::SurfaceList & surfaces)
	: header(header)
	, surfaces(surfaces)
{
}

TGen::MD3::File::~File() {
	if (header)
		free(header);
}


TGen::MD3::File * TGen::MD3::Parser::parse(TGen::InputStream & source) {
	uint size = source.getSize();

	if (size < sizeof(TGen::MD3::Header))
		throw TGen::RuntimeException("MD3::Parser::parse", "file can't possibly be valid, not enough room for header! Size: ") << size;
	
	DEBUG_PRINT("[md3]: size: " << size);
		
	TGen::MD3::Header * header = reinterpret_cast<TGen::MD3::Header *>(malloc(size));
	source.read(reinterpret_cast<char *>(header), size);
		
	SwapLocalLe32(header->ident);
	SwapLocalLe32(header->version);
	SwapLocalLe32(header->flags);
	SwapLocalLe32(header->num_frames);
	SwapLocalLe32(header->num_tags);
	SwapLocalLe32(header->num_surfaces);
	SwapLocalLe32(header->num_skins);
	SwapLocalLe32(header->frames);
	SwapLocalLe32(header->tags);
	SwapLocalLe32(header->surfaces);
	SwapLocalLe32(header->eof);

	if (header->ident != TGen::MD3::MAGIC)
		throw TGen::RuntimeException("MD3::Parser", "header invalid!");
	
	PatchPointer(header->frames, header);
	PatchPointer(header->tags, header);
	PatchPointer(header->first_surface, header);
	PatchPointer(header->eof, header);
	
	TGen::MD3::SurfaceList surfaces;
	TGen::MD3::Surface * surface = header->first_surface;

	for (int i = 0; i < header->num_surfaces; ++i) {
		PatchPointer(surface->triangles, surface);
		PatchPointer(surface->shaders, surface);
		PatchPointer(surface->texCoords, surface);
		PatchPointer(surface->vertices, surface);
		PatchPointer(surface->next, surface);
		
		surfaces.push_back(surface);
		surface = surface->next;
	}
	
	return new TGen::MD3::File(header, surfaces);
}

void TGen::MD3::File::printInfo(std::ostream & stream) const {
	stream << "version: " << header->version << "\n";
	stream << "name: " << header->name << "\n";
	stream << "frames: " << header->num_frames << "\n";
	
	for (int a = 0; a < header->num_frames; ++a) {
		TGen::MD3::Frame * frame = &header->frames[a];
					
		stream << " * name: " << frame->name << "\n";
		stream << "\n";
	}
				
	
	stream << "tags: " << header->num_tags << "\n";
	
	for (int i = 0; i < header->num_tags; ++i) {
		TGen::MD3::Tag * tag = &header->tags[i];
		
		stream << " * name: " << tag->name << "\n";
		stream << "\n";
	}
	
	stream << "surfaces: " << header->num_surfaces << "\n";
	
	for (int i = 0; i < surfaces.size(); ++i) {
		TGen::MD3::Surface * surface = surfaces[i];
		
		if (surface->ident == TGen::MD3::MAGIC) {
			stream << " * ident: " << surface->ident << "\n";
			stream << "   name: " << surface->name << "\n";
			stream << "   flags: " << surface->flags << "\n";
			stream << "   frames: " << surface->num_frames << "\n";
						
			stream << "   shaders: " << surface->num_shaders << "\n";
		
			for (int a = 0; a < surface->num_shaders; ++a) {
				stream << "    * name: " << surface->shaders[a].name << "\n";
				stream << "      index: " << surface->shaders[a].shader_index << "\n";
			}
			
			stream << "\n";
		}
		else {
			stream << " * invalid ident\n\n";
		}
	}
	
	stream << "num_skins: " << header->num_skins << "\n";
}


TGen::MD3::Model * TGen::MD3::File::createModel(TGen::VertexDataSource & dataSource, scalar scale) const {
	if (header->num_frames > 1)
		return createAnimatingModel(dataSource, scale);

	return createStaticModel(dataSource, scale);
}

TGen::MD3::Model * TGen::MD3::File::createStaticModel(TGen::VertexDataSource & dataSource, scalar scale) const {
	TGen::MD3::Model * newModel = new TGen::MD3::Model(reinterpret_cast<const char *>(header->name));
	
	DEBUG_PRINT("[md3]: creating surfaces...");
	
	// TODO: portalbanor skickar egen user-grej... ClippedRoom (entities to draw, room vertices)
	//       eller sÂ fixas ClippedFace ist‰llet, kanske! dÂ kan man sortera baserat pÂ clipping 
	
	for (int i = 0; i < surfaces.size(); ++i) {
		TGen::MD3::Surface * surface = surfaces[i];
		
		if (surface->ident == TGen::MD3::MAGIC) {
			TGen::MD3::Mesh * mesh = new TGen::MD3::Mesh(reinterpret_cast<const char*>(surface->shaders[0].name));
			
			int numIndices = surface->num_triangles * 3;
			int numVertices = surface->num_verts;
			
			mesh->vb = dataSource.createVertexData(TGen::MD3::VertexDecl(), sizeof(TGen::MD3::VertexDecl::Type) * numVertices, TGen::UsageStatic);
			mesh->ib = dataSource.createVertexData(TGen::MD3::IndexDecl(), sizeof(TGen::MD3::IndexDecl::Type) * numIndices, TGen::UsageStatic);
			mesh->indexCount = numIndices;
			
			if (!mesh->vb || !mesh->ib)
				throw TGen::RuntimeException("MD3::File::createMesh", "failed to create vertex buffers");
			
			TGen::MD3::VertexDecl::Type * vbpos = reinterpret_cast<TGen::MD3::VertexDecl::Type *>(mesh->vb->lock(TGen::LockDiscard | TGen::LockWrite));
			
			for (int i = 0; i < surface->num_verts; ++i) {
				TGen::MD3::TexCoord * texCoord = &surface->texCoords[i];
				TGen::MD3::Vertex * vertex = &surface->vertices[i];
				
				scalar lat = ((vertex->normal >> 8) & 255) * (2.0 * TGen::PI) / 255;
				scalar lng = (vertex->normal & 255) * (2.0 * TGen::PI) / 255;
				
				TGen::Vector3 normal;
				normal.x = cos(lat) * sin(lng);
				normal.y = cos(lng);
				normal.z = sin(lat) * sin(lng);
				normal.normalize();
				
				vbpos->x = float(vertex->x) * scale;
				vbpos->y = float(vertex->z) * scale;
				vbpos->z = float(vertex->y) * scale;
				vbpos->u = texCoord->st[0];
				vbpos->v = texCoord->st[1];
				vbpos->nx = normal.x;
				vbpos->ny = normal.y;
				vbpos->nz = normal.z;
				
				vbpos++;
			}
			
			mesh->vb->unlock();
			
			TGen::MD3::IndexDecl::Type * ibpos = reinterpret_cast<TGen::MD3::IndexDecl::Type *>(mesh->ib->lock(TGen::LockDiscard | TGen::LockWrite));
			
			std::cout << "MD3 IB OFFSET: " <<  mesh->ib->getReadOffset() << std::endl;
			
			for (int i = 0; i < surface->num_triangles; ++i) {
				TGen::MD3::Triangle * triangle = &surface->triangles[i];
				*(ibpos++) = triangle->indexes[2] + mesh->vb->getReadOffset();
				*(ibpos++) = triangle->indexes[1] + mesh->vb->getReadOffset();
				*(ibpos++) = triangle->indexes[0] + mesh->vb->getReadOffset();
			}
			
			mesh->ib->unlock();
			
			newModel->addMesh(mesh);
		}
	}
	
	for (int i = 0; i < header->num_tags; ++i) {
		TGen::ModelJoint joint;
		Tag * tag = &header->tags[i];
		joint.origin = TGen::Vector3(tag->origin.x, tag->origin.z, tag->origin.y) * scale;
		
		TGen::Matrix3x3 orientation(TGen::Vector3(tag->axis[0].x, tag->axis[0].z, tag->axis[0].y),
											 TGen::Vector3(tag->axis[1].x, tag->axis[1].z, tag->axis[1].y),
											 TGen::Vector3(tag->axis[2].x, tag->axis[2].z, tag->axis[2].y));
		
		joint.orientation = TGen::Quaternion4(orientation);
		
		newModel->addJoint(reinterpret_cast<char *>(tag->name), joint);
	}
	
	return newModel;	
}

TGen::MD3::Model * TGen::MD3::File::createAnimatingModel(TGen::VertexDataSource & dataSource, scalar scale) const {
	
	exit(1);
}

// TODO: OPT: VertexCache, IndexCache
