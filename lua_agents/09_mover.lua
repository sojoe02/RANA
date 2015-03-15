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

	strength = l_getMersenneInteger(1,10)

	mapWidth, mapHeight = l_getEnvironmentSize()

	l_print("Mover Agent #: " .. id .. " has been initialized")


end

-- Event Handling:
function handleEvent(origX, origY, origID, origDesc, origTable)
	--make a response:
	if origDesc == "battle" then

		l_print("Agent "..ID.." initiates battle with agent "..origID)

		load("stable="..origTable)()

		if strength >= stable.strength then
			l_removeAgent(origID)
		elseif strength <= stable.strength then
			l_removeAgent(ID)
		else
			local dice = l_getRandomInteger(0,1)
			if dice == 0 then
				l_removeAgent(origID)
			else 
				l_removeAgent(ID)
			end
		end


	end

	return 0,0,0,"null"
end	

--Determine whether or not this Auton will initiate an event.
function initiateEvent()

	move() 
	--check for collisions:
	local collisionTable = scanField(10)
	
	if #collisionTable ~= 0 then
		l_debug("Agent "..ID.."collides with: ".. collisionTable[1])

		--for i=1, #collisionTable do
		--	l_debug(collisionTable[i])
		--end
		--table = serializeTbl({strength = strength})
		--collisionTable[1]
		return 0, serializeTbl({strength=strength}), "battle",collisionTable[1]
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

function scanField(radius)

	local collisionTable ={}

	for i=-radius, radius, 1 do
		for j=-radius, radius, 1 do

			if i ~=0 and j ~=0 then
				if l_checkCollision(posX+i, posY+j) then

					collisionTable = l_checkPosition(posX+i, posY+j)
					return collisionTable

				end
			end
		end
	end

	return collisionTable
end

--function to change position:
function move(newPosX, newPosY)

	local newPosX = posX + l_getMersenneInteger(0,2)-1;
	local newPosY = posY + l_getMersenneInteger(0,2)-1;

	if newPosX > mapWidth then 
		newPosX = 0
	end
	if newPosX < 0 then 
		newPosX = mapWidth
	end
	if newPosY < 0 then
		newPosY = mapHeight
	end
	if newPosY > mapHeight then
		newPosY = 0
	end

	--l_debug("moving from X"..posX..", Y"..posY)
	l_updatePosition(posX, posY, newPosX, newPosY,ID)
	posX = newPosX
	posY = newPosY

end



function serializeTbl(val, name, depth)
	--skipnewlines = skipnewlines or false
	depth = depth or 0
	local tbl = string.rep("", depth)
	if name then
		if type(name)=="number" then
			namestr = "["..name.."]"
			tbl= tbl..namestr.."="
			elseif name then 
				tbl = tbl ..name.."="
				--else tbl = tbl .. "systbl="
			end	
		end
		if type(val) == "table" then
			tbl = tbl .. "{"
			local i = 1
			for k, v in pairs(val) do
				if i ~= 1 then
					tbl = tbl .. ","
				end	
				tbl = tbl .. serializeTbl(v,k, depth +1) 
				i = i + 1
			end
			tbl = tbl .. string.rep(" ", depth) ..  "}"
			elseif type(val) == "number" then
				tbl = tbl .. tostring(val) 
				elseif type(val) == "string" then
					tbl = tbl .. string.format("%q", val)
				else
					tbl = tbl .. "[datatype not serializable:".. type(val) .. "]"
				end

				return tbl
			end
