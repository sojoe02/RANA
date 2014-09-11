----begin_license--
--
--Copyright 	2013 	Søren Vissing Jørgensen.
--			2014	Søren Vissing Jørgensen, Center for Bio-Robotics, SDU, MMMI.  
--
--This file is part of RANA.
--
--RANA is free software: you can redistribute it and/or modify
--it under the terms of the GNU General Public License as published by
--the Free Software Foundation, either version 3 of the License, or
--(at your option) any later version.
--
--RANA is distributed in the hope that it will be useful,
--but WITHOUT ANY WARRANTY; without even the implied warranty of
--MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--GNU General Public License for more details.
--
--You should have received a copy of the GNU General Public License
--along with RANA.  If not, see <http://www.gnu.org/licenses/>.
--
----end_license--
posX = 0
posY = 0
ID = 0
macroF = 0
timeRes = 0

Energy_start = 0
Energy_max = 0
Strength = 0

-- Environment variables:

-- Setting the values for landscape generation:
Color_Water = {0,0,255}
Color_Shore = {255,210,103}
Color_Land = {0,200,0}

-- Modules:
--
--The event function module:
func = {}

function func.execute(name, index, ...)
	return func[name]["f"..index](...)
end

func.soundIntensity = {}
function func.soundIntensity.simple(...)

	local setPosX
	local setPosY
	local power

	setPosX, setPosY, power = ...
	local x = 0
	local y = 0

	if setPosX and setPosY then
		x = setPosX-posX
		y = setPosY-posY
	end

	if not power then power = 50 end
	
	l = math.sqrt(x*x + y*y)/power-1
	return 1/(math.exp(l)+1)
end

--The event processing function, needed for postprocessing:
function processFunction(fromX, fromY, toX, toY, callTable)

	posX = fromX
	posY = fromY

	load("ctable="..callTable)()
	--handle the relevant function:
	if ctable.f_name == "soundIntensity" then
		if ctable.index == 1 then
			return func.execute(ctable.name, ctable.index, toX, toY, ctable.power)
		end
	end

end


-- Init of the lua frog, function called upon initilization of the LUA auton:
function initAuton(x, y, id, macroFactor, timeResolution)

	posX = x
	posY = y
	ID = id
	macroF = macroFactor
	timeRes = timeResolution

	if id == 1 then
		makeEnvironment()
	end

	l_debug("Agent #: " .. id .. " has been initialized")

end

-- Event Handling:
function handleEvent(origX, origY, origID, origDesc, origTable)
	--make a response:
	return 0,0,0,"null"
end	

--Determine whether or not this Auton will initiate an event.
function initiateEvent()

	if l_getMersenneInteger(1,100) <= 1 then
		calltable = {f_name = "soundIntensity", index = 2, arg1 = callStrength}
		s_calltable = serializeTbl(calltable) 
		desc = "mating_call"
		id = l_generateEventID()
		propagationSpeed = 343
		targetID = 0;
		--l_gridMove(posX, posY, newPosX, newPosY)
		--posX = newPosX
		--posY = newPosY
		return propagationSpeed, s_calltable, desc, targetID
	end

	return 0,0,0,"null"
end


function getSyncData()
	return posX, posY
end

function simDone()
	l_debug("Agent #: " .. ID .. " is done\n")
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

--ENVIRONMENT GENERATION-----------------------
--
--
-----------------------------------------------

function makeEnvironment()

	local width
	local height

	width, height = l_getEnvironmentSize()

	l_debug(width..": "..height)
	local background = Color_Land

	--color the map
	for i = 0, width do
		for j =0, height do
			l_modifyMap(i,j, background[1], background[2], background[3])
		end
	end

	generateLake(width/5, width/2, height/2)

end



function generateLake(radius,x0, y0,resolution)
	resolution = resolution or 20
	--innercircle
	generateCircle(x0,y0,radius, Color_Water)

	for i = 1, resolution do
		anglechunk = l_getMersenneInteger(1,360);

		local angle = l_getMersenneInteger(1,359) 
		local x = math.cos((anglechunk*i/180*math.pi)) * radius + x0
		local y = math.sin((anglechunk*i/180*math.pi)) * radius + y0
		if l_getMersenneInteger(1,1) == 1 then
			generateCircle(x,y,l_getMersenneInteger(2,radius), Color_Water, true)
		else
			--generateCircle(x,y,l_getMersenneInteger(2,radius),Color_Land,true)
		end
	end

	local width = 0
	local height = 0

	--make the shore
	width, height =  l_getEnvironmentSize()

	local color1 = {}
	local color2 = {}
	local color3 = {}
	local color4 = {}

	for i = 0, width-1 do
		for j =0, height-1 do

			color1[1], color1[2], color1[3] = l_checkMap(i,j)
			color2[1], color2[2], color2[3]	= l_checkMap(i,j+1)
			color3[1], color3[2], color3[3]	= l_checkMap(i+1,j)
			color4[1], color4[2], color4[3] = l_checkMap(i,j-1)

			if (compareColor(color1,color2)== false or compareColor(color2,color3)== false or
				compareColor(color1,color3)== false or compareColor(color1,color4)== false or
				compareColor(color2,color4)== false or compareColor(color3,color4)== false) and 
				compareColor(Color_Shore, color1) == false and compareColor(Color_Shore,color2)==false and
				compareColor(Color_Shore, color3) == false and compareColor(Color_Shore,color4)==false
				then

					if compareColor(color1, Color_Water) or compareColor(color4, Color_Water) or
						compareColor(color2, Color_Water)or compareColor(color3, Color_Water) then

						l_modifyMap(i,j,Color_Shore[1],Color_Shore[2],Color_Shore[3])

						generateCircle(i,j,5,Color_Shore,true)
					end
				end
			end

		end
	end

	function compareColor(color1, color2)

		if color1[1] == color2[1] and 
			color1[2] == color2[2] and
			color1[3] == color2[3] then		

			return true
		else 
			return false
		end

	end

	function generateCircle(x0, y0, radius, color,filled)

		local filled = filled or false

		--draw lakeedge:
		local x = radius;
		local y = 0;
		local radiusError = 1 - x;

		r = color[1]
		g = color[2]
		b = color[3]

		while x >= y do 	

			l_modifyMap(x + x0, y + y0, r,g,b)
			l_modifyMap(y + x0, x + y0,r,g,b)
			l_modifyMap(-x + x0, y + y0,r,g,b)
			l_modifyMap(-y + x0, x + y0,r,g,b)
			l_modifyMap(-x + x0, -y + y0,r,g,b)
			l_modifyMap(-y + x0, -x + y0,r,g,b)
			l_modifyMap(x + x0, -y + y0,r,g,b)
			l_modifyMap(y + x0, -x + y0,r,g,b)

			if filled == true then
			for i=0, radius do
			l_modifyMap(x + x0 -i, y + y0, r,g,b)
			l_modifyMap(y + x0 , x + y0-i, r,g,b)
			l_modifyMap(-x + x0 +i, y + y0, r,g,b)
			l_modifyMap(-y + x0, x + y0-i, r,g,b)
			l_modifyMap(-x + x0 +i, -y + y0, r,g,b)
			l_modifyMap(-y + x0, -x + y0 +i, r,g,b)
			l_modifyMap(x + x0 -i, -y + y0, r,g,b)
			l_modifyMap(y + x0, -x + y0+i, r,g,b)
			end
			end

			y = y +1;

			if radiusError < 0 then 
			radiusError = radiusError + 2 * y +1
			else
			x = x -1
			radiusError =radiusError + 2 * (y - x +1)
			end

		end

		node = {x = x0, y = y0 }
		target_color = {0,0,0}

		--floodFill(node, target_color, Color_Water)
		end

	function floodFill(node, target_color, replacement_color)
	--If target-color is equal to replacement-color, return.
	if target_color[1] == replacement_color[1] and
	target_color[2] == replacement_color[2] and
	target_color[3] == replacement_color[3] then
	return
	end

		node_color = {}
		node_color[1], node_color[2], node_color[3] = l_checkMap(node.x,node.y)

		--If the color of node is not equal to target-color, return.
		if node_color[1] ~= target_color[1] or
		node_color[2] ~= target_color[2] or
		node_color[3] ~= target_color[3] then
		return
		end

		--Set the color of node to replacement-color.
		l_modifyMap(node.x, node.y, replacement_color[1], replacement_color[2], replacement_color[3])

		--Perform flood-fill on neigbouring nodes;
		newnode = {x=node.x-1, y = node.y}
		floodFill(newnode, target_color, replacement_color)
		newnode = {x=node.x+1, y = node.y}
		floodFill(newnode, target_color, replacement_color)
		newnode = {x=node.x, y = node.y+1}
		floodFill(newnode, target_color, replacement_color)
		newnode = {x=node.x, y = node.y-1}
		floodFill(newnode, target_color, replacement_color)

	end
