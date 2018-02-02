local ranaLibAgent = {}

local currentColor = {r=255,g=255,b=255,alpha=255}

-- Add an agent to the simulation. This agent will only receive
-- events emitted after it has been added.
-- For simulation consistency it is recommended to use this mostly during takeStep,
-- rather than handleEvent.
-- @Param filename the path of the agent relative to the agent defined in Rana upon simulation
-- start.
-- @Return the ID of the added agent, or -1 if the filename is not of type string,
-- if the filename is not present, the new agent will be in 'disabled' mode.
-- Example: addAgent([[02_oscillator.lua]], 50, 50)
function ranaLibAgent.addAgent(filename, posX, posY)
        local mapWidth, mapHeight = l_getEnvironmentSize()
        local path = l_getAgentPath()

        local posX = posX or l_getRandomInteger(1,mapWidth)
        local posY = posY or l_getRandomInteger(1,mapHeight)

        if type(filename) == "string" then
                local ID = l_addAgent(posX, posY, 0, path, filename)
                return ID
        else
                return -1
        end

end

-- Remove an agent from the simulation, using it's ID.
-- The agent will be taken out of the simulation flow in atomic fashion.
-- @Return boolean depending on whether it was successfull in removing the
-- agent or not.
function ranaLibAgent.removeAgent(ID)

        local success

        if type(ID) == "number" then
                success = l_removeAgent(ID)
        else
                success = false
        end

        return success
end

-- Join a group, Events can, upon emision be targeted at a specific
-- group. An agent can belong to 0 or more groups. Groups will be
-- removed and added on the fly as needed.
function ranaLibAgent.joinGroup(groupID)

        if type(groupID) == "number" then
                l_addGroup(groupID,ID)
        end
end

-- Leave a group. Removes the agent from a certain group.
-- @Return true of false depending on whether removal was successfull or not,
-- this will only happen if the agent is not already part of the group or groupID
-- is not of type number.
function ranaLibAgent.leaveGroup(groupID)

        local success

        if type(groupID) == "number" then
                success = l_removeGroup(ID, groupID)
        else
                success = false
        end

        return success
end

-- Sets a new step multiple of the agent
-- Setting this value allows the user to control how
-- often the agents will takeStep() en relation to default.
-- It will also correct the stepPrecision value.
-- Default is 1 and a value of 0 will take the agent out of
-- the Step flow completely.
-- Example:
-- if the stepPrecision is at 1e-3[s] and this agent only needs
-- a 1e-2 precision level the user can set the multiple at 10.
-- E.g. setStepMultiple(10)
function ranaLibAgent.setStepMultiple(multiple)

        local success

        if type(multiple) == "number" and multiple >= 0 then

                l_setMacroFactorMultiple(ID, multiple)
                StepPrecision = multiple * stepPrecision/stepMultiple
                StepMultiple = multiple
                success = true
        else
                success = false
        end

        return success
end

-- changes the color of an agent, returns true if the color
-- values are valid.
function ranaLibAgent.changeColor(options)

        ColorRed = options.r or 0
        ColorGreen = options.g or 0
        ColorBlue = options.b or 0
        ColorAlpha = options.alpha or 255

        --if r ~= currentColor.r or g ~= currentColor.g or b ~= currentColor.b or alpha ~= currentColor.alpha then
                --currentColor.r = r
                --currentColor.g = g
                --currentColor.b = b
                --currentColor.alpha = alpha



                --return l_changeAgentColor(id,r,g,b,alpha)

        --else
                --return true
        --end

end

return ranaLibAgent
