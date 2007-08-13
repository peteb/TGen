/*
 *  scenenode.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/13/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_SCENENODE_H
#define _TGEN_RENDERER_SCENENODE_H

#include <tgen_math.h>
#include <vector>

namespace TGen {
	class Face;
	
	class SceneNode {
	public:
		typedef std::vector<Face *> FaceList;
		class Walker;
		
		SceneNode(const std::string & name, const TGen::Vector3 & position, const TGen::Quaternion4 & orientation = TGen::Quaternion4(0.0f, 0.0f, 1.0f));
		virtual ~SceneNode();
		
		virtual void update();
		void updateChildren();
		virtual void calculateFacesBV();
		virtual void calculateWorldBV();
		
		void addChild(TGen::SceneNode * node);
		void removeChild(TGen::SceneNode * node);
		void addFace(TGen::Face * face);
		
		TGen::AABB getChildrenBoundingBox() const;
		
		TGen::Vector3 getLocalPosition() const;
		TGen::Quaternion4 getLocalOrientation() const;
		TGen::Vector3 getWorldPosition() const;
		TGen::Quaternion4 getWorldOrientation() const;
		
		bool hasChanged() const;
		const FaceList & getFaces() const;
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