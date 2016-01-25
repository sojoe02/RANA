local RanaLibAPI = {}

local _EmitEvent
local serializeTable

--check is a boolean options is a table.
function RanaLibAPI.EmitEvent(options)

	_EmitEvent(
		options.propagationSpeed or 0,
		options.desc or "event",
		options.table or false,
		options.targetID or 0,
		options.targetGroup or 0
	)

end

_EmitEvent = function(propagationSpeed, desc, table, targetID, targetGroup)

	if table then
		tableString = serializeTable(table)
	else
		tableString = ""
	end

	l_emitEvent(ID, posX, posY, pSpeed, table, desc, targetID, targetGroup)
end

function RanaLibAPI.ShareTable(key, table, check)

	local check or check

	local status = 0

	if check

	if type(table)=="table" and type(key)~="table" then
		
		tableString = ranaLibAPI.serialize(table)
		status = l_addSharedString(key, String)
		return status --status will be 1 for new key, 2 when overwriting an existing key 
	else 
		
		l_debug("failure to store table, table and key is not of correct type")
		status = nil
		return status
	end 
end

function RanaLibAPI.GetSharedTable(key)

	if type(key)~="table" then

		l_getSharedString(key)
	end

	return ranaLibAPI.loadStringTable(string)

end 

function RanaLibAPI.LoadStringTable(string)

	if string.len(string) < 3 then 

		return nil
	end
	
	load("table="..string)()
	
	return table

end

-- local functions:
--

serializeTable = function(val, name, debth)
	--skipnewlines = skipnewlines or false
	depth = depth or 0
	local tbl = string.rep("", depth)
	if name then
		if type(name)=="number" then
			namestr = "["..name.."]"
			tbl= tbl..namestr.."="
		elseif name then 
			tbl = tbl ..name.."="
		end	
	end
	
	if type(val) == "table" then
			tbl = tbl .. "{"
			local i = 1
		for k, v in pairs(val) do
			if i ~= 1 then
				tbl = tbl .. ","
			end	
			tbl = tbl .. Table.serialize(v,k, depth +1) 
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

return RanaLibAPI
