
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

--Define the function meta table:
func ={}
-- generic function caller:
function func.execute(name, index, ...)
	return func[name]["f"..index](...)
end
func.soundIntensity = {}
function func.soundIntensity.f1(...)
	setPosX, setPosY = ...
return Dthr * 1/(math.exp(l) + 1 )
end


-- Init of the lua frog, function called upon initilization of the LUA auton:
function initAgent(x, y, id, macroFactor, timeResolution)

	posX = x
	posY = y
	ID = id
	macroF = macroFactor
	timeRes = timeResolution

	l_debug("Agent #: " .. id .. " has been initialized")

end

-- Event Handling:
function handleInternalEvent(origX, origY, origID, origDesc, origTable)
	--make a response:

	return 0,0,0,"null"
end	

--Determine whether or not this Auton will initiate an event.
function initiateEvent()
	
		calltable = {name = "communication", index = 2, arg1 = callStrength}
		s_calltable = serializeTbl(calltable) 
		desc = "sound"
		id = l_generateEventID()
		propagationSpeed = 50000
		
		targetID = 0;

		return propagationSpeed, s_calltable, desc, targetID
		


end


function getSyncData()
	return posX, posY
end

function simDone()
	l_debug("Agent #: " .. ID .. " is done\n")
end

function processFunction(posX, posY, callTable)
	--load the callTable:
	loadstring("ctable="..callTable)()
	--handle the call:
	if ctable.name == "" then
		if ctable.index == 1 then
			return func.execute()
		elseif ctable.index == 2 then
			return func.execute()
		end
	end
end
