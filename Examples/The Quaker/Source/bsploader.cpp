/*
 *  bsploader.cpp
 *  The Quaker
 *
 *  Created by Peter Backman on 7/21/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "bsploader.h"
#include <fstream>

BSPLoader::BSPLoader()
	: textures(NULL)
	, numTextures(0)
	, faces(NULL)
	, numFaces(0)
	, vertices(NULL)
	, numVertices(0)
	, meshverts(NULL)
	, numMeshverts(0)
{
}

BSPLoader::~BSPLoader() {
	delete meshverts;
	delete vertices;
	delete faces;
	delete textures;
}

void BSPLoader::Parse(std::ifstream & file) {
	ReadHeader(file);
	
	ReadTextures(file);
	ReadFaces(file);
	ReadVertices(file);
	ReadMeshverts(file);
}

void BSPLoader::ReadHeader(std::ifstream & file) {
	if (!file.read(reinterpret_cast<char *>(&header), sizeof(Header)))
		throw TGen::RuntimeException("BSPLoader::ReadHeader", "failed to read header");
	
	if (header.magic[0] != 'I' || header.magic[1] != 'B' || header.magic[2] != 'S' || header.magic[3] != 'P')
		throw TGen::RuntimeException("BSPLoader::ReadHeader", "Invalid magic: ") << header.magic[0] << header.magic[1] << header.magic[2] << header.magic[3];

	if (header.version != 0x2E)
		std::cout << "[bsp]: *** WARNING *** version is not quake 3!" << std::endl;
}

void BSPLoader::ReadTextures(std::ifstream & file) {
	numTextures = header.entries[BSPLumpTextures].length / sizeof(Texture);
	
	delete textures;
	textures = new Texture[numTextures];
	
	file.seekg(header.entries[BSPLumpTextures].offset, std::ios::beg);
	if (!file.read(reinterpret_cast<char *>(textures), header.entries[BSPLumpTextures].length))
		throw TGen::RuntimeException("BSPLoader::ReadTextures", "failed to read textures");
	
	materialDeps.clear();
	
	for (int i = 0; i < numTextures; ++i) {
		std::cout << "[bsp]: " << textures[i].name << std::endl;
		materialDeps.push_back(textures[i].name);
	}
	
	std::cout << "[bsp]: textures: " << numTextures << std::endl;
}

void BSPLoader::ReadFaces(std::ifstream & file) {
	numFaces = header.entries[BSPLumpFaces].length / sizeof(Face);

	delete faces;
	faces = new Face[numFaces];
	
	file.seekg(header.entries[BSPLumpFaces].offset, std::ios::beg);
	if (!file.read(reinterpret_cast<char *>(faces), header.entries[BSPLumpFaces].length))
		throw TGen::RuntimeException("BSPLoader::ReadFaces", "failed to read faces");
	
	std::cout << "[bsp]: faces: " << numFaces << std::endl;
}

void BSPLoader::ReadVertices(std::ifstream & file) {
	numVertices = header.entries[BSPLumpVertices].length / sizeof(Vertex);
	
	delete vertices;
	vertices = new Vertex[numVertices];
	
	file.seekg(header.entries[BSPLumpVertices].offset, std::ios::beg);
	if (!file.read(reinterpret_cast<char *>(vertices), header.entries[BSPLumpVertices].length))
		throw TGen::RuntimeException("BSPLoader::ReadVertices", "failed to read vertices");
	
	std::cout << "[bsp]: vertices: " << numVertices << std::endl;	
}

void BSPLoader::ReadMeshverts(std::ifstream & file) {
	numMeshverts = header.entries[BSPLumpMeshverts].length / sizeof(Meshvert);
	
	delete meshverts;
	meshverts = new Meshvert[numMeshverts];
	
	file.seekg(header.entries[BSPLumpMeshverts].offset, std::ios::beg);
	if (!file.read(reinterpret_cast<char *>(meshverts), header.entries[BSPLumpMeshverts].length))
		throw TGen::RuntimeException("BSPLoader::ReadMeshverts", "failed to read meshverts");
	
	std::cout << "[bsp]: meshverts: " << numMeshverts << std::endl;		
}

const BSPLoader::StringList & BSPLoader::getTextures() const {
	return materialDeps;
}

BSPTree * BSPLoader::CreateTree(TGen::Renderer & renderer, SurfaceLinker & linker) {
	BSPTree * tree = new BSPTree("bsp");
	
	delete tree->vb;
	tree->vb = renderer.CreateVertexBuffer(MyVertex(), sizeof(MyVertex::Type) * numVertices, TGen::UsageStatic);
	
	
	TGen::Vector3 max, min;
	for (int i = 0; i < numVertices; ++i) {
		vertices[i].position[0] /= 3.0f;
		vertices[i].position[1] /= 3.0f;
		vertices[i].position[2] /= 3.0f;
		
		std::swap(vertices[i].position[1], vertices[i].position[2]);
		std::swap(vertices[i].normal[1], vertices[i].normal[2]);
		vertices[i].position[1] = -vertices[i].position[1];
		
		if (vertices[i].position[0] > max.x)
			max.x = vertices[i].position[0];
		if (vertices[i].position[1] > max.y)
			max.y = vertices[i].position[1];
		if (vertices[i].position[2] > max.z)
			max.z = vertices[i].position[2];
		
	}

	int maxo = 0;
	int ones = 0;
	for (int i = 0; i < numMeshverts; ++i) {
		if (meshverts[i].offset == 3)
			ones++;
		
		if (meshverts[i].offset > maxo)
			maxo = meshverts[i].offset;
	}
	
	std::cout << "MAXO: " << maxo << " ONES: " << ones << std::endl;
	
	tree->vb->BufferData(vertices, sizeof(Vertex) * numVertices, NULL);

	
	std::cout << "max: " << std::string(max) << std::endl;
	
	//tree->ib = renderer.CreateIndexBuffer(MyIndex(), sizeof(MyIndex::Type) * numMeshverts, TGen::UsageStatic);
	//tree->ib->BufferData(meshverts, sizeof(Meshvert) * numMeshverts, NULL);
	
	std::cout << "bsp vertex: " << sizeof(Vertex) << " tgen: " << sizeof(MyVertex::Type) << std::endl;
	std::cout << "bsp index: " << sizeof(Meshvert) << " tgen: " << sizeof(MyIndex::Type) << std::endl;
	
	IndexMap indicesPerMaterial;
	std::vector<MyIndex::Type> indics;
	
	int biggest = 0, geoms = 0;
	
	for (int i = 0; i < numFaces; ++i) {
		Face * currentFace = &faces[i];
		
		if (currentFace->type == BSPFaceMesh || currentFace->type == BSPFacePolygon) {
			//std::cout << "mesh" << std::endl;
			//std::vector<MyIndex::Type> & indicesMaterial = indicesPerMaterial[textures[currentFace->texture].name];
			BSPGeometry * newGeom = new BSPGeometry(*tree, false);
			newGeom->startIndex = indics.size();
			
			for (int a = 0; a < currentFace->num_meshvertices; ++a) {
				indics.push_back(meshverts[currentFace->meshvert + a].offset + currentFace->vertex);
				//indicesMaterial.push_back(meshverts[currentFace->meshvert + a].offset + currentFace->vertex);				
			}
			
			if (currentFace->num_meshvertices > biggest)
				biggest = currentFace->num_meshvertices;
			
			newGeom->numIndices = currentFace->num_meshvertices;
			
			geoms++;
			tree->AddSurface(Surface(linker.getMaterial(textures[currentFace->texture].name), newGeom));
			
			/*BSPGeometry * newGeom = new BSPGeometry(*tree, false);
		 
			int sizze = currentFace->num_meshvertices;
			
			newGeom->startIndex = 0;
			newGeom->numIndices = sizze;
			newGeom->ib = renderer.CreateIndexBuffer(MyIndex(), sizeof(MyIndex::Type) * sizze, TGen::UsageStatic);
			
			MyIndex::Type * data = new MyIndex::Type[sizze];
			
			for (int a = 0; a < sizze; ++a) {
				data[a] = meshverts[currentFace->meshvert + a].offset + currentFace->vertex;
			}
			
			newGeom->ib->BufferData(data, sizeof(MyIndex::Type) * sizze, NULL);
			delete data;
			
			tree->AddSurface(Surface(linker.getMaterial(textures[currentFace->texture].name), newGeom));*/
			
		}
		else if (currentFace->type == BSPFacePatch) {
			continue;
			BSPGeometry * newGeom = new BSPGeometry(*tree, true);
						
			
			std::vector<Vertex> verticesbez;
			std::vector<MyIndex::Type> indices;
			int xPos = 0;
			
			for (int a = 0; a < (currentFace->size[0] - 1) / 2; ++a) {
				Bezier bez;
				for (int i = 0; i < 9; i++)
					bez.controls[i] = vertices[xPos + currentFace->vertex + i];
				
				bez.Tessellate(8);
				
				for (int i = 0; i < bez.vertices.size(); ++i)
					verticesbez.push_back(bez.vertices[i]);
				
				
				for (int i = 0; i < bez.indices.size(); ++i) {
					indices.push_back(bez.indices[i]);
					
				}
			
				xPos += 6;
				
				for (int i = 0; i < bez.rowIndices.size(); ++i) {
					TGen::IndexBuffer * ib = renderer.CreateIndexBuffer(MyIndex(), sizeof(MyIndex::Type) * bez.trianglesPerRow[i] * 1, TGen::UsageStatic);
					ib->BufferData(bez.rowIndices[i], sizeof(MyIndex::Type) * bez.trianglesPerRow[i] * 1, NULL);
					
					newGeom->multidraw.push_back(std::pair<TGen::IndexBuffer *, int>(ib, bez.trianglesPerRow[i] * 1));
				}
			}
			
			newGeom->startIndex = 0;
			newGeom->numIndices = indices.size();
			newGeom->vb = renderer.CreateVertexBuffer(MyVertex(), sizeof(MyVertex::Type) * verticesbez.size(), TGen::UsageStatic);
			//newGeom->ib = renderer.CreateIndexBuffer(MyIndex(), sizeof(MyIndex::Type) * indices.size(), TGen::UsageStatic);
			
			newGeom->vb->BufferData(&verticesbez[0], sizeof(MyVertex::Type) * verticesbez.size(), NULL);
			//newGeom->ib->BufferData(&indices[0], sizeof(MyIndex::Type) * indices.size(), NULL);
			

			
			
			tree->AddSurface(Surface(linker.getMaterial(textures[currentFace->texture].name), newGeom));
			
			// TODO: fixa igång så man kan dumpa vertices någonstans och som sen rendreras som surface. mesh kan användsa för det, om ib är NULL så blir det DrawPrimitive, inte DrawIndexed
			// TODO: rgbgen, sawtooth, etc
		}
		else if (currentFace->type == BSPFaceBillboard) {
			std::cout << "BILLBOARD" << std::endl;
		}
	}
	
	
	// Create indexbuffers
	/*for (IndexMap::iterator iter = indicesPerMaterial.begin(); iter != indicesPerMaterial.end(); ++iter) {
		std::vector<MyIndex::Type> & indicesMaterial = iter->second;
		
		BSPGeometry * newGeom = new BSPGeometry(*tree, false);
		
		newGeom->startIndex = 0;
		newGeom->numIndices = indicesMaterial.size();
		newGeom->ib = renderer.CreateIndexBuffer(MyIndex(), sizeof(MyIndex::Type) * indicesMaterial.size(), TGen::UsageStatic);
		newGeom->ib->BufferData(&indicesMaterial[0], sizeof(MyIndex::Type) * indicesMaterial.size(), NULL);
		
		tree->AddSurface(Surface(linker.getMaterial(iter->first), newGeom));
	}*/
	
	tree->ib = renderer.CreateIndexBuffer(MyIndex(), sizeof(MyIndex::Type) * indics.size(), TGen::UsageStatic);
	tree->ib->BufferData(&indics[0], sizeof(MyIndex::Type) * indics.size(), NULL);
	
	std::cout << "biggest draw: " << biggest << " objects: " << geoms << " ib size: " << std::fixed << std::setprecision(2) << (sizeof(MyIndex::Type) * indics.size()) / 1000.0 << " kb  vb size: " <<  (sizeof(MyVertex::Type) * numVertices) / 1000.0 << " kb " << std::endl;
	
	//std::cout << "GEOMS: " << indicesPerMaterial.size() << std::endl;
	
	return tree;	
}

TGen::Vector3 BSPLoader::getQuadratic(const TGen::Vector3 & p0, const TGen::Vector3 & p1, const TGen::Vector3 & p2, scalar t) {
	TGen::Vector3 ret;
	scalar it = 1.0 - t;
	ret.x = p0.x * pow(t, 2) + p1.x * 2.0 * t * it + p2.x * pow(it, 2);
	ret.y = p0.y * pow(t, 2) + p1.y * 2.0 * t * it + p2.y * pow(it, 2);
	ret.z = p0.z * pow(t, 2) + p1.z * 2.0 * t * it + p2.z * pow(it, 2);
	
	return ret;
}

void BSPLoader::Bezier::Tessellate(int level) {
	
    // The number of vertices along a side is 1 + num edges
    const int L1 = level + 1;
	
    vertices.resize(L1 * L1);
	
    // Compute the vertices
    int i;
	
    for (i = 0; i <= level; ++i) {
        double a = (double)i / level;
        double b = 1 - a;
		
        vertices[i] =
            controls[0] * (b * b) + 
            controls[3] * (2.0 * b * a) +
            controls[6] * (a * a);
    }
	
    for (i = 1; i <= level; ++i) {
        double a = (double)i / level;
        double b = 1.0 - a;
		
        Vertex temp[3];
		
        int j;
        for (j = 0; j < 3; ++j) {
            int k = 3 * j;
            temp[j] =
                controls[k + 0] * (b * b) + 
                controls[k + 1] * (2.0 * b * a) +
                controls[k + 2] * (a * a);
        }
		
        for(j = 0; j <= level; ++j) {
            double a = (double)j / level;
            double b = 1.0 - a;
			
            vertices[i * L1 + j]=
                temp[0] * (b * b) + 
                temp[1] * (2.0 * b * a) +
                temp[2] * (a * a);
        }
    }
	
	
    // Compute the indices
    int row;
    indices.resize(level * (level + 1) * 2);
	
	
    for (row = 0; row < level; ++row) {
        for(int col = 0; col <= level; ++col)	{
            indices[(row * (level + 1) + col) * 2 + 1] = row       * L1 + col;
            indices[(row * (level + 1) + col) * 2]     = (row + 1) * L1 + col;
        }
    }
	
    trianglesPerRow.resize(level);
    rowIndices.resize(level);
    for (row = 0; row < level; ++row) {
        trianglesPerRow[row] = 2 * L1;
        rowIndices[row]      = &indices[row * 2 * L1];
    }	
}

