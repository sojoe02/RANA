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
myX = 0
myY = 0
ID = 0
stepPrecision = 0
eventPrecision = 0

-- Import valid Rana lua libraries.
EventLib = require "ranalib_event"
Util = require "ranalib_utility"

-- Init of the lua frog, function called upon initilization of the LUA auton.
function initializeAgent(x, y, id, stepPrecision, eventPrecision)

	myX = x
	myY = y
	ID = id
	stepPrecision = stepPrecision
	eventPrecision = eventPrecision

	l_debug("Agent #: " .. ID .. " has been initialized")

end

function handleEvent(sourceX, sourceY, sourceID, eventDescription, eventTable)

	realtable = EventLib.loadTable(eventTable)

	
	
	--if eventDescription == "ping" then
	--	l_print("Agent: "..ID .." received a ping from agent: "
	--		..sourceID.." emitting pong")
--
  --              EventLib.emit{speed=343, description="pong"}

	--elseif eventDescription == "pong" then
	--	l_print("Agent: "..ID.." received a pong from agent: ".. sourceID)

--	end
--

end

function takeStep()


	if l_getMersenneInteger(1,1000) <= 1 then
		l_debug("Agent:"..ID.." is emiting ping")
                EventLib.emit{speed=343, description="ping",table={desc="something", id=ID}}
	end

end

function synchronizePosition()
	return myX, myY
end

function cleanUp()
	l_debug("Agent #: " .. ID .. " is done\n")
end

