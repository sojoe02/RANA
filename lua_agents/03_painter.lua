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

-- Import valid Rana lua libraries.
Stat = require "ranalib_statistic"
Map = require "ranalib_map"

function takeStep()

	local x = Stat.randomInteger(0, ENV_WIDTH)
	local y = Stat.randomInteger(0, ENV_HEIGHT)

	Map.modifyColor(x,y,Stat.randomInteger(1,255),Stat.randomInteger(1,255),Stat.randomInteger(1,255)) 

end

function cleanUp()
	local color = Map.checkColor(1,1)
	say("Color at 1,1 is R:".. color.R .. " G:".. color.G .." B:"..color.B)

end
