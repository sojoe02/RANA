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
--
local lib_table = "lib_table"

posX = 0
posY = 0
goalPosX = 0
goalPosY = 0
envX = 0
envY = 0
speed = 100 -- speed in m/s

goalThresshold = 2
closenessThresshold = 25
repulseFactor = 5

phaseDelay = 2
phaseCountdown = phaseDelay --issue a position event every 10 seconds.

countdownDelay = 10
countdown = countdownDelay 


ID = 0
macroF = 0
timeRes = 0

S_Moving = "sMoving"
S_Repositioning = "sRepo"
SS_Repulsing = "ssRepulse"
SS_Attraction = "ssAttract"

ActiveState = S_Moving
S_ActiveState = SS_Repulsing


-- Init of the lua frog, function called upon initilization of the LUA auton:
function initAuton(x, y, id, macroFactor, timeResolution)

	posX = x
	posY = y
	ID = id
	macroF = macroFactor
	timeRes = timeResolution

	envX, envY = l_getEnvironmentSize()

	l_debug("Agent #: " .. id .. " has been initialized")

end

-- Event Handling:
function handleEvent(origX, origY, origID, origDesc, origTable)
	--make a response:
	--
	local distance = l_distance(posX, posY, origX, origY)

	if S_ActiveState == SS_Attraction and distance <= closenessThresshold  and origID > ID then 
		
		goalPosX = origX
		goalPosY = origY

	

	elseif S_ActiveState == SS_Repulsing and distance <= closenessThresshold  then

		if origX < posX and posX + repulseFactor <= envX then
			goalPosX = posX + repulseFactor
		elseif origX >= posX and posX - repulseFactor > 0 then
			goalPosX = posX - repulseFactor
		end

		if origY < posY and posY + repulseFactor <= envY then 
			goalPosY = posY + repulseFactor
		elseif origY >= posY and posY - repulseFactor > 0 then
			goalPosY = posY - repulseFactor
		end

	end

	return 0,0,0,"null"
end	

--Determine whether or not this Auton will initiate an event.
function initiateEvent()
	
	if ActiveState == S_Moving then

		local status = move(speed)		
		if status == 0 then ActiveState = S_Repositioning end

	elseif ActiveState == S_Repositioning then

		goalPosX = l_getRandomInteger(1, envX)
		goalPosY = l_getRandomInteger(1, envY)

		ActiveState = S_Moving
	end

	phaseCountdown = phaseCountdown - (macroF * timeRes)

	--l_debug("counting down phase : "..phaseCountdown)

	if phaseCountdown <= 0 then

		phaseCountdown = phaseDelay

		if S_ActiveState == SS_Attraction then
			S_ActiveState = SS_Repulsing
		else
			S_ActiveState = SS_Attraction
		end

	end

	countdown = countdown - 1 -- (macroF * timeRes)

	if countdown <= 0 then
		
		countdown = countdownDelay
		return 0, "{}", "position_broadcast", 0

	end

	return 0,0,0,"null"

end


function getSyncData()
	return posX, posY
end

function simDone()
	--if ID ==  1 then
		positionTable = {}
                positionTable = l_checkPosition(posX, posY);
		l_debug("---start---")
		for i = 1, #positionTable do 
			l_debug(positionTable[i])
		end
	--end
	l_debug("Agent #: " .. ID .. " is done\n")
end

--function to move around the map, given an angle.
function move(speed, convergence)

	local distance = macroF * timeRes * speed

	posXAbs = math.abs(goalPosX - posX)
	posYAbs = math.abs(goalPosY - posY)

	if posXAbs <= goalThresshold and posYAbs <=goalThresshold then

		return 0

	end

	if posXAbs > goalThresshold then

		if goalPosX > posX then
			posX = posX + distance
		else 
			posX = posX - distance

		end
	end

	if posYAbs > goalThresshold then

		if goalPosY > posY then
			posY = posY + distance
		else
			posY = posY - distance

		end
	end

	l_updatePosition(posX, posY, newPosX, newPosY, ID)

	return 1
end 
