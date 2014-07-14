
function serializeTbl(val, name, depth)
	--skipnewlines = skipnewlines or false
	depth = depth or 0
	local tbl = string.rep("", depth)
	if name then 
		tbl = tbl .. name .. " = "
		--else tbl = tbl .. "systbl="
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
-- Global variables:
posX = 0 	--autons X position
posY = 0 	--autons Y position
ID = 0		--autons ID
macroF = 0	--simulations macrofactor
timeRes = 0	--simulations time resolution.
numberOfCalls = 0

point_nosync = 0
point_partialsync = 0
point_sync = 0

create_response = 0
init_event_chance = 1
callDuration = 175 -- call duration in milliseconds

Neighbour_1_interval = 0
Neighbour_2_interval = 0

Neighbour_1_last = 0
Neighbour_2_last = 0

Neighbour_1_ID = 0
Neighbour_2_ID = 0

Neighbour_1_Ilevel = 0
Neighbour_2_Ilevel = 0

synctime = 100 -- when it considers itself in sync in milliseconds

last_call_step = 0

callStrength = 0 --my call strength, sound intensity at origin of my call.
energyRegenRate = 0 --how fast I regenerate energy
energyLevel = 0 --how much energy I have
compellevel = 0.1
compelRegenRate = 0

iterator = 1

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
function initASynchronizer(x, y, id, macroFactor, timeResolution)
	posX = x
	posY = y
	ID = id
	macroF = macroFactor
	timeRes = timeResolution

	compellevel = l_getMersenneFloat(0, 10)
	compelRegenRate = l_getMersenneFloat(0.020,0.04)
	callStrength = l_getMersenneFloat(0.7,1)
	energyLevel = l_getMersenneFloat(0,1)
	energyRegenRate = l_getMersenneFloat(0.1,0.15)
	--l_debug("energy level:\t"..energyLevel.."callstrength:\t"..energyRegenRate.."\n");
end

-- Handling of an external event
function handleExternalEvent(origX, origY, eventID, eventPropagationSpeed ,eventDesc , eventTable)
	if eventDesc == "data" then
		return 0,0,"null"
	end

	if (l_currentTime() - last_call_step) < 10000 then
		point_sync = point_sync + 1
	end
	--calculate call intensity level so I can dismiss it right away
	--load the eventTable:
	loadstring("ctable="..eventTable)()
	ilevel = 0;
	--handle the call:
	if ctable.name == "soundIntensity" then
		if ctable.index == 1 then
			ilevel = func.execute(ctable.name, ctable.index, origX, origY)
		elseif ctable.index == 2 then
			ilevel = func.execute(ctable.name, ctable.index, ctable.arg1, origX, origY)
		end
	end

	if ilevel < 0.1 then
		return 0,0,"null"
	else
		activationTMU = l_speedOfSound(posX, posY, origX, origX, propagationSpeed);
		return "standard",l_generateEventID(),activationTMU;
	end	
end

-- Handling an internal event.
function handleInternalEvent(origX, origY, origID, origDesc, origTable)

	--Determine neigbours
	--if (Neigbour_1_found == false or Neighbour_2_found == false ) then
	ilevel = 0
	--handle the call:
	if ctable.name == "soundIntensity" then
		if ctable.index == 1 then
			ilevel = func.execute(ctable.name, ctable.index, origX, origY)
		elseif ctable.index == 2 then
			ilevel = func.execute(ctable.name, ctable.index, ctable.arg1, origX, origY)
		end
	end
	--determine strongest neighbours:
	if (ilevel > Neighbour_1_Ilevel and origID ~= Neighbour_2) then
		Neighbour_1_Ilevel = ilevel
		Neighbour_1_ID = origID

	elseif (ilevel > Neighbour_2_Ilevel and origID ~= Neighbour_1) then
		Neighbour_2_Ilevel = ilevel
		Neighbour_2_ID = origID
	end
	--end

	if origID == Neighbour_1_ID then

		Neighbour_1_interval = Neighbour_1_last - l_currentTime()
		Neighbour_1_last = l_currentTime()

	elseif origID == Neighbour_2_ID then

		Neighbour_2_interval = Neighbour_2_last - l_currentTime()
		Neighbour_2_last = l_currentTime()		
	end

	return 0,0,0,0,0,"null"
end	

--Determine whether or not this Auton will initiate an event.
function initiateEvent()
	if ( l_currentTime() % (macroF*120000) == 0) then
		--l_debug(l_currentTime().."\n")
		calltable = {originID = ID, sync_points = point_sync, itr = iterator}
		iterator = iterator + 1
		s_calltable = serializeTbl(calltable) 
		desc = "data"
		id = l_generateEventID()
		activationTime = l_currentTime()		
		point_sync = 0
		propagationSpeed = 5000

		return propagationSpeed, s_calltable, desc, id, activationTime, callDuration
		-- if sync level is perfect an energy level is good then make a call immediately.
	elseif (Neighbour_2_interval + Neighbour_2_last >= l_currentTime() + (10 * macroF) and energyLevel > 0.8) or
		(Neighbour_1_interval + Neighbour_1_last >= l_currentTime() + (10 * macroF) and energyLevel > 0.8) then

		calltable = {name = "soundIntensity", index = 2, arg1 = callStrength}
		s_calltable = serializeTbl(calltable) 
		desc = "sound"
		id = l_generateEventID()
		activationTime = l_currentTime()		
		energyLevel = energyLevel - 0.8
		numberOfCalls = numberOfCalls + 1
		propagationSpeed = 343.1
		point_sync = point_sync + 1
		compellevel = 0

		return propagationSpeed, s_calltable, desc, id, activationTime, callDuration

	elseif (energyLevel > 0.8 and compellevel > 9 ) then
		calltable = {name = "soundIntensity", index = 2, arg1 = callStrength}
		s_calltable = serializeTbl(calltable) 
		desc = "sound"
		id = l_generateEventID()
		activationTime = l_currentTime()		
		energyLevel = energyLevel - 0.8
		numberOfCalls = numberOfCalls + 1
		propagationSpeed = 343.1
		point_nosync = point_nosync + 1

		return propagationSpeed, s_calltable, desc, id, activationTime, callDuration
	else 
		energyLevel = energyLevel + (energyRegenRate * macroF * timeRes)
		compellevel = compellevel + (compelRegenRate * macroF * timeRes)
		return 0,0,0,0,0,"null"
	end
end

function getSyncData()
	return posX, posY
end

function simDone()
	--l_debug("Frog #" .. ID .. " made: "..point_nosync.."/"..point_sync.." calls\n")
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
