--- Interpolates an object's position or orientation across a number of keypoints using different methods.
-- @class table
-- @name interpolator

---------------------------------------------------------------------------------------
-- How fast the interpolation should be, meassured in lengths per seconds.
-- ie; 1.0 = 1 second for the whole path.
-- @param speed The speed in lengths per second
function interpolator:setSpeed(speed)
