local ranalib_utility = {}

-- Deserialize an event table
function RanaLibUtility.deserializeTable(string)

	if string.len(string) < 3 then 
		return nil
	end
	
	load("table="..string)()
	return table
end

function RanaLibUtility.serializeTable = function(val, name, debth)
	--skipnewlines = skipnewlines or false
	depth = depth or 0
	local tbl = string.rep("", depth)
	if name then
		if name then
			namestr = "["..name.."]"
			tbl= tbl..namestr.."="
		end	
	end
	
	if type(val) == "table" then
			tbl = tbl .. "{"
			local i = 1
		for k, v in pairs(val) do
			if i ~= 1 then
				tbl = tbl .. ","
			end	
			tbl = tbl .. ranalib_utility.serializeTable(v,k, depth +1) 
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

return ranalib_utility
