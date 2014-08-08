posX = 0
posY = 0
ID = 0
macroF = 0
timeRes = 0


-- Init of the lua frog, function called upon initilization of the LUA auton:
function initAuton(x, y, id, macroFactor, timeResolution)

	posX = 10
	posY = 10
	ID = id
	macroF = macroFactor
	timeRes = timeResolution

	l_debug("Agent #: " .. id .. " has been initialized")

end

-- Event Handling:
function handleEvent(origX, origY, origID, origDesc, origTable)
	--make a response:

        return 0,0,0,"null"
end	

--Determine whether or not this Auton will initiate an event.
function initiateEvent()

	newPosX = posX + l_getMersenneInteger(0,3)-1;
	newPosY = posY + l_getMersenneInteger(0,3)-1;

	if newPosX > 19 then 
		newPosX = 0
	end

	if newPosX < 0 then 
		newPosX = 19
	end

	if newPosY < 0 then
		newPosY = 19
	end

	if newPosY > 19 then
		newPosY = 0
	end

	--l_debug(newPosX..":"..newPosY)
 	move(newPosX, newPosY)
	

        return 0,0,0,"null"
end


function getSyncData()
	return posX, posY
end

function simDone()
	--if ID ==  1 then
		positionTable = {}
                positionTable = l_checkPosition(posX, posY);
		l_debug("---start---")
		for i = 1, #positionTable do 
			l_debug(positionTable[i])
		end
	--end
	l_debug("Agent #: " .. ID .. " is done\n")
end

--function to change position:
function move(newPosX, newPosY)

	--l_debug("moving from X"..posX..", Y"..posY)
        l_updatePosition(posX, posY, newPosX, newPosY,ID)
	posX = newPosX
	posY = newPosY

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
