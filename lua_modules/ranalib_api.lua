local ranaLibAPI = {}

function ranaLibAPI.emitEvent(arg_propagationSpeed, arg_desc, arg_table, arg_targetID, arg_targetGroup, arg_ID)

	local table = arg_table or ""
	local targetID = arg_targetID or 0
	local targetGroup = arg_targetGroup or 0
	local ID = arg_ID or ID

	l_debug(ID .. " . "..posX .." . " ..posY.."."   )

	l_emitEvent(ID, posX, posY, arg_propagationSpeed, table, arg_desc, targetID, targetGroup)
end

return ranaLibAPI
