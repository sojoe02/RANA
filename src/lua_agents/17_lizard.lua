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
Event = require "ranalib_event"
Shared = require "ranalib_shared"
Stat = require "ranalib_statistic"

sample_rate = 50000;
target_freq = 2300;
ear_separation = 13*1500/target_freq;
sf = ear_separation/13;

sound_freq = 2000;
omega = 2*math.pi*sound_freq;
c = omega*13/340000;

n_samples = 500;
n_op_samples = 50;

function _InitializeAgent()
        say("Agent #: " .. ID .. " has been initialized")

        PositionY = -100
        PositionX = -100

        x = 300
        y = 300

        dist = math.sqrt(math.pow(y-PositionY,2) + math.pow(x-PositionX,2));
        spkr_angle = math.atan2(y-PositionY,x-PositionX)

        --  Sense the sound direction with the ear model
        phase_diff = c*math.sin(0-spkr_angle);
        sinewave_L = math.sin(omega*t - 0);
        sinewave_R = math.sin(omega*t - phase_diff);

        print(dist .. " " .. spkr_angle .. " " .. phase_diff)

end

function handleEvent(sourceX, sourceY, sourceID, eventDescription, eventTable)

        local event_table = {x=sourceX, y=sourceY}

        Event.emit{targetID=sourceID, speed=343, description="echo", table=event_table}

end

function takeStep()
end

function cleanUp()
end
