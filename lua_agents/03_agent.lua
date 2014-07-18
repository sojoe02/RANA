
-- Init of the lua frog, function called upon initilization of the LUA auton:
function initAuton(x, y, id, macroFactor, timeResolution)

	posX = x
	posY = y
	ID = id
	macroF = macroFactor
	timeRes = timeResolution

	l_debug("Agent #: " .. id .. " has been initialized")
    
	l_modifyMap(10,10,255,0,0)

end

-- Event Handling:
function handleInternalEvent(origX, origY, origID, origDesc, origTable)
	--make a response:

	return 0,0,0,"null"
end	

--Determine whether or not this Auton will initiate an event.
function initiateEvent()

		s_calltable = "empty" 
		desc = "sound"
		id = l_generateEventID()
		propagationSpeed = 50000
		
		targetID = 0;
		
		i = l_checkCollision(posX, posY)

		if ID == 1 then
			l_debug("agents at my position".. i)
		end

		newPosX = 150
		newPosY = 150
		
		if l_gridMove(posX, posY, newPosX, newPosY) == true then
			posX = newPosX
			posY = newPosY
		end


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
