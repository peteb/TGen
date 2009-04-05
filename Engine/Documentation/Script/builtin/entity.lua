--- Basically a container of components.
-- Components of an entity are fields in the table.
-- @class table
-- @name entity

--- Name of entity
-- @return Name as string
function entity:name()

--- Event, triggered when the entity is created and is inside the world
function entity:onCreation()
