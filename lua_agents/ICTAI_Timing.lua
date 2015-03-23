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
fired = true
count = 0
propagationSpeed = 343

--The event processing function, needed for postprocessing:
function processFunction(fromX, fromY, toX, toY,time, callTable)

	posX = fromX
	posY = fromY

	local off = 0
	local f = 1.5
	--l_debug(callTable)

	load("ctable="..callTable)()

	local distance = l_distance(toX/f,toY/f,fromX/f,fromY/f)
	
	if ctable.dir == "up" then



		if toX <= fromX+off and toX >= fromX-off  and toY <=fromY then
			--l_debug(l_distance(fromY, fromX, toX, toY))

			if distance <= 0 then
				return ctable.power, ctable.duration
			end
			return ctable.power / distance, ctable.duration
		end

	elseif ctable.dir == "down" then

		if toX <= fromX+off and toX >= fromX-off and toY >=fromY  then

			if distance <= 0 then
				return ctable.power, ctable.duration
			end
			return ctable.power / distance, ctable.duration
		end

	elseif ctable.dir == "right" then

		if toX >= fromX and toY <= fromY+off and toY >=fromY-off then

			if distance <= 0 then
				return ctable.power, ctable.duration
			end
			return ctable.power / distance, ctable.duration
		end

	elseif ctable.dir == "left" then

		if toX <= fromX and toY <= fromY+off and toY >=fromY-off then

			if distance <= 0 then
				return ctable.power, ctable.duration
			end
			--local value = ctable.power / l_distance(fromY, fromX, toX, toY)
			--l_debug(value.." : "..toX..":"..fromX)
			return ctable.power / distance, ctable.duration

		end

	end
	--handle the relevant function:
	--if ctable.f_name == "soundIntensity" then
	--	if ctable.index == 2 then
	return 0, 0
	--	end
	--end

--	return 1,1

end

-- Init of the lua frog, function called upon initilization of the LUA auton:
function initAuton(x, y, id, macroFactor, timeResolution)

	posX = x
	posY = y
	ID = id
	macroF = macroFactor
	timeRes = timeResolution

	if ID == 1 then
		posX = 10
		posY = 10
	elseif ID == 2 then
		posX = 90
		posY = 10
	elseif ID == 3 then
		posX = 90
		posY = 90
	elseif ID == 4 then
		posX = 10
		posY = 90
	end

	l_debug("Agent #: " .. id .. " has been initialized")

end

-- Event Handling:
function handleEvent(origX, origY, origID, origDesc, origTable)

	desc = "ping"
	id = l_generateEventID()
	load("ctable="..origTable)()
	dist = ctable.distance + 80

	if ID == 1 then
		calltable = {dir = "right", duration = 0, power = 1, distance = dist}
		s_calltable = serializeTbl(calltable)
		targetID = 2
		
	elseif ID == 2 then
		calltable = {dir = "down", duration = 0, power = 1, distance = dist}
		s_calltable = serializeTbl(calltable)
		targetID = 3

	elseif ID == 3 then
		calltable = {dir = "left", duration = 0, power = 1, distance = dist}
		s_calltable = serializeTbl(calltable)
		targetID = 4

	elseif ID == 4 then
		calltable = {dir = "up", duration = 0, power = 1, distance = dist}
		s_calltable = serializeTbl(calltable)
		targetID = 1

	end

		if ID == 1 then

			--if count%9999 or count==19999 or count==99999 then
				l_debug("Trip #: "..count.."Time: "..l_currentTime()*l_getTimeResolution()..":"..calltable.dir)
				l_debug("Distance: "..dist)
			--end
		count = count + 1
	end

	return propagationSpeed, s_calltable, desc, targetID
end	

--Determine whether or not this Auton will initiate an event.
function initiateEvent()
	
	if ID == 1 and fired == true then
		fired = false
		calltable = {dir = "right", duration = 5, power = 1,distance = 0}
		s_calltable = serializeTbl(calltable)
		targetID = 2
		desc = "ping"
		id = l_generateEventID()
		count = count + 1
		
		return propagationSpeed, s_calltable, desc, targetID
	end
		
	return 0,0,0,"null"
end


function getSyncData()
	return posX, posY
end

function simDone()
	l_debug("Agent #: " .. ID .. " is done\n")
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
			i = i + 1;
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


