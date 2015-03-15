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
posX = 0
posY = 0
ID = 0
macroF = 0
timeRes = 0


-- Init of the lua frog, function called upon initilization of the LUA auton:
function initAuton(x, y, id, macroFactor, timeResolution)

	posX = x
	posY = y
	ID = id
	macroF = macroFactor
	timeRes = timeResolution

	l_print("Controller Agent with ID: " .. id .. " has been initialized")

	mapWidth, mapHeight = l_getEnvironmentSize()
	path, filename = l_getAgentPath()

	l_print("my path is"..path.."/"..filename)

end

-- Event Handling:
function handleEvent(origX, origY, origID, origDesc, origTable)
	--make a response:
	return 0,0,0,"null"

end	

--Determine whether or not this Auton will initiate an event.
function initiateEvent()

	if(l_getMersenneInteger(1,1000) == 1) then
		--l_debug("Generating new agent: "..path.." : "..filename)
		local new_id = l_addAuton(l_getMersenneInteger(1,mapWidth), l_getMersenneInteger(1,mapHeight),0,path, [[09_fighter.lua]])
		l_debug("Agent has ID".. new_id)
	end

	return 0,0,0,"null"

end


function getSyncData()
	return posX, posY
end

function simDone()
	l_debug("Agent #: " .. ID .. " is done\n")
end

