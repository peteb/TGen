--- Namespace for manipulating the environment.
-- @class table
-- @name env
env = {};

---------------------------------------------------------------------------------------
-- Displays a window containing the message.
-- The title is optional.
-- @param title The title, optional.
-- @param message The message to be shown.
function env.showMessage(title, message)

---------------------------------------------------------------------------------------
-- Quits the environment/game/application.
-- @param code The exit code, optional.
function env.quit(code)
