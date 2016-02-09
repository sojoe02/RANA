function _HandleEvent(sourceX, sourceY, originID, description, serialTable)
	
	--local eventTable = {}
	if string.len(serialTable) > 3 then 
		load("_eventTable="..serialTable)()
	else 
		_eventTable = {}
	end

	handleEvent(sourceX, sourceY, originID, description, _eventTable)

end
