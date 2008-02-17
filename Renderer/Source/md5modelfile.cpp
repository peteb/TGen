/*
 *  md5file.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 1/19/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "md5modelfile.h"
#include "md5model.h"
#include "md5mesh.h"

TGen::MD5::ModelFile::ModelFile() 
	: version(0)
	, numMeshes(0)
	, numJoints(0)
{
	
}

TGen::MD5::ModelFile::~ModelFile() {
	
}

void TGen::MD5::ModelFile::printInfo(std::ostream & stream) const {
	stream << "version: " << version << std::endl;
	stream << "num joints: " << numJoints << std::endl;
	stream << "num meshes: " << numMeshes << std::endl;
	stream << "meshes: (" << meshes.size() << ")" << std::endl;
	
	for (int i = 0; i < meshes.size(); ++i)
		meshes[i]->printInfo(stream);
}

void TGen::MD5::ModelFile::setVersion(int version) {
	this->version = version;
}

void TGen::MD5::ModelFile::setNumMeshes(int numMeshes) {
	this->numMeshes = numMeshes;
}

void TGen::MD5::ModelFile::setNumJoints(int numJoints) {
	this->numJoints = numJoints;
}

void TGen::MD5::ModelFile::addMesh(TGen::MD5::FileMesh * mesh) {
	meshes.push_back(mesh);
}


TGen::MD5::FileMesh::FileMesh() 
	: numVerts(0)
	, numTris(0)
	, numWeights(0)
{
	
}

TGen::MD5::FileMesh::~FileMesh() {
	
}

void TGen::MD5::FileMesh::setMaterial(const std::string & material) {
	this->material = material;
}

void TGen::MD5::FileMesh::setNumVerts(int numVerts) {
	this->numVerts = numVerts;
}

void TGen::MD5::FileMesh::setNumTris(int numTris) {
	this->numTris = numTris;
}

void TGen::MD5::FileMesh::setNumWeights(int numWeights) {
	this->numWeights = numWeights;
}

void TGen::MD5::FileMesh::setVertices(const std::vector<TGen::MD5::FileVertex> & vertices) {
	this->vertices = vertices;
}

void TGen::MD5::FileMesh::setTriangles(const std::vector<TGen::MD5::FileTriangle> & triangles) {
	this->triangles = triangles;
}

void TGen::MD5::FileMesh::setWeights(const std::vector<TGen::MD5::FileWeight> & weights) {
	this->weights = weights;
}

void TGen::MD5::FileMesh::printInfo(std::ostream & stream) const {
	stream << "material: " << material << std::endl;
	stream << "num verts: " << numVerts << std::endl;
	stream << "num tris: " << numTris << std::endl;
	stream << "num weights: " << numWeights << std::endl;
	stream << "vertices: (" << vertices.size() << ")" << std::endl;
	
	for (int i = 0; i < vertices.size(); ++i)
		stream << i << " (" << vertices[i].s << " " << vertices[i].t << ") " << vertices[i].startWeight << " " << vertices[i].weightCount << std::endl;
	
	stream << "triangles: (" << triangles.size() << ")" << std::endl;
	
	for (int i = 0; i < triangles.size(); ++i) {
		stream << i << " ";
	
		for (int a = 0; a < 3; ++a)
			stream << triangles[i].indices[a] << " ";

		stream << std::endl;
	}
	
	stream << "weights: (" << weights.size() << ")" << std::endl;
	for (int i = 0; i < weights.size(); ++i)
		stream << i << " " << weights[i].joint << " " << weights[i].bias << " (" << std::string(weights[i].position) << ")" << std::endl;
	

}

const std::vector<TGen::MD5::FileVertex> & TGen::MD5::FileMesh::getVertices() const {
	return vertices;
}

const std::vector<TGen::MD5::FileTriangle> & TGen::MD5::FileMesh::getTriangles() const {
	return triangles;
}

const std::vector<TGen::MD5::FileWeight> & TGen::MD5::FileMesh::getWeights() const {
	return weights;
}

std::string TGen::MD5::FileMesh::getMaterialName() const {
	return material;
}

// TODO: better vertex handling
//       eg: createModel(blabla, "blabla", VertexScale(0.45).SwapComponents(1, 2));

TGen::MD5::Model * TGen::MD5::ModelFile::createModel(TGen::VertexDataSource & dataSource, const std::string & name, scalar scale) const {
	TGen::MD5::Model * newModel = new TGen::MD5::Model(name, dataSource);
	
	for (int i = 0; i < meshes.size(); ++i) {
		TGen::MD5::FileMesh const & fileMesh = *meshes[i];
		TGen::MD5::Mesh * mesh = new TGen::MD5::Mesh(fileMesh.getMaterialName());
		
		
		
		newModel->addMeshData(mesh);
		
		
		/*int numVertices = fileMesh.getVertices().size();
		int numIndices = fileMesh.getTriangles().size() * 3;
		
		mesh->vb = dataSource.createVertexData(TGen::MD5::VertexDecl(), sizeof(TGen::MD5::VertexDecl::Type) * numVertices, TGen::UsageStatic);
		mesh->ib = dataSource.createVertexData(TGen::MD5::IndexDecl(), sizeof(TGen::MD5::IndexDecl::Type) * numIndices, TGen::UsageStatic);
		mesh->indexCount = numIndices;
		
		if (!mesh->vb || !mesh->ib)
			throw TGen::RuntimeException("MD5::File::createMesh", "failed to create vertex buffers");
		
		TGen::MD5::VertexDecl::Type * vertices = reinterpret_cast<TGen::MD5::VertexDecl::Type *>(mesh->vb->lock(TGen::LockDiscard | TGen::LockWrite));
		TGen::MD5::IndexDecl::Type * indices = reinterpret_cast<TGen::MD5::IndexDecl::Type *>(mesh->ib->lock(TGen::LockDiscard | TGen::LockWrite));
		
		for (int a = 0; a < numVertices; ++a) {
			const TGen::MD5::FileVertex & vertex = fileMesh.getVertices()[a];
			
			TGen::Vector3 pos, normal, tangent;
			
			for (int b = 0; b < vertex.weightCount; ++b) {
				const TGen::MD5::FileWeight & weight = fileMesh.getWeights()[vertex.startWeight + b];
				const TGen::MD5::FileJoint & joint = *joints[weight.joint];
				
				TGen::Vector3 wv = joint.orientation.rotatePoint(weight.position);
				
				pos += (joint.position + wv) * weight.bias;
			}

			//std::swap(pos.x, pos.y);
			std::swap(pos.z, pos.y);
			
			pos *= 0.05;
			pos.y -= 3.0;
			
			vertices[a].MD5::Vertex::Type::x = pos.x;
			vertices[a].MD5::Vertex::Type::y = pos.y;
			vertices[a].MD5::Vertex::Type::z = pos.z;
			
			vertices[a].MD5::Normal::Type::nx = normal.x;
			vertices[a].MD5::Normal::Type::ny = normal.y;
			vertices[a].MD5::Normal::Type::nz = normal.z;
			
			vertices[a].MD5::Tangent::Type::x = tangent.x;
			vertices[a].MD5::Tangent::Type::y = tangent.y;
			vertices[a].MD5::Tangent::Type::z = tangent.z;
			
			vertices[a].MD5::TexCoord::Type::u = vertex.s;
			vertices[a].MD5::TexCoord::Type::v = vertex.t;
		}				
		
		uint readOffset = mesh->vb->getReadOffset();
		
		for (int a = 0; a < fileMesh.getTriangles().size(); ++a) {
			const TGen::MD5::FileTriangle & triangle = fileMesh.getTriangles()[a];
			
			indices[a * 3 + 0] = triangle.indices[2] + readOffset;
			indices[a * 3 + 1] = triangle.indices[1] + readOffset;
			indices[a * 3 + 2] = triangle.indices[0] + readOffset;			
		}
		
		mesh->ib->unlock();		
		mesh->vb->unlock();
		
		newMesh->addLeaf(mesh);		*/
		
	
	}
	
	return newModel;
}

