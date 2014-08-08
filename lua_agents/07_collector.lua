-- Global variables:
posX = -10 	--autons X position
posY = -10 	--autons Y position
ID = 0		--autons ID

data_set = {}

-- Init of the lua frog, function called upon initilization of the LUA auton:
function initCollector(x, y, id, macroFactor, timeResolution)
	posX = -10
	posY = -10
	ID = id
	l_debug("The Collector with ID: " .. id .. " has been initialized")

end

-- Handling of an external event
function handleEvent(origX, origY, origID, origDesc, eventTable)

	if origDesc == "data" then
		l_debug(eventTable)
		load("ctable="..eventTable)()
		if data_set[ctable.itr] == nil then --just a check to see if there is no value
			data_set[ctable.itr] = ctable.dirt
			l_debug("Collector writing dirty data:"..ctable.dirt)
		else --support more than one scanner, makes no sense though...
			data_set[ctable.itr] = data_set[ctable.itr] + ctable.dirt
		end
	end

	return 0,0,0,"null"
end


--Determine whether or not this Auton will initiate an event.
function initiateEvent()

	--l_debug("macro-step taken at step: ".. l_currentTime())

	return 0,0,0,"null"
end

function getSyncData()

	return posX, posY
end

function simDone()

	file = io.open("07_data_set.csv","w")
	for i,v in pairs(data_set) do
		file:write(i..","..v.."\n")	
	end

	file:close()
	--l_debug( data_set[20].. " : "..data_set[10] )
end
