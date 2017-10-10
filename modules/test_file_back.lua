--  Import tables, agents, sim, and param.
--require ("test_file_input")

--  Set local tables used
local simParam = {}
local paramOrderedKeys = {}
local agentOrderedKeys = {}

local _inputFilePath

local function paramForLoop(table, numInTable, maxNumInTable)

    --  Base case
    if numInTable > maxNumInTable then

        for index, value in ipairs(simParam) do
            io.write(value, ' ')
        end
        io.write('\n')

        -- Do co-rutine, and run simulation with parameters.

        return
    end

    --  Nested case
    local k, v = paramOrderedKeys[numInTable], param[ paramOrderedKeys[numInTable] ]
    if #v == 2 then
        for p = v[1], v[2] do
            simParam[numInTable] = p
            paramForLoop(table, numInTable+1, maxNumInTable)
        end
    elseif #v == 3 then
        if v[3] > v[2] then
            v[2], v[3] = v[3], v[2]
        end

        for p = v[1], v[2], v[3] do
            simParam[numInTable] = p
            paramForLoop(table, numInTable+1, maxNumInTable)
        end
    else
        for p = 1, #v do
            simParam[numInTable] = v[p]
            paramForLoop(table, numInTable+1, maxNumInTable)
        end
    end

end

local function paramMain()

    for value in pairs(param) do
        table.insert(paramOrderedKeys, value)
        print(paramOrderedKeys, value, #paramOrderedKeys)
    end

    paramForLoop(param, 1, #paramOrderedKeys)

end

local function agentMain()

    --  Go over all agents in file, and number of agents
    for key, value in ipairs(agents) do
        --print(key, value, type(value))

        --  Check if it is a file path
        if type(value) == "string" then
            io.write(value, " ")

            --  Check if there is a number of agents to initiate
            if type( select(2, next( agents, key) ) ) == "number" then
                io.write( select(2, next( agents, key)) )
            else    --  Instantiate 1
                io.write( 1 )
            end

            io.write("\n")
        end

    end

end

function _getSimulationFile(inputFilePath)
    _inputFilePath = inputFilePath
    print(_inputFilePath)
    require(_inputFilePath)
end

function _testFunc()
    agentMain()
    paramMain()
end























