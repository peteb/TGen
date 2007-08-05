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
	, nodes(NULL)
	, numFaces(0)
	, vertices(NULL)
	, planes(NULL)
	, leaves(NULL)
	, leafFaces(NULL)
	, numVertices(0)
	, meshverts(NULL)
	, numMeshverts(0)
	, numNodes(0)
	, numPlanes(0)
	, numLeaves(0)
	, numLeafFaces(0)
	, SCALE(80.0f)
{
}

BSPLoader::~BSPLoader() {
	delete meshverts;
	delete vertices;
	delete faces;
	delete textures;
	delete nodes;
	delete planes;
	delete leaves;
	delete leafFaces;
}

void BSPLoader::Parse(std::ifstream & file) {
	ReadHeader(file);
	
	ReadTextures(file);
	ReadFaces(file);
	ReadVertices(file);
	ReadMeshverts(file);
	ReadNodes(file);
	ReadPlanes(file);
	ReadLeaves(file);
	ReadLeafFaces(file);
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

void BSPLoader::ReadNodes(std::ifstream & file) {
	numNodes = header.entries[BSPLumpNodes].length / sizeof(Node);
	
	delete nodes;
	nodes = new Node[numNodes];
	
	file.seekg(header.entries[BSPLumpNodes].offset, std::ios::beg);
	if (!file.read(reinterpret_cast<char *>(nodes), header.entries[BSPLumpNodes].length))
		throw TGen::RuntimeException("BSPLoader::ReadNodes", "failed to read nodes");
	
	std::cout << "[bsp]: nodes: " << numNodes << std::endl;			
}

void BSPLoader::ReadPlanes(std::ifstream & file) {
	numPlanes = header.entries[BSPLumpPlanes].length / sizeof(Plane);
	
	delete planes;
	planes = new Plane[numPlanes];
	
	file.seekg(header.entries[BSPLumpPlanes].offset, std::ios::beg);
	if (!file.read(reinterpret_cast<char *>(planes), header.entries[BSPLumpPlanes].length))
		throw TGen::RuntimeException("BSPLoader::ReadPlanes", "failed to read planes");
	
	std::cout << "[bsp]: planes: " << numPlanes << std::endl;			
	
}

void BSPLoader::ReadLeaves(std::ifstream & file) {
	numLeaves = header.entries[BSPLumpLeaves].length / sizeof(Leaf);
	
	delete leaves;
	leaves = new Leaf[numLeaves];
	
	file.seekg(header.entries[BSPLumpLeaves].offset, std::ios::beg);
	if (!file.read(reinterpret_cast<char *>(leaves), header.entries[BSPLumpLeaves].length))
		throw TGen::RuntimeException("BSPLoader::ReadLeaves", "failed to read leaves");
	
	std::cout << "[bsp]: leaves: " << numLeaves << std::endl;			
	
}

void BSPLoader::ReadLeafFaces(std::ifstream & file) {
	numLeafFaces = header.entries[BSPLumpLeafFaces].length / sizeof(LeafFace);
	
	delete leafFaces;
	leafFaces = new LeafFace[numLeafFaces];
	
	file.seekg(header.entries[BSPLumpLeafFaces].offset, std::ios::beg);
	if (!file.read(reinterpret_cast<char *>(leafFaces), header.entries[BSPLumpLeafFaces].length))
		throw TGen::RuntimeException("BSPLoader::ReadLeafFaces", "failed to read leaffaces");
	
	std::cout << "[bsp]: leaffaces: " << numLeafFaces << std::endl;			
	
}

const BSPLoader::StringList & BSPLoader::getTextures() const {
	return materialDeps;
}


void BSPLoader::CreateNodes(TGen::Renderer & renderer, BSPNode * node, int nodeIndex, SurfaceLinker & linker) {
	Node * bspNode = &nodes[nodeIndex];
	Plane * plane = &planes[bspNode->plane * 2];
	node->plane = TGen::Plane3(TGen::Vector3(plane->normal[0], -plane->normal[2], plane->normal[1]), plane->dist / SCALE);
	
	//std::swap(node->plane.normal.y, node->plane.normal.z);
	
	
	//node->plane.normal.Normalize();
	node->min = TGen::Vector3(bspNode->mins[0], bspNode->mins[2], bspNode->mins[1]) / SCALE;
	node->max = TGen::Vector3(bspNode->maxs[0], bspNode->maxs[2], bspNode->maxs[1]) / SCALE;
	//node->aabb.Calculate(node->min, node->max);
	
	// TODO: scale min/max
	
	if (bspNode->children[0] >= 0) {
		node->front = new BSPNode;
		CreateNodes(renderer, node->front, bspNode->children[0], linker);	// TODO: rätta sättet är ju att returnera BSPNode *, BSPLeaf *
	}
	else {
		node->leaf1 = new BSPLeaf;
		CreateLeaf(renderer, node->leaf1, node, -(bspNode->children[0] + 1), linker);
	}
	
	if (bspNode->children[1] >= 0) {
		node->back = new BSPNode;
		CreateNodes(renderer, node->back, bspNode->children[1], linker);
	}
	else {
		node->leaf2 = new BSPLeaf;
		CreateLeaf(renderer, node->leaf2, node, -(bspNode->children[1] + 1), linker);		
	}
}

void BSPLoader::CreateLeaf(TGen::Renderer & renderer, BSPLeaf * leaf, BSPNode * node, int leafIndex, SurfaceLinker & linker) {
	Leaf * bspLeaf = &leaves[leafIndex];
	
	for (int i = bspLeaf->leafface; i < bspLeaf->leafface + bspLeaf->n_leaffaces; ++i) {
		LeafFace * leafFace = &leafFaces[i];
		CreateFace(renderer, leaf, node, leafFace->face, linker);
	}
}

void BSPLoader::CreateFace(TGen::Renderer & renderer, BSPLeaf * leaf, BSPNode * node, int faceIndex, SurfaceLinker & linker) {
	Face * bspFace = &faces[faceIndex];
	// TODO: man borde länka senare... gör så surfaces kan spara material som namn och sen länkas
	
	if (bspFace->type == BSPFaceMesh || bspFace->type == BSPFacePolygon) {
//		if (linker.getMaterial(textures[bspFace->texture].name)->getSortLevel() != TGen::MaterialSortTransparent) {
			BSPGeometry * newGeom = new BSPGeometry(*node, false);
			newGeom->startIndex = 0;
			
			std::vector<MyIndex::Type> indics;
			for (int a = 0; a < bspFace->num_meshvertices; ++a) {
				indics.push_back(meshverts[bspFace->meshvert + a].offset + bspFace->vertex);
			}
			
			newGeom->numIndices = indics.size();
			
			newGeom->ib = renderer.createIndexBuffer(MyIndex(), sizeof(MyIndex::Type) * indics.size(), TGen::UsageStatic);
			newGeom->ib->bufferData(&indics[0], sizeof(MyIndex::Type) * indics.size(), NULL);
			newGeom->vb = globalVB;
			
			leaf->AddSurface(Surface(linker.getMaterial(textures[bspFace->texture].name), newGeom));			
//		}		
	}
/*	else if (currentFace->type == BSPFacePatch) {
		BSPGeometry * newGeom = new BSPGeometry(*tree, true);
		
		
		std::vector<Vertex> verticesbez;
		std::vector<MyIndex::Type> indices;
		int xPos = 0;
		
		for (int a = 0; a < (currentFace->size[0] - 1) / 2; ++a) {
			Bezier bez;
			for (int i = 0; i < 9; i++)
				bez.controls[i] = vertices[xPos + currentFace->vertex + i];
			
			bez.Tessellate(7);
			
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
	}
	else if (currentFace->type == BSPFaceBillboard) {
		std::cout << "BILLBOARD" << std::endl;
	}
*/	
}


BSPTree * BSPLoader::CreateTree(TGen::Renderer & renderer, SurfaceLinker & linker) {
	BSPTree * tree = new BSPTree("bsp");
	
	
	globalVB = renderer.createVertexBuffer(MyVertex(), sizeof(MyVertex::Type) * numVertices, TGen::UsageStatic);
	
	TGen::Vector3 max, min;
	for (int i = 0; i < numVertices; ++i) {
		vertices[i].position[0] /= SCALE;
		vertices[i].position[1] /= SCALE;
		vertices[i].position[2] /= SCALE;
		
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
		
	globalVB->bufferData(vertices, sizeof(Vertex) * numVertices, NULL);
	
	
	
	
	
	
	tree->root = new BSPNode;
	CreateNodes(renderer, tree->root, 0, linker);
	
	return tree;
	
	delete tree->vb;
	tree->vb = renderer.createVertexBuffer(MyVertex(), sizeof(MyVertex::Type) * numVertices, TGen::UsageStatic);
	
	
	//TGen::Vector3 max, min;
	for (int i = 0; i < numVertices; ++i) {
		vertices[i].position[0] /= 80.0f;
		vertices[i].position[1] /= 80.0f;
		vertices[i].position[2] /= 80.0f;
		
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
	
	tree->vb->bufferData(vertices, sizeof(Vertex) * numVertices, NULL);

	
	std::cout << "max: " << std::string(max) << std::endl;
	
	//tree->ib = renderer.CreateIndexBuffer(MyIndex(), sizeof(MyIndex::Type) * numMeshverts, TGen::UsageStatic);
	//tree->ib->BufferData(meshverts, sizeof(Meshvert) * numMeshverts, NULL);
	
	std::cout << "bsp vertex: " << sizeof(Vertex) << " tgen: " << sizeof(MyVertex::Type) << std::endl;
	std::cout << "bsp index: " << sizeof(Meshvert) << " tgen: " << sizeof(MyIndex::Type) << std::endl;
	
	/*IndexMap indicesPerMaterial;
	std::vector<MyIndex::Type> indics;
	
	int biggest = 0, geoms = 0;
	
	for (int i = 0; i < numFaces; ++i) {
		Face * currentFace = &faces[i];
		
		if (currentFace->type == BSPFaceMesh || currentFace->type == BSPFacePolygon) {
			//std::cout << "mesh" << std::endl;
			
			if (linker.getMaterial(textures[currentFace->texture].name)->getSortLevel() != TGen::MaterialSortTransparent) {
				std::vector<MyIndex::Type> & indicesMaterial = indicesPerMaterial[textures[currentFace->texture].name];
				//BSPGeometry * newGeom = new BSPGeometry(*tree, false);
				//newGeom->startIndex = indics.size();
		
				for (int a = 0; a < currentFace->num_meshvertices; ++a) {
					//indics.push_back(meshverts[currentFace->meshvert + a].offset + currentFace->vertex);
					indicesMaterial.push_back(meshverts[currentFace->meshvert + a].offset + currentFace->vertex);				
				}
			
				if (currentFace->num_meshvertices > biggest)
					biggest = currentFace->num_meshvertices;
			
				//newGeom->numIndices = currentFace->num_meshvertices;
			
				geoms++;
			}
			
				//tree->AddSurface(Surface(linker.getMaterial(textures[currentFace->texture].name), newGeom));
			

			
		}
		else if (currentFace->type == BSPFacePatch) {
			BSPGeometry * newGeom = NULL; new BSPGeometry(*tree, true);
						
			
			std::vector<Vertex> verticesbez;
			std::vector<MyIndex::Type> indices;
			int xPos = 0;
			
			for (int a = 0; a < (currentFace->size[0] - 1) / 2; ++a) {
				Bezier bez;
				for (int i = 0; i < 9; i++)
					bez.controls[i] = vertices[xPos + currentFace->vertex + i];
				
				bez.Tessellate(7);
				
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
		}
		else if (currentFace->type == BSPFaceBillboard) {
			std::cout << "BILLBOARD" << std::endl;
		}
	}
	
	
	// Create indexbuffers
	for (IndexMap::iterator iter = indicesPerMaterial.begin(); iter != indicesPerMaterial.end(); ++iter) {
		std::vector<MyIndex::Type> & indicesMaterial = iter->second;
		
		BSPGeometry * newGeom = new BSPGeometry(*tree, false);
		
		newGeom->startIndex = 0;
		newGeom->numIndices = indicesMaterial.size();
		newGeom->ib = renderer.CreateIndexBuffer(MyIndex(), sizeof(MyIndex::Type) * indicesMaterial.size(), TGen::UsageStatic);
		newGeom->ib->BufferData(&indicesMaterial[0], sizeof(MyIndex::Type) * indicesMaterial.size(), NULL);
		
		tree->AddSurface(Surface(linker.getMaterial(iter->first), newGeom));
	}
	
	//tree->ib = renderer.CreateIndexBuffer(MyIndex(), sizeof(MyIndex::Type) * indics.size(), TGen::UsageStatic);
	//tree->ib->BufferData(&indics[0], sizeof(MyIndex::Type) * indics.size(), NULL);
	
	std::cout << "biggest draw: " << biggest << " objects: " << geoms << " ib size: " << std::fixed << std::setprecision(2) << (sizeof(MyIndex::Type) * indics.size()) / 1000.0 << " kb  vb size: " <<  (sizeof(MyVertex::Type) * numVertices) / 1000.0 << " kb " << std::endl;
	
	//std::cout << "GEOMS: " << indicesPerMaterial.size() << std::endl;
	*/
	
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

