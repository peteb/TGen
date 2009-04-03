--- Something that triggers on an interval.
-- @class table
-- @name timer

---------------------------------------------------------------------------------------
-- How often the timer triggers.
-- @return The interval in seconds.
function timer:interval()

---------------------------------------------------------------------------------------
-- Changes how often the timer triggers.
-- @param interval Interval in seconds.
function timer:setInterval(interval)

---------------------------------------------------------------------------------------
-- Enables the timer.
function timer:enable()

---------------------------------------------------------------------------------------
-- Disables the timer.
function timer:disable()

---------------------------------------------------------------------------------------
-- The component's name
-- @return The name of the timer component as a string
function timer:name()

---------------------------------------------------------------------------------------
-- Event, triggered on every interval
function timer:onTick()
