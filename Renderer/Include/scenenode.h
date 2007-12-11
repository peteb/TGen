/*
 *  scenenode.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/13/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

//// summary ///////////////////////////////////////////////////////////////////////////////////////
// A Scene Node stores relative positions and orientations in a tree and bounding volumes (sphere
// and AABB, one node's BV is big enough to hold everything in it + it's children):
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _TGEN_RENDERER_SCENENODE_H
#define _TGEN_RENDERER_SCENENODE_H

#include <tgen_math.h>
#include <vector>

namespace TGen {
	class Face;
	class RenderList;
	class Camera;
	
	class SceneNode {
	public:
		typedef std::vector<Face> FaceList;
		class Walker;
		
		SceneNode(const std::string & name, const TGen::Vector3 & position = TGen::Vector3(0.0f, 0.0f, 0.0f), const TGen::Quaternion4 & orientation = TGen::Quaternion4(0.0f, 0.0f, 1.0f));
		virtual ~SceneNode();
		
		virtual void update();
		virtual void calculateFacesBV();
		virtual void calculateWorldBV();
		virtual bool fillFaces(TGen::RenderList & list, const TGen::Camera & camera) const;
		virtual bool fillUser(TGen::RenderList & list, const TGen::Camera & camera) const;
		
		void updateChildren();
		void addChild(TGen::SceneNode * node);
		void removeChild(TGen::SceneNode * node);
		void addFace(const TGen::Face & face);
		TGen::SceneNode * getChild(const std::string & name);
		TGen::SceneNode * getNode(const std::string & path, bool create = false);
		
		TGen::AABB getChildrenBoundingBox() const;
		
		void setPosition(const TGen::Vector3 & position);
		void setOrientation(const TGen::Vector3 & orientation);
		TGen::Vector3 getLocalPosition() const;
		TGen::Quaternion4 getLocalOrientation() const;
		TGen::Vector3 getWorldPosition() const;
		TGen::Quaternion4 getWorldOrientation() const;
		
		bool hasChanged() const;
		//const FaceList & getFaces() const;
		FaceList & getFaces();
		const std::string & getName() const;
		const TGen::Matrix4x4 & getTransform() const;		
		const TGen::AABB & getLocalBoundingBox() const;
		const TGen::AABB & getWorldBoundingBox() const;
		const TGen::Sphere & getLocalBoundingSphere() const;
		const TGen::Sphere & getWorldBoundingSphere() const;

		//void traverse(TGen::SceneNode::Walker & walker);
		void traverse(const TGen::SceneNode::Walker & walker);

		
		class Walker {
		public:
			Walker() {}
			virtual ~Walker() {}
			
			virtual bool pre(TGen::SceneNode & node) const abstract;
			virtual void post(TGen::SceneNode & node) const {}
		};		
		
	private:
		void attached(TGen::SceneNode * parent);
		void detached();
		
	protected:
		typedef std::vector<SceneNode *> SceneNodeList;
		
		std::string name;
		SceneNode * parent;
		TGen::Vector3 position, up;
		TGen::Quaternion4 orientation;
		TGen::Matrix4x4 transform;
		TGen::AABB localBoundingBox, worldBoundingBox;
		TGen::Sphere localBoundingSphere, worldBoundingSphere;
		
		SceneNodeList children;
		FaceList faces;
		
		bool changed;
	};
	
} // !TGen

#endif // !_TGEN_RENDERER_SCENENODE_H
