agent   = require "ranalib_agent"
shared  = require "ranalib_shared"
map     = require "ranalib_map"
stat    = require "ranalib_statistic"
core    = require "ranalib_core"

--Initialization of the agent.
function initializeAgent()
        --say("Master Agent#: " .. ID .. " has been initialized")

  --Create a shared table with all the default values.   
    shared.storeTable("defaultValues", {
        tnum = shared.getNumber(1),
        C = shared.getNumber(2),
        D = shared.getNumber(3),
        G = shared.getNumber(4)*shared.getNumber(4),
        I = shared.getNumber(5),
        M = shared.getNumber(6),
        N = shared.getNumber(7),
        P = shared.getNumber(8),
        Q = shared.getNumber(9),
        T = shared.getNumber(10),
        W = shared.getNumber(11),
        X = shared.getNumber(12),
        Y = shared.getNumber(13),
        Z = shared.getNumber(14),
        S = shared.getNumber(12)+shared.getNumber(13),
        refuel = shared.getNumber(15),
        A = shared.getNumber(16),
        E = shared.getNumber(17)
    })

  --Defined the default colors for easier visualtion. 
  shared.storeTable("defaultColors", {
    ore         = {r = 255, g = 255, b = 0},
    base        = {r = 255, g = 0, b = 0},
    explorer    = {r = 0, g = 255, b = 0},
    transporter = {r = 0, g = 0, b = 255}
  })

  --Create ore on the map, but first clear it
  ore_color = shared.getTable("defaultColors")["ore"];
  
  for x = 1, ENV_WIDTH do
      for y = 1, ENV_HEIGHT do
        map.modifyColor(x, y, {0, 0, 0});
      end
  end
  
  for i = 1, ENV_WIDTH*ENV_HEIGHT*shared.getTable("defaultValues")["D"] do

        x = stat.randomInteger(1, ENV_WIDTH)
        y = stat.randomInteger(1, ENV_HEIGHT)
        local currentColor = map.checkColor(x, y)

    map.modifyColor(x, y, {ore_color.r, ore_color.g, ore_color.b});
  end

  --Create the define bases an place randomly on the map
  for i = 1, shared.getTable("defaultValues")["N"] do
    agent.addAgent("base.lua")
  end

  --Remove the "master" agent from the map
	PositionX = -1
	PositionY = -1
  
  cycleCount = 0

  defaultValues = shared.getTable("defaultValues")
end

--Remove the master agent at the first step 
--since it is not needed any more. 
function takeStep()
  cycleCount = cycleCount + 1
  
  if cycleCount > defaultValues["T"] then
    core.stopSimulation();
  end
  
	--agent.removeAgent(ID)
end

