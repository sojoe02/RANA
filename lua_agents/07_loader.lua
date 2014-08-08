

-- Init of the lua frog, function called upon initilization of the LUA auton:
function initAuton(x, y, id, macroFactor, timeResolution)	

	if id == 1 then
		dofile([[/home/sojoe/qtProjects/Rana_qt/lua_agents/07_collector.lua]])
		initCollector(x,y,id, macroFactor,timeResolution)


	elseif id == 2 then
		dofile([[/home/sojoe/qtProjects/Rana_qt/lua_agents/07_scanner.lua]])
		initScanner(x,y,id, macroFactor,timeResolution)


	else
		dofile([[/home/sojoe/qtProjects/Rana_qt/lua_agents/07_vacuum.lua]])
		initVacuum(x,y,id, macroFactor,timeResolution)

	end
	
end

function serializeTbl(val, name, depth)
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
			tbl = tbl .. serializeTbl(v,k, depth +1) 
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

--function to change position:
function move(newPosX, newPosY)
	--l_debug("moving from X"..posX..", Y"..posY)
	l_updatePosition(posX, posY, newPosX, newPosY,ID)
	posX = newPosX
	posY = newPosY

end

