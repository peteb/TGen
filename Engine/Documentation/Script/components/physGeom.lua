--- Geometry for collision detection.
-- Implements the component interface.
-- @class table
-- @name physGeom

---------------------------------------------------------------------------------------
-- Event, called when the geometry collides with another geometry.
-- @param with The geom it collided with.
function physGeom:onCollision(with)

---------------------------------------------------------------------------------------
-- Returns the linked object, either a physBody, worldInterface or nil.
function physGeom:link()
