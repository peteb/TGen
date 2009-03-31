--- A node in the scene graph.
-- Contains an orientation and local position, and usually one or more graphical objects that should be rendered.
-- The position and orientation are relative to the parent's (a node is the child of a parent) position and orientation.
-- @class table
-- @name sceneNode

---------------------------------------------------------------------------------------
-- Position in world space coordinate system.
-- @return A vec3 with the position of the sceneNode in world/global coordinate system.
function sceneNode:worldPosition()

---------------------------------------------------------------------------------------
-- Position in local space coordinate system.
-- @return A vec3 with the position of the sceneNode in local coordinate system, relative to the parent node.
function sceneNode:localPosition()

---------------------------------------------------------------------------------------
-- Changes overriding material of the sceneNode.
-- All models in the sceneNode will use this material if set.
-- @param material The material to change to, should be retrieved using the resource manager.
-- @see resources
function sceneNode:setMaterial(material)

---------------------------------------------------------------------------------------
-- Changes the local position so that it aligns with the position in world space.
-- @param vec3 The position in world space that will be converted into local space.
function sceneNode:setWorldPosition(vec3)

---------------------------------------------------------------------------------------
-- Changes the local position.
-- @param vec3 The position to change to.
function sceneNode:setLocalPosition(vec3)

---------------------------------------------------------------------------------------
-- The local orientation of the sceneNode.
-- @return The local orientation (relative to parent node) of the sceneNode as a mat3.
function sceneNode:localOrientation()

---------------------------------------------------------------------------------------
-- Changes the local orientation of the sceneNode.
-- @param mat3 The new local orientation of the sceneNode as a mat3.
function sceneNode:setLocalOrientation(mat3)

---------------------------------------------------------------------------------------
-- Changes the local orientation by multiplying the old orientation with the new one.
-- @param mat3 The orientation that the old orientation should be multiplied with.
function sceneNode:multiplyLocalOrientation(mat3)
