--- WorldObject implements world interface stuff.
-- @class table
-- @name worldobj

--- The object's position in the world.
-- @return The position in world coordinate space as a vec3
function worldobj:worldPosition()

--- The object's orientation in the world.
-- @return The orientation in world space as a mat3/quat.
function worldobj:worldOrientation()

--- The object's velocity in the world.
-- @return The velocity in world space as a vec3.
function worldobj:worldVelocity()

--- Changes the world position of the object by reversing transformations if there are any.
-- @param position Thew new position in world space coordinate system.
function worldobj:setWorldPosition(position)

--- Changes the world orientation of the object by reversing transformations if there are any.
-- @param orientation The new orientation in world space coordinate system.
function worldobj:setWorldOrientation(orientation)

