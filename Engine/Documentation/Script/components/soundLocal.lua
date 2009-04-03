--- A local sound source.
-- Spawned sounds will be updated with position and velocity.
-- @class table
-- @name soundLocal

---------------------------------------------------------------------------------------
-- Spawn a new sound from the source.
-- @param sound The sound to play, get from resources.
-- @usage <code>entity.soundLocal:playSound(resources.sound("ouch.wav"))</code>
function soundLocal:playSound(sound)

