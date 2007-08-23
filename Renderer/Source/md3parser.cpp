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
#include "mesh.h"

TGen::MD3::Parser::Parser() {}
TGen::MD3::Parser::~Parser() {}

TGen::MD3::File::File(TGen::MD3::Header * header)
	: header(header)
{
}

TGen::MD3::File::~File() {
	if (header)
		free(header);
}


TGen::MD3::File * TGen::MD3::Parser::parse(std::istream & source) {
	source.seekg(0, std::ios::beg);
	uint s1 = source.tellg();
	source.seekg(0, std::ios::end);
	uint s2 = source.tellg();
	uint size = s2 - s1;
	source.seekg(0, std::ios::beg);

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
	PatchPointer(header->surfaces, header);
	PatchPointer(header->eof, header);
	
	for (int i = 0; i < header->num_surfaces; ++i) {
		TGen::MD3::Surface * surface = &header->surfaces[i];
		
		PatchPointer(surface->triangles, surface);
		PatchPointer(surface->shaders, surface);
		PatchPointer(surface->texCoords, surface);
		PatchPointer(surface->vertices, surface);
		PatchPointer(surface->end, surface);
	}	

	return new TGen::MD3::File(header);
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
	
	for (int i = 0; i < header->num_surfaces; ++i) {
		TGen::MD3::Surface * surface = &header->surfaces[i];
		
		if (surface->ident == TGen::MD3::MAGIC) {
			stream << " * ident: " << surface->ident << "\n";
			stream << "   name: " << surface->name << "\n";
			stream << "   flags: " << surface->flags << "\n";
			stream << "   frames: " << surface->num_frames << "\n";
						
			stream << "   shaders: " << surface->num_shaders << "\n";
		
			for (int a = 0; a < surface->num_shaders; ++a) {
				stream << "      name: " << surface->shaders[a].name << "\n";
				stream << "      index: " << surface->shaders[a].shader_index << "\n";
			}
			
			stream << "\n";
		}
	}
	
	stream << "num_skins: " << header->num_skins << "\n";
}


TGen::Mesh * TGen::MD3::File::createMesh(scalar scale) const {
	TGen::Mesh * newMesh = new TGen::Mesh(reinterpret_cast<const char *>(header->name));
	
	return newMesh;
}

