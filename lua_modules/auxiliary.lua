function _HandleEvent(sourceX, sourceY, originID, description, serialTable)
	
	--local eventTable = {}
	if string.len(serialTable) > 3 then 
		load("_eventTable="..serialTable)()
	else 
		_eventTable = {}
	end
	
	if handleEvent ~= nil then
		handleEvent(sourceX, sourceY, originID, description, _eventTable)
	end

end

function _ProcessEventFunction(sourceX, sourceY, posX, posY, time, serialTable)
	
	--
	if string.len(serialTable) > 3 then
		load("_eventTable="..serialTable)()
	else
		_eventTable = {}
	end

	if processEventFunction == nil then

		if posX == sourceX and posY == sourceY then
			return 1
		else
			return .5
		end
	else 
		return processEventFunction(sourceX, sourceY, posX, posY, time, _eventTable)
	end

end
