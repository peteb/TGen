--- The resource manager loads and pools resources.
-- @class table
-- @name resources

--- Loads a material.
-- Always returns a material -- a placeholder will be returned if the material isn't found
-- @param name The identifier of the material
-- @return A material object
function resources:material(name)

--- Loads a sound.
-- @param name The identifier of the material, which might be a genline
-- @return A sound object
function resources:sound(name)
