local RanaLibAPI = {}

local utility = require "ranalib_utility"

local _EmitEvent
local _SerializeTable

-- Emit an event, using an option table to enable varying parameters.
-- Example: emitEvent{speed=343, description="call", table={intensity=5}}
function RanaLibAPI.emitEvent(options)

	_EmitEvent(
		options.speed or 0,
		options.description or "event",
		options.table or false,
		options.targetID or 0,
		options.targetGroup or 0
	)
end

-- Submit a shared table to a central register available to all agents.
-- The table is stored in serialized form in a hashmap, using a string key as index
-- @param check, it's possible to enable check for table type, 
-- this will adversely affect performance)
-- Example: API.shareTable("agent1",{myX, myY, myID}, false)
function RanaLibAPI.shareTable(key, table, check)

	local check = check or true
	local status = 0

	if check==true then

		if type(table)=="table" and type(key)~="table" then
			tableString = utility.serializeTable(table)
			status = l_addSharedString(key, tableString) 
		else 
			l_debug("failure to store table, table and key is not of correct type")			
		end

	else 
		tableString = utility.serializeTable(table)
		status = l_addSharedString(key, tableString)
	end 

	return status
end

-- Retrieve the shared table from the central register, using a known key.
function RanaLibAPI.getSharedTable(key)

	if type(key)~="table" then
		l_getSharedString(key)
	end

	return ranaLibAPI.loadStringTable(string)
end 

-- Update the agents position in the central collision table.
-- If the agent is currently not in the table it will be added automatically.
function RanaLibAPI.updatePosition(newX, newY)
	
	l_updatePosition(myX, myY, newPosX, newPosY, ID)
end

-- Adds the agents position to the contral collision table.
-- Allows the agent to occupy more than one x,y at a time,
-- for collision check puposes only.
function RanaLibAPI.addPosition()

	l_addPosition(myX, myY, ID)
end

-- Perform a check on a given position.
-- Returns table containing the IDs present at that location.
-- Example(getting table and printing it's contents): 
-- table = API.checkPosition(50,50)
-- for i=#table do l_print(positionTable[i]) end
function RanaLibAPI.checkPosition(x, y)
	
	return l_checkPosition(x, y)
end

-- return true or false whether there is a collision at x,y
function RanaLibAPI.checkCollision(x, y)

	return l_checkCollision(x,y)
end

-- --------------------------------------------------------------
-- Local Functions:
-- --------------------------------------------------------------


_EmitEvent = function(speed, desc, table, targetID, targetGroup)

	if table ~= false then
		tableString = _SerializeTable(table)
	else
		tableString = ""
	end

	l_emitEvent(ID, posX, posY, speed, tableString, desc, targetID, targetGroup)
end


return RanaLibAPI
