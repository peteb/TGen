--- A sound source.
-- Spawns sounds. There are two kinds of sources; soundLocal and soundGlobal.
-- @class table
-- @name soundSource

---------------------------------------------------------------------------------------
-- Spawn a new sound from the source.
-- @param sound The sound to play, get from resources.
-- @usage <code>entity.soundLocal:playSound(resources.sound("ouch.wav"))</code>
function soundSource:playSound(sound)

---------------------------------------------------------------------------------------
-- Stops all sounds from the source and removes all channels
function soundSource:stopAllChannels()

---------------------------------------------------------------------------------------
-- Called when a channel has stopped playing.
function soundSource:onChannelFinished()
