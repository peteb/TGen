--- Base class for components.
-- @class table
-- @name component

---------------------------------------------------------------------------------------
-- The component belongs to an entity.
-- @return The owning entity.
-- @see entity
function component:owner()

---------------------------------------------------------------------------------------
-- A component has a name. 
-- The name is either the component type (physGeom, sceneNode, etc) or the name of the component if one has been given.
-- @return The name of the component as a string.
function component:name()



