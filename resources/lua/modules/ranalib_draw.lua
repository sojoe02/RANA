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
-- Usage:
-- 
-- save the module to a variable (local):
-- local <foo> = require "lib_draw"
--
-- Operates with two different colors
-- 'edge' and 'fill' they are table containing and 24bit r,g and b color e.g {255,255,255}. 
-- 'edge' is for 1 pixel outlines, and 'fill' for...well fill.
-- 
-- The following functions are available:
-- | setFillColor(color) 	| sets the 'fill' color, returns true if success, false if not, prints error message
-- | setEdgeColor(color)	| sets the 'edge' color, see setFillColor()
-- | setColors(edge, fill) 	| sets the 'edge' and 'fill' colors, wrapper for the two previous functions, returns true and false.
-- | -----------------------|
-- | circle(x0, y0, radius, filled) | draws a circle originating from coordinate x0 and y0 with the given radius, if 'filled' is true, the circle will be filled with color 'filled'.
-- | floodfill(x0, y0) 		| performs a floodfill from the given coordinates, replacing all adjacent pixels with 'fill', it does so recursively until the original color is different from the one at 'x0' and 'y0'.
-- 
-----------------------------------------------


-----------------------------------------------
-- Initialize the interface:
-----------------------------------------------

local Draw = {}

--define the local function variables
local edgeColor = {255,255,255}
local fillColor = {255,255,255}

local recursive_floodfill

function Draw.setFillColor(color)

	if type(color) ~= "table" or #color ~= 3 then
		l_debug("failed to set fill color, not a table or table length not 3")
		return false

	end
	
	for i = 1, #color do
		if color[i]  > 255 or color[i] < 0 then
			l_debug("fail to set colors the value of color["..i.."]="..color[i].." must be between 0 and 255")
			return false
		end
	end

	fillColor = color

	return true
end

function Draw.setEdgeColor(color)

	if type(color) ~= "table" or #color ~= 3 then
		l_debug("failed to set fill color, not a table or table length not 3")
		return false

	end
	
	for i = 1, #color do
		if color[i]  > 255 or color[i] < 0 then
			l_debug("fail to set colors the value of color["..i.."]="..color[i].." must be between 0 and 255")
			return false
		end
	end

	edgeColor = color

	return true
end


--fetch and set functions for the colors 
function Draw.setColors(edge, fill)

	local success = true

	success = Draw.setEdgeColor(edge)
	success = Draw.setFillColor(fill)

	return success

end 

function Draw.circle(x0, y0, radius, filled)

	local filled = filled or false

	--draw lakeedge:
	local x = radius;
	local y = 0;
	local radiusError = 1 - x;

	re = edgeColor[1]
	ge = edgeColor[2]
	bf = edgeColor[3]

	rf = fillColor[1]
	gf = fillColor[2]
	bf = fillColor[3]

	while x >= y do 	

		l_modifyMap( x + x0,  y + y0, re,ge,be)
		l_modifyMap( y + x0,  x + y0, re,ge,be)
		l_modifyMap(-x + x0,  y + y0, re,ge,be)
		l_modifyMap(-y + x0,  x + y0, re,ge,be)
		l_modifyMap(-x + x0, -y + y0, re,ge,be)
		l_modifyMap(-y + x0, -x + y0, re,ge,be)
		l_modifyMap( x + x0, -y + y0, re,ge,be)
		l_modifyMap( y + x0, -x + y0, re,ge,be)

		if filled == true then
			for i=1, radius do
				l_modifyMap(x + x0 -i, y + y0,   rf,gf,bf)
				l_modifyMap(y + x0 , x + y0-i,   rf,gf,bf)
				l_modifyMap(-x + x0 +i, y + y0,  rf,gf,bf)
				l_modifyMap(-y + x0, x + y0-i,   rf,gf,bf)
				l_modifyMap(-x + x0 +i, -y + y0, rf,gf,bf)
				l_modifyMap(-y + x0, -x + y0 +i, rf,gf,bf)
				l_modifyMap(x + x0 -i, -y + y0,  rf,gf,bf)
				l_modifyMap(y + x0, -x + y0+i,   rf,gf,bf)
			end
		end

		y = y +1;

		if radiusError < 0 then 
			radiusError = radiusError + 2 * y +1
		else
			x = x -1
			radiusError =radiusError + 2 * (y - x) +1
		end

	end
	--node = {x = x0, y = y0 }
	--target_color = {0,0,0
	--floodFill(node, target_color, waterColor)
end


function Draw.floodfill(x0, y0)

	replacement_color = {}
	replacement_color[1], replacement_color[2], replacement_color[3] = l_checkMap(x, y)

	node = { x = x0, y = y0 }

	recursive_floodfill(node, fillColor, replacement_color)

end

recursive_flodfill = function(node, target_color, replacement_color)
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
	recursive_floodfill(newnode, target_color, replacement_color)
	newnode = {x=node.x+1, y = node.y}
	recursive_floodfill(newnode, target_color, replacement_color)
	newnode = {x=node.x, y = node.y+1}
	recursive_floodfill(newnode, target_color, replacement_color)
	newnode = {x=node.x, y = node.y-1}
	recursive_floodfill(newnode, target_color, replacement_color)

end

function Draw.compareColor(color1, color2)

 if color1[1] == color2[1] and color1[2] == color2[2] and color1[3] == color2[3] then
	 return true
 end

 return false

end


return Draw
