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
-- ------------------------------------
-- IMMUTABLES.
-- ------------------------------------
-- ID -- id of the agent.
-- STEP_RESOLUTION 	-- resolution of steps, in the simulation core.
-- EVENT_RESOLUTION	-- resolution of event distribution.
-- ENV_WIDTH -- Width of the environment in meters.
-- ENV_HEIGHT -- Height of the environment in meters.
-- ------------------------------------
-- VARIABLES.
-- ------------------------------------
-- PositionX	 	-- Agents position in the X plane.
-- PositionY	 	-- Agents position in the Y plane.
-- DestinationX 	-- Agents destination in the X plane. 
-- DestinationY 	-- Agents destination in the Y plane.
-- StepMultiple 	-- Amount of steps to skip.
-- Speed 		-- Movement speed of the agent in meters pr. second.
-- Moving 		-- Denotes wether this agent is moving (default = false).
-- GridMove 		-- Is collision detection active (default = false).
-- ------------------------------------

-- Import valid Rana lua libraries.
Agent = require "ranalib_agent"
Event = require "ranalib_event"
Shared = require "ranalib_shared"
Stat = require "ranalib_statistic"
Move = require "ranalib_movement"

n = 1
t = 1

-- Init of the lua frog, function called upon initilization of the LUA auton.
function _InitializeAgent()
        say("Agent #: " .. ID .. " has been initialized")

	if ID == 1 then
		PositionX = ENV_WIDTH/2
		PositionY = ENV_HEIGHT/2
	end
        DestinationX = 1
        DestinationY = 1
        Moving = true
        Speed = 1

        print("My ID, and Group numbers:")
        groups = Agent.getMemberOfGroups(ID)
end

function HandleEvent(event)

	fibonacci_tail(2000)

	if event.description == "ping" then
                        say("Agent: "..ID .." received a ping from: "..event.ID .." at ".. t .. " , saying: "..event.table.msg)
                        Event.emit{speed=343,targetID=event.ID, description="pong"}

	elseif event.description == "pong" then
                        --say("Agent: "..ID.." received a pong from agent: ".. event.ID)
	end

end

function takeStep()

        --[[
        if groups ~= nil then
            print("asdf")
            if type(groups) == 'number' then
                print("asdf")
                --print(ID.." "..groups)
            elseif type(groups) == 'table' then
                print("asdf")
                for value = 1, #groups do
                    print("asdf")
                    print(ID.. " " .. value .. " " .. groups[value])
                end
            end
        end
        ]]--


        if n%1000 == 0 then
            --l_debug(PositionX.." "..PositionY)
        end
        n = n + 1

	if Stat.randomInteger(1,1/STEP_RESOLUTION) <= 1 then
                say("Agent:"..ID.." is emiting ping")
                --Event.emit{speed=343,description="ping",targetGroup=groups,table={msg="I am agent "..ID}}
                Event.emit{speed=0,description="ping",targetGroup=groups,table={msg="Sent at "..t}}
	end

        t = t + 1

end

function cleanUp()
        --l_debug("Agent #: " .. ID .. " is done\n")

end

function fibonacci(n)
    if n<3 then
        return 1
    else
        return fibonacci(n-1) + fibonacci(n-2)
    end
end

function fibonacci_tail(n)
    local function f(a, b, n)
        if n < 3 then
            return b
        else
            return f(b, a+b, n-1)
        end
    end
 
    return f(1,1,n)
end

function luaTesat()
    print("hello")
end
