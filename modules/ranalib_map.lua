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

local RanaLibMap= {}

-- checks a position on the map and returns a table with the  R,G,B 
-- values at that position.
function RanaLibMap.checkColor(x, y)

	local r,g,b = l_checkMap(x,y)

	return {r,g,b}
end

--modifies a positions color on the map. with a new RGB value
function RanaLibMap.modifyColor(x, y, color)

	l_modifyMap(x,y, color[1],color[2],color[3])

	--agentSay(color[2])

end

--retrieves a radial matrice with legal coordinates.
function RanaLibMap.getRadialMask(radius)

	local table
		
		if type(radius) == "number" and radius > 0 then

			table = l_getMaskRadial(radius, PositionX, PositionY)

		end

	return table
end

function RanaLibMap.radialMapScan(radius)

	local table

	if type(radius) == "number" and radius > 0 then

			table = l_radialMapScan(radius, PositionX, PositionY)

	end

	--if table[1]== nil then return nil end  

	return table
end

function RanaLibMap.radialMapColorScan(radius, r, g, b)

	local table

	if type(radius) == "number" and radius > 0 then

			table = l_radialMapColorScan(radius, PositionX, PositionY, r, g, b)

	end

	--if table[1]==nil then return nil end

	return table

end

-- modiffies the map with a new color only if the current color on the map corresponds
-- to a given color. This ensures that agent don't try and modify a pixel color twice.
function RanaLibMap.quantumModify(x, y, check_color, change_color) 

	return l_checkMapAndChange(x,y, check_color[1], check_color[2], check_color[3], 
	change_color[1], change_color[2], change_color[3] )

end

return RanaLibMap
