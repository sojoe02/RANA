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

	return {R=r,G=g,B=b}
end

--modifies a positions color on the map. with a new RGB value
function RanaLibMap.modifyColor(x, y, r, g, b)

	l_modifyMap(x,y, r,g,b)

end

return RanaLibMap
