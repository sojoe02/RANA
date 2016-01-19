----begin_license--
--
--Copyright 	2013 	Søren Vissing Jørgensen.
--			2014	Søren Vissing Jørgensen, Center for Bio-Robotics, SDU, MMMI.  
--
--This file is part of RANA.
--
--RANA is free software: you can redistribute it and/or modify
--it under the terms of the GNU General Public License as published by
--the Free Software Foundation, either version 3 of the License, or
--(at your option) any later version.
--
--RANA is distributed in the hope that it will be useful,
--but WITHOUT ANY WARRANTY; without even the implied warranty of
--MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--GNU General Public License for more details.
--
--You should have received a copy of the GNU General Public License
--along with RANA.  If not, see <http://www.gnu.org/licenses/>.
--
----end_license--

-- set the global variables:
posX = 0
posY = 0
ID = 0
macroF = 0
timeRes = 0

-- Import valid Rana lua libraries.
API = require "ranalib_api"

-- Init of the lua frog, function called upon initilization of the LUA auton:
function initializeAgent(x, y, id, stepPrecision, eventPrecision)

	posX = x
	posY = y
	ID = id
	macroF = macroFactor
	timeRes = timeResolution

	l_debug("Agent #: " .. ID .. " has been initialized")

end

function handleEvent(origX, origY, eventID, eventDesc, eventTable)

	
	if eventDesc == "ping" then
		l_print("Agent: "..ID .." emits pong")	
		propagationSpeed = 343
		API.emitEvent(progagationSpeed, "pong")
	end

end

function takeStep()


	if l_getMersenneInteger(1,100) <= 5 then

		l_debug("emiting event")
		
		propagationSpeed = 343
		API.emitEvent(propagationSpeed, "ping")

	end

end

function synchronizePosition()
	return posX, posY
end

function cleanUp()
	l_debug("Agent #: " .. ID .. " is done\n")
end

