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
envWidth = 0
envHeight = 0


-- Init of the lua frog, function called upon initilization of the LUA auton:
function initVacuum(x, y, id, macroFactor, timeResolution)

	posX = x
	posY = y
	ID = id
	macroF = macroFactor
	timeRes = timeResolution

	envWidth, envHeight = l_getEnvironmentSize()

	l_debug("Vacuumer with ID: " .. id .. " has been initialized")

end

-- Event Handling:
function handleEvent(origX, origY, origID, origDesc, origTable)
	--make a response:

		return 0,0,0,"null"
end	

--Determine whether or not this Auton will initiate an event.
function initiateEvent()

	newPosX = posX + l_getMersenneInteger(0,4)-2;
	newPosY = posY + l_getMersenneInteger(0,4)-2;

	if newPosX > envWidth then
		newPosX = 0
	end

	if newPosX < 0 then
		newPosX = envWidth
	end

	if newPosY < 0 then
		newPosY = envHeight
	end

	if newPosY > envHeight then
		newPosY = 0
	end

	--l_debug(newPosX..":"..newPosY)
 	move(newPosX, newPosY)

	r,g,b = l_checkMap(posX,posY)

	if r == 255 then
		l_modifyMap(posX,posY,0,0,0)
	end
	

	return 0,0,0,"null"
end


function getSyncData()
	return posX, posY
end

function simDone()
	l_debug("Agent #: " .. ID .. " is done\n")
end

--function to change position:
function move(newPosX, newPosY)

	--l_debug("moving from X"..posX..", Y"..posY)
	l_updatePosition(posX, posY, newPosX, newPosY,ID)
	posX = newPosX
	posY = newPosY

end

