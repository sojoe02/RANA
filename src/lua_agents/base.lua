agent   = require "ranalib_agent"
shared  = require "ranalib_shared"
map     = require "ranalib_map"
stat    = require "ranalib_statistic"
core    = require "ranalib_core"
msg     = require "lib_msg"

-- Initialization of the agent.
function initializeAgent()

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

    cycleCount = 0

    --Retrive the default parameters
    defaultValues = shared.getTable("defaultValues")
    defaultColors = shared.getTable("defaultColors")

    if defaultValues.M == 1 then
        companyID = 1
    else
        companyID = ID
    end

    agent.joinGroup(companyID)

    shared.storeNumber("companyID", companyID)
    shared.storeNumber("baseID", ID)
    shared.storeNumber("noOfOreSamples"..ID, 0)

    msg.init{
        agentType           = "base",
        baseID              = ID,
        companyID           = companyID,
        communicationScope  = defaultValues.I
    }

    --Setup the base
    agent.changeColor(defaultColors["base"])

    --Create the bases explorers and transporters
    --with same coordinates as the base.
    for i = 1, defaultValues["X"] do
        agent.addAgent("explorer.lua", PositionX, PositionY)
    end

    for i = 1, defaultValues["Y"] do
        agent.addAgent("transporter.lua", PositionX, PositionY)
    end

    --skipStateProcessing = false

    recvMsgHandlers = {
    --offload = msgOffload
    }

    log = {
        oreLevel = {}
    }
  
end

function handleEvent(sourceX, sourceY, sourceID, eventDescription, eventTable)  
    --Check is this is a message is should be able to "see" otherwise ignore it
    if msg.isValid(sourceX, sourceY) then
        if recvMsgHandlers[eventDescription] then
            recvMsgHandlers[eventDescription](sourceID, eventTable.payload)
        end
    end
end

function takeStep()
    cycleCount = cycleCount + 1

    if cycleCount > defaultValues["T"] then
        core.stopSimulation();
    end

end

function cleanUp()
  say("Base Total Ore " .. shared.getNumber("noOfOreSamples"..ID))
  wrt = (shared.getNumber("noOfOreSamples"..ID)..','..shared.getNumber(1)..','..shared.getNumber(2)..','..shared.getNumber(3)..','..shared.getNumber(4)..','..shared.getNumber(5)..','..shared.getNumber(6)..','..shared.getNumber(7)..','..shared.getNumber(8)..','..shared.getNumber(9)..','..shared.getNumber(10)..','..shared.getNumber(11)..','..shared.getNumber(12)..','..shared.getNumber(13)..','..shared.getNumber(14)..','..shared.getNumber(15)..','..shared.getNumber(16)..','..shared.getNumber(17))

  print("\t"..wrt)
  print(" ")

  file = io.open("base_"..ID..".csv", "a")
  file:write(wrt.."\n")

  file:close()
end
