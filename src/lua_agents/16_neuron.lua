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

current = 10;

g1 = 36;
g2 = 120;
g3 = 0.3;

E1 = -12;
E2 = 115;
E3 = 10.613;

I_ext = 0;
V = -10;

x1 = 0;
x2 = 0;
x3 = 1;

t = 1;
t_curr = 1;
tn = STEP_RESOLUTION;

Alpha1 = 0;
Alpha2 = 0;
Alpha3 = 0;

Beta1 = 0;
Beta2 = 0;
Beta3 = 0;

file = io.open("test.csv", "w")

function _InitializeAgent()

    PositionY = 50
    PositionX = 5

    say("Agent #: " .. ID .. " has been initialized")
end

function handleEvent(sourceX, sourceY, sourceID, eventDescription, eventTable)

    I_ext = eventTable.msg;
    print("16_Neuron: "..I_ext.." sent time: "..eventTable.time.." arrival time: " .. t);

end

function takeStep()

    I_ext = 100;

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

    V = V+tn*(I_ext-(I1+I2+I3))

    if t % 10 == 0 then
        file:write(V.."\n");
    end
    if t % 100 == 0 then
        print(STEP_RESOLUTION.. ' ' ..tn.. ' ' .. t .. ' '..V);
    end

    t = t+1;
end

function cleanUp()
end
