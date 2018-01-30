----begin_license--
--
--Copyright 	2013 - 2016 	Søren Vissing Jørgensen.
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

--The following global values are set via the simulation core:

-- Import valid Rana lua libraries.
Shared = require "ranalib_shared"

local t = 0
local name = ''

function _InitializeAgent()
    name = "t1"
    print(name)
end

function HandleEvent(event)
end

function takeStep()
    local tcpInput = Shared.getTcpInput(name)
    print(name .. " " ..#tcpInput)
    for value = 1, #tcpInput do

        print(tcpInput[value])

    end

    t = t+1
end

function cleanUp()
end
