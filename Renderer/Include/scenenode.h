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
	class MaterialSource;
	class NewModelInstance;
	
	class SceneNode {
	public:
		typedef std::vector<Face> FaceList;
		typedef std::vector<TGen::NewModelInstance *> ModelInstanceList;
		
		class Walker;
		
		SceneNode(const std::string & name, const TGen::Vector3 & position = TGen::Vector3(0.0f, 0.0f, 0.0f));
		SceneNode(const std::string & name, const TGen::Vector3 & position, const TGen::Rotation & orientation);
		
		virtual ~SceneNode();
		
		virtual void update(scalar dt);
		virtual void calculateFacesBV();
		virtual void calculateWorldBV();
		virtual bool fillFaces(TGen::RenderList & list, const TGen::Camera & camera) const;
		virtual bool fillUser(TGen::RenderList & list, const TGen::Camera & camera) const;
		virtual bool fillMeta(TGen::RenderList & list, const TGen::Camera & camera) const;
		virtual TGen::SceneNode * getNodeFromPoint(const TGen::Vector3 & point);
		
		virtual void linkMaterials(TGen::MaterialSource & source) {}
		virtual void unlinkMaterials() {}
		
		void updateChildren(scalar dt);
		void addChild(TGen::SceneNode * node);
		void removeChild(TGen::SceneNode * node);
		void addFace(const TGen::Face & face);
		void addModel(TGen::NewModelInstance * model);
		
		TGen::SceneNode * getChild(const std::string & name);
		TGen::SceneNode * getNode(const std::string & path, bool create = false);
		TGen::SceneNode * getParent();
		
		TGen::AABB getChildrenBoundingBox() const;
		
		void setPosition(const TGen::Vector3 & position);
		void setOrientation(const TGen::Rotation & orientation);
		TGen::Vector3 getLocalPosition() const;
		TGen::Rotation getLocalOrientation() const;
		TGen::Vector3 getWorldPosition() const;
		TGen::Rotation getWorldOrientation() const;
		
		void setAutoTP(TGen::SceneNode * autoTP);
		
		bool hasChanged() const;
		bool checkChanged();
		void moveTo(TGen::SceneNode * newParent, bool translate = true);
		
		//const FaceList & getFaces() const;
		FaceList & getFaces();
		ModelInstanceList & getModels();
		const std::string & getName() const;
		const TGen::Matrix4x4 & getTransform() const;		
		const TGen::AABB & getLocalBoundingBox() const;
		const TGen::AABB & getWorldBoundingBox() const;
		const TGen::Sphere & getLocalBoundingSphere() const;
		const TGen::Sphere & getWorldBoundingSphere() const;

		//void traverse(TGen::SceneNode::Walker & walker);
		virtual void traverse(const TGen::SceneNode::Walker & walker);
		
		// this traverse should be overridden by Map to only walk models that are visible
		
		enum WalkerFlags {
			WalkerFollowVisibility		= 0x0001,
			
		};
		
		class Walker {
		public:
			Walker(uint flags = 0) : flags(flags) {}
			virtual ~Walker() {}
			
			virtual bool pre(TGen::SceneNode & node) const abstract;
			virtual void post(TGen::SceneNode & node) const {}
			
			uint getFlags() const {return flags; }
			
		private:
			uint flags;
		};		
		
	private:
		void attached(TGen::SceneNode * parent);
		void detached();
		
		TGen::SceneNode * autoParent;
		
	protected:
		typedef std::vector<SceneNode *> SceneNodeList;
		
		std::string name;
		SceneNode * parent;
		TGen::Vector3 position, up;
		TGen::Rotation orientation;
		TGen::Matrix4x4 transform;
		TGen::AABB localBoundingBox, worldBoundingBox;
		TGen::Sphere localBoundingSphere, worldBoundingSphere;
		
		SceneNodeList children;
		FaceList faces;
		ModelInstanceList models;
		
		bool changed, changedSinceLastCheck;
	};
	
} // !TGen

#endif // !_TGEN_RENDERER_SCENENODE_H
