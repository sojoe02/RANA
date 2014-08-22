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
-- Global variables:
posX = 0 	--autons X position
posY = 0 	--autons Y position
ID = 0		--autons ID
macroF = 0	--simulations macrofactor
timeRes = 0	--simulations time resolution.
numberOfCalls = 0
data_set = {}
total_set = {}

--Define the function meta table:
func ={}
-- generic function caller:
function func.execute(name, index, ...)
	return func[name]["f"..index](...)
end
func.soundIntensity = {}
function func.soundIntensity.f1(...)
	setPosX, setPosY = ...
	x = 0
	y = 0
	if setPosX and setPosY then
		x = setPosX-posX
		y = setPosY-posY
	end
	l = math.sqrt(x*x + y*y)/50-1
	return 1/(math.exp(l)+1)
end
function func.soundIntensity.f2(...)
	Dthr, setPosX, setPosY = ...
	x = 0
	y = 0
	if setPosX and setPosY then
		x = setPosX-posX
		y = setPosY-posY
	end
	l = math.sqrt(x*x + y*y)/ 30 - 1
	return Dthr * 1/(math.exp(l) + 1 )
end



-- Init of the lua frog, function called upon initilization of the LUA auton:
function initCollector(x, y, id, macroFactor, timeResolution)
	posX = x
	posY = y
	ID = id
	macroF = macroFactor
	timeRes = timeResolution

end

-- Handling of an external event
function handleEvent(origX, origY, eventID, eventPropagationSpeed ,eventDesc , eventTable)
	
	if eventDesc == "data" then
		loadstring("ctable="..eventTable)()
		autonID = ctable.originID
		if data_set[autonID] == nil then
			--data_set.autonID = true 
			data_set[autonID] = ctable.sync_points
			--l_debug(data_set[autonID].."\n")
		else
			data_set[autonID] = data_set[autonID] + ctable.sync_points 			--l_debug(data_set.autonID.."\n")
		end
		if total_set[ctable.itr] == nil then

			total_set[ctable.itr] = ctable.sync_points
		else
			total_set[ctable.itr] = total_set[ctable.itr] + ctable.sync_points
		end
		--l_debug(total_set[ctable.itr].."\t")
	end

	return 0,0,"null"
end

-- Handling an internal event.
function handleInternalEvent(origX, origY, origID, origDesc, origTable)

	return 0,0,0,0,0,"null"
end	

--Determine whether or not this Auton will initiate an event.
function initiateEvent()

	--l_debug("macro-step taken at step: ".. l_currentTime())

	return 0,0,0,0,0,"null"
end

function getSyncData()

	return posX, posY
end

function simDone()

	file = io.open("07_data.csv","w")
	for i,v in pairs(data_set) do
		file:write(i..","..v.."\n")	
	end

	file2 = io.open("07_total.csv","w")
	for i,v in pairs(total_set) do
		file2:write(i..","..v.."\n")	
	end
	file:close()
	file2:close()
	--l_debug( data_set[20].. " : "..data_set[10] )
end

function processFunction(posX, posY, callTable)
	--load the callTable:
	loadstring("ctable="..callTable)()
	--handle the call:
	if ctable.name == "soundIntensity" then
		if ctable.index == 1 then
			return func.execute(ctable.name, ctable.index, posX, posY)
		elseif ctable.index == 2 then
			return func.execute(ctable.name, ctable.index, ctable.arg1, posX, posY)
		end
	end
end
