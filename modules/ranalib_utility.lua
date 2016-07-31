local RanaLibUtility = {}

-- Deserialize a string representation of a table.
-- This is usually used to deserialize event tables.
function RanaLibUtility.deserializeTable(string)

	if string.len(string) < 3 then 
		return nil
	end
	
	load("stable="..string)()
	return stable
end

-- Generate a string representation of a lua table recursively.
function RanaLibUtility.serializeTable(val, name, depth)
	--skipnewlines = skipnewlines or false
	depth = depth or 0
	local tbl = string.rep("", depth)
	if name then
		if type(name)=="number" then
			namestr = "["..name.."]"
			tbl= tbl..namestr.."="
		elseif name then 
			tbl = tbl ..name.."="
			--else tbl = tbl .. "systbl="
		end	
	end
	if type(val) == "table" then
		tbl = tbl .. "{"
		local i = 1
		for k, v in pairs(val) do
			if i ~= 1 then
				tbl = tbl .. ","
			end	
			tbl = tbl .. RanaLibUtility.serializeTable(v,k, depth +1) 
			i = i + 1
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


return RanaLibUtility
