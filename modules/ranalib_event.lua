local RanaLibEvent = {}

local utility = require "ranalib_utility"

local _EmitEvent

-- Emit an event, using an option table to enable varying parameters.
-- Example: emitEvent{speed=343, description="call", table={intensity=5}}
function RanaLibEvent.emit(options)

	_EmitEvent(
		options.speed or 0,
		options.description or "event",
		options.table or false,
		options.targetID or 0,
		options.targetGroup or 0
		)

end

-- Deserializes the serialized table received with an event.
-- @Return Table value or nil, if there is no valid table data.
function RanaLibEvent.loadTable(tableString)
	
	return utility.deserializeTable(tableString)
	
end

-- --------------------------------------------------------------
-- Local Functions:
-- --------------------------------------------------------------


_EmitEvent = function(speed, desc, table, targetID, targetGroup)

	if table ~= false then
		if type(table)=="table" then
			tableString = utility.serializeTable(table)
		elseif type(table)=="string" then
			tableString = table
		end
	else
		tableString = ""
	end

	l_emitEvent(ID, PositionX, PositionY, speed, desc, targetID, targetGroup, tableString, PositionZ)
end


return RanaLibEvent
