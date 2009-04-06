--- Physical body.
-- Implements the component interface.
-- @class table
-- @name physBody

---------------------------------------------------------------------------------------
-- Enables the body.
-- Bodies fall asleep when they haven't been touched for a while.
function physBody:enable()

---------------------------------------------------------------------------------------
-- Resets all accumulated forces acting on the body.
function physBody:resetForces()
