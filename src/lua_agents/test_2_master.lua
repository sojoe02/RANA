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
Event = require "ranalib_event"

local t = 0
local name = ''

local newValue;
local previousValue = nil;

function _InitializeAgent()

    PositionY = 50
    PositionX = 0

    name = "t1"
    print(name)

end

function HandleEvent(event)
end

function takeStep()


    local tcpInput = Shared.getTcpInput(name)

    if (tonumber(tcpInput[1]) ~= nil) then

        --print("VALUE FOR AGENT: "..name.." IS: "..tcpInput[1])

    end

end

function tcpInputAgentEvent(value)
    Event.emit{speed=100,description="ping",table={msg=tonumber(value),time=t}}
end

function cleanUp()
end
