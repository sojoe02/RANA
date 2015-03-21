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

-----------------------------------------------
-- Initialize the interface:
-----------------------------------------------

local Environment = {}

--set default values for the environment colors
local waterColor = {0,0,255}
local shoreColor = {200,110,80}
local landColor  = {0,100,0}

--define the local function variables
local generateLake
local compareColor
local generateCircle
local floodFill



--fetch and set functions for the colors 
function Environment.getColors()
	return waterColor, shoreColor, landColor
end

function Environment.setColors(water, shore, land)

	--check table lengths
	if #water ~= 3 and #shore ~= 3 and #land ~= 3 then 
		return "failed to set new environment colors, table length not correct"
	end
	--TODO check table values
	waterColor = water
	shoreColor = shore
	landColor  = land

	return "New environment colors set"

end

--environment building function:
function Environment.buildEnvironment()

	local width
	local height

	width, height = l_getEnvironmentSize()

	l_debug(width..": "..height)
	local background = landColor

	--color the map
	for i = 0, width do
		for j =0, height do
			l_modifyMap(i,j, background[1], background[2], background[3])
		end
	end
	--test()
	generateLake(width/4, width/2, height/2)

end

--
generateLake = function (radius,x0, y0,resolution)
	resolution = resolution or 20
	--innercircle
	generateCircle(x0, y0, radius, waterColor)

	for i = 1, resolution do
		anglechunk = l_getMersenneInteger(1,360);

		local angle = l_getMersenneInteger(1,359) 
		local x = math.cos((anglechunk*i/180*math.pi)) * radius + x0
		local y = math.sin((anglechunk*i/180*math.pi)) * radius + y0
		if l_getMersenneInteger(1,1) == 1 then
			generateCircle(x,y,l_getMersenneInteger(2,radius), waterColor, true)
		else
			--generateCircle(x,y,l_getMersenneInteger(2,radius),landColor,true)
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
				compareColor(shoreColor, color1) == false and compareColor(shoreColor,color2)==false and
				compareColor(shoreColor, color3) == false and compareColor(shoreColor,color4)==false then

				if compareColor(color1, waterColor) or compareColor(color4, waterColor) or
					compareColor(color2, waterColor)or compareColor(color3, waterColor) then

					l_modifyMap(i,j,shoreColor[1],shoreColor[2],shoreColor[3])

					generateCircle(i,j,5,shoreColor,true)
				end
			end
		end
	end
end

compareColor = function(color1, color2)


	if color1[1] == color2[1] and 
		color1[2] == color2[2] and
		color1[3] == color2[3] then		

		return true
	else 
		return false
	end
end

generateCircle = function(x0, y0, radius, color,filled)

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

	--floodFill(node, target_color, waterColor)
end

floodFill = function(node, target_color, replacement_color)
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


return Environment
