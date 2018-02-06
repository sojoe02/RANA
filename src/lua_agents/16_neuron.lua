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
Agent = require "ranalib_agent"
Event = require "ranalib_event"
Shared = require "ranalib_shared"
Stat = require "ranalib_statistic"

local current = 10;

local g1 = Stat.randomMean(5, 36);
local g2 = Stat.randomMean(10, 120);
local g3 = Stat.randomMean(0.1, 0.3);

local E1 = Stat.randomMean(5, -12);
local E2 = Stat.randomMean(10, 115);
local E3 = Stat.randomMean(5, 10.613);

local I_ext_input = 0;
local I_ext_output = Stat.randomMean(5,8);
local V = -10;

local x1 = 0;
local x2 = 0;
local x3 = 1;

local t = 1;
local t_curr = 1;
local tn = STEP_RESOLUTION;

local Alpha1 = 0;
local Alpha2 = 0;
local Alpha3 = 0;

local Beta1 = 0;
local Beta2 = 0;
local Beta3 = 0;

local file
local file2
local groups

local last_v = 0
local boolFlag = 1;

function _InitializeAgent()
    groups = Agent.getMemberOfGroups()
    file = io.open("test_"..ID..".csv", "w")
    if ID == 7 then
        file2 = io.open("test_"..(ID+1)..".csv", "w")
    end

    print(ID)

    if ID ~= 7 then
        I_ext_input = 100;
    end
end

function sentEvent(gateOpenTime)
    if ID ~= 7 then
        Event.emit{speed=100,description="ping",targetGroup=groups,table={value=gateOpenTime, time = t}}
    end
end

function calculateModel()

end

function handleEvent(sourceX, sourceY, sourceID, eventDescription, eventTable)
    if ID == 7 then
        print("ID: "..ID.."\tSent Time: "..eventTable.time.."\t\tArrival Time: "..t.." \tOpen Time:"..eventTable.value)

        I_ext_input = I_ext_input + eventTable.value
        print("I_ext_input\t"..I_ext_input)
    end
end

function takeStep()

    Alpha1=(10-V)/(100*(math.exp((10-V)/10)-1));
    Alpha2=(25-V)/(10*(math.exp((25-V)/10)-1));
    Alpha3=0.07*math.exp(-V/20);

    Beta1=0.125*math.exp(-V/80);
    Beta2=4*math.exp(-V/18);
    Beta3=1/(math.exp((30-V)/10)+1);

    tau1=1/(Alpha1+Beta1);
    tau2=1/(Alpha2+Beta2);
    tau3=1/(Alpha3+Beta3);

    x0_1 = Alpha1 * tau1;
    x0_2 = Alpha2 * tau2;
    x0_3 = Alpha3 * tau3;

    x1 = (1-tn/tau1)*x1+tn/tau1*x0_1;
    x2 = (1-tn/tau2)*x2+tn/tau2*x0_2;
    x3 = (1-tn/tau3)*x3+tn/tau3*x0_3;

    gnmh1 = g1*math.pow(x1,4);
    gnmh2 = g2*math.pow(x2,3)*x3;
    gnmh3 = g3;

    I1 = gnmh1*(V-E1)
    I2 = gnmh2*(V-E2)
    I3 = gnmh3*(V-E3)

    V = V+tn*(I_ext_input-(I1+I2+I3))

    if t ~= 0 then
        decideToOpenGates()
    end

    if t % 1 == 0 then
        file:write(V.."\n");
        if ID == 7 then
            file2:write(I_ext_input.."\n");
        end
    end
    if t % 100 == 0 then
        --print(STEP_RESOLUTION.. ' ' ..tn.. ' ' .. t .. ' '..V);
    end

    last_v = V

    t = t+1;
end

function decideToOpenGates()

    local changeInV = V - last_v

    if (changeInV > 0 and boolFlag == 1) then
            --print(t..' 1x1 ' .. changeInV .. ' true '..boolFlag)
            boolFlag = 0
            --print(t..' 1x2 ' .. changeInV .. ' true '..boolFlag)

            sentEvent(I_ext_output)

    elseif (changeInV < 0 and boolFlag == 0) then

            --print(t..' 2x1 ' .. changeInV .. ' false '..boolFlag)
            boolFlag = 1
            --print(t..' 2x2 ' .. changeInV .. ' false '..boolFlag)

            sentEvent(-I_ext_output)
    end


end

function cleanUp()
end












