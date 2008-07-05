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
#include "md3staticmodel.h"
#include "md3animmodel.h"
#include "md3animmesh.h"
#include "md3staticmesh.h"
#include "mesh.h"
#include "model_new.h"
#include "vertextransformer.h"
#include "vertextransformlist.h"
#include "vertexscaler.h"

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

	if (header->ident != TGen::MD3::MAGIC) {
		free(header);
		throw TGen::RuntimeException("MD3::Parser", "header invalid!");
	}
	
	OffsetPointer(header->frames, header);
	OffsetPointer(header->tags, header);
	OffsetPointer(header->first_surface, header);
	OffsetPointer(header->eof, header);
	
	TGen::MD3::SurfaceList surfaces;
	TGen::MD3::Surface * surface = header->first_surface;

	for (int i = 0; i < header->num_surfaces; ++i) {
		OffsetPointer(surface->triangles, surface);
		OffsetPointer(surface->shaders, surface);
		OffsetPointer(surface->texCoords, surface);
		OffsetPointer(surface->vertices, surface);
		OffsetPointer(surface->next, surface);
		
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


TGen::NewModel * TGen::MD3::File::createModel(TGen::VertexDataSource & dataSource, const TGen::VertexTransformer & transformer) const {
	TGen::VertexTransformList newTransformer;
	newTransformer.addTransformer(new TGen::VertexScaler(TGen::MD3::XYZ_SCALE));	// everything we get from MD3 files should be scaled by this amount
	newTransformer.addTransformer(transformer.clone());
	
	
	
	if (header->num_frames > 1)
		return createAnimatingModel(dataSource, newTransformer);

	return createStaticModel(dataSource, newTransformer);
}

TGen::MD3::StaticModel * TGen::MD3::File::createStaticModel(TGen::VertexDataSource & dataSource, const TGen::VertexTransformer & transformer) const {
	TGen::MD3::StaticModel * newModel = new TGen::MD3::StaticModel(reinterpret_cast<const char *>(header->name), dataSource, "", "");
	
	DEBUG_PRINT("[md3]: creating surfaces...");
	
	// TODO: portalbanor skickar egen user-grej... ClippedRoom (entities to draw, room vertices)
	//       eller sÂ fixas ClippedFace ist‰llet, kanske! dÂ kan man sortera baserat pÂ clipping 
	
	//newModel->setDataSource(&dataSource);
	
	for (int i = 0; i < surfaces.size(); ++i) {
		TGen::MD3::Surface * surface = surfaces[i];
		
		if (surface->ident == TGen::MD3::MAGIC) {
			TGen::MD3::StaticMesh * mesh = new TGen::MD3::StaticMesh(reinterpret_cast<char*>(surface->shaders[0].name), "");
			
			int numIndices = surface->num_triangles * 3;
			int numVertices = surface->num_verts;
			
			mesh->vb = dataSource.createVertexData(TGen::MD3::VertexDecl(), sizeof(TGen::MD3::VertexDecl::Type) * numVertices, TGen::UsageStatic);
			mesh->ib = dataSource.createVertexData(TGen::MD3::IndexDecl(), sizeof(TGen::MD3::IndexDecl::Type) * numIndices, TGen::UsageStatic);
			mesh->numIndices = numIndices;
			
			if (!mesh->vb || !mesh->ib)
				throw TGen::RuntimeException("MD3::File::createMesh", "failed to create vertex buffers");
			
			TGen::MD3::VertexDecl::Type * vbpos = reinterpret_cast<TGen::MD3::VertexDecl::Type *>(mesh->vb->lock(TGen::LockDiscard | TGen::LockWrite));
			
			for (int i = 0; i < surface->num_verts; ++i) {
				TGen::MD3::TexCoord const & texCoord = surface->texCoords[i];
				TGen::MD3::Vertex const & vertex = surface->vertices[i];
				
				TGen::Vector3 normal = normalToVector(vertex.normal);
				TGen::Vector3 coord(vertex.x, vertex.y, vertex.z);
				
				transformer.transform(normal);
				transformer.transform(coord);
				
				vbpos->x = coord.x;
				vbpos->y = coord.y;
				vbpos->z = coord.z;
				vbpos->u = texCoord.st[0];
				vbpos->v = texCoord.st[1];
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
				*(ibpos++) = triangle->indices[2] + mesh->vb->getReadOffset();
				*(ibpos++) = triangle->indices[1] + mesh->vb->getReadOffset();
				*(ibpos++) = triangle->indices[0] + mesh->vb->getReadOffset();
			}
			
			mesh->ib->unlock();
			
			newModel->addMesh(mesh);
		}
	}
	
	for (int i = 0; i < header->num_tags; ++i) {
		TGen::ModelJoint joint;
		Tag * tag = &header->tags[i];
		joint.origin = TGen::Vector3(tag->origin.x, tag->origin.y, tag->origin.z);
		
		TGen::Vector3 axisX(tag->axis[0].x, tag->axis[0].y, tag->axis[0].z);
		TGen::Vector3 axisY(tag->axis[1].x, tag->axis[1].y, tag->axis[1].z);
		TGen::Vector3 axisZ(tag->axis[2].x, tag->axis[2].y, tag->axis[2].z);
		
		transformer.transform(joint.origin);
		transformer.transform(axisX);
		transformer.transform(axisY);
		transformer.transform(axisZ);
		
		TGen::Matrix3x3 orientation(axisX, axisY, axisZ);
		
		joint.orientation = TGen::Quaternion4(orientation);
		
		newModel->addJoint(reinterpret_cast<char *>(tag->name), joint);
	}
	
	return newModel;	
}

TGen::MD3::AnimatingModel * TGen::MD3::File::createAnimatingModel(TGen::VertexDataSource & dataSource, const TGen::VertexTransformer & transformer) const {
	TGen::MD3::AnimatingModel * newModel = new TGen::MD3::AnimatingModel(reinterpret_cast<const char *>(header->name), dataSource);
	
	for (int i = 0; i < surfaces.size(); ++i) {
		TGen::MD3::Surface * surface = surfaces[i];
	
		if (surface->ident == TGen::MD3::MAGIC) {
			TGen::MD3::AnimatingMesh * newMesh = new TGen::MD3::AnimatingMesh(reinterpret_cast<char*>(surface->shaders[0].name), surface->num_verts);
			
			newMesh->reserveIndices(surface->num_triangles * 3);
			newMesh->reserveTexcoords(surface->num_verts);
			
			for (int i = 0; i < surface->num_triangles; ++i) {
				TGen::MD3::Triangle const & triangle = surface->triangles[i];
				newMesh->addIndex(triangle.indices[2]);
				newMesh->addIndex(triangle.indices[1]);
				newMesh->addIndex(triangle.indices[0]);
			}
			
			for (int i = 0; i < surface->num_verts; ++i) {
				TGen::MD3::TexCoord const & texCoord = surface->texCoords[i];
				newMesh->addTexcoord(TGen::MD3::TexCoordDecl::Type(TGen::Vector2(texCoord.st[0], texCoord.st[1])));
			}
			
			for (int i = 0; i < surface->num_frames; ++i) {
				TGen::MD3::AnimationFrame * newFrame = new TGen::MD3::AnimationFrame(reinterpret_cast<char *>(header->frames[i].name));
				
				TGen::Vector3 radius(header->frames[i].radius);
				transformer.transform(radius);
				
				newFrame->radius = radius.getMagnitude();
				newFrame->min = TGen::Vector3(header->frames[i].min_bounds.x, header->frames[i].min_bounds.y, header->frames[i].min_bounds.z);
				newFrame->max = TGen::Vector3(header->frames[i].max_bounds.x, header->frames[i].max_bounds.y, header->frames[i].max_bounds.z);
				newFrame->origin = TGen::Vector3(header->frames[i].local_origin.x, header->frames[i].local_origin.y, header->frames[i].local_origin.z);
				
				transformer.transform(newFrame->min);
				transformer.transform(newFrame->max);
				transformer.transform(newFrame->origin);
			
				newFrame->vertices.clear();
				newFrame->vertices.reserve(surface->num_verts);
				
				int vertexBase = i * surface->num_verts;
				
				for (int i = 0; i < surface->num_verts; ++i) {
					TGen::MD3::Vertex const & vertex = surface->vertices[i + vertexBase];
					
					TGen::Vector3 normal = normalToVector(vertex.normal);
					TGen::Vector3 coord(vertex.x, vertex.y, vertex.z);
					
					transformer.transform(coord);
					transformer.transform(normal);
					
					newFrame->vertices.push_back(TGen::MD3::FrameVertexDecl::Type(coord, normal));
				}
				
				newMesh->addAnimationFrame(newFrame);
			}
			
			newModel->addMeshData(newMesh);
		}
	}
	
	for (int i = 0; i < header->num_tags; ++i) {
		TGen::ModelJoint joint;
		Tag * tag = &header->tags[i];
		joint.origin = TGen::Vector3(tag->origin.x, tag->origin.y, tag->origin.z);
		transformer.transform(joint.origin);
		
		TGen::Vector3 axisX(tag->axis[0].x, tag->axis[0].y, tag->axis[0].z);
		TGen::Vector3 axisY(tag->axis[1].x, tag->axis[1].y, tag->axis[1].z);
		TGen::Vector3 axisZ(tag->axis[2].x, tag->axis[2].y, tag->axis[2].z);
		
		transformer.transform(axisX);
		transformer.transform(axisY);
		transformer.transform(axisZ);
		
		TGen::Matrix3x3 orientation(axisX, axisY, axisZ);
		
		joint.orientation = TGen::Quaternion4(orientation);
		
		newModel->addJoint(reinterpret_cast<char *>(tag->name), joint);
	}
	
	return newModel;
}

TGen::Vector3 TGen::MD3::File::normalToVector(TGen::MD3::S16 normal) {
	scalar lat = ((normal >> 8) & 255) * (2.0 * TGen::PI) / 255;
	scalar lng = (normal & 255) * (2.0 * TGen::PI) / 255;
	
	TGen::Vector3 ret;
	ret.x = cos(lat) * sin(lng);
	ret.z = cos(lng);
	ret.y = sin(lat) * sin(lng);
	ret.normalize();
	
	return ret;
}

// TODO: OPT: VertexCache, IndexCache
