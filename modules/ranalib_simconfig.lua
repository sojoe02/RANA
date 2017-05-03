--[[
Klargør simulation.
Tjek antal total skal køres i simconfig.
Skriv table med parameter til fil.
Indlæs parameter fra agent.
Kør simulation.


Question for Søren:
Hvad tager længst tid: At skriv til filerne, og læse derfra hvergang agenterne oprettes
eller at lave udregningen for parameter sætningen inde i agenten hver gang (n-for loops)
]]--


--  Variables used to set the simulation
simname = "_simconfig.data"
parname = "_parameters.data"
currentIteration = 0
simulationIterations = 1000

--  Import Rana lua libraries
--Para    = require "ranalib_variable"    --Used for setting the parameters each iteration.
Shared  = require "ranalib_shared"


--  Master table to be set with parameters
configTbl =
{
    simIteration=1,
    numAgents=2,
    {
        name="greenfield",
        v1_f = 0.0,
        v1_t = 1.0,
        v1_ss = 0.001,
        numVar=1
    },
    {
        name="female",
        v1_f=0.0,
        v1_t=1.0,
        v1_ss = 0.001,
        numVar=1
    },
    {
        name="freerunning",
        v1_f=0.0,
        v1_t=1.0,
        v1_ss = 0.001,
        numVar=1
    }
}

function _loadNumberIterations()

    if simulationIterations == nil then
        return 1
    end

    saveTable(configTbl,simname)
    _saveNewAgentParametersToFile()

    return simulationIterations

end

function _saveNewAgentParametersToFile()

    local paramTable = configTbl

    for key, value in pairs(configTbl) do
        if(type(value) == "table") then
            tbl = handleParameterTable(value)

            tableMerge(value, tbl)
        end
    end

    saveTable( paramTable, parname )

    currentIteration = currentIteration + 1
    configTbl.simIteration = configTbl.simIteration + 1

    print("Simulation #: "..currentIteration.." of "..simulationIterations)

end

function tableMerge(t1, t2)

    for k,v in pairs(t2) do

        if type(v) == "table" then
            if type(t1[k] or false) == "table" then
                tableMerge(t1[k] or {}, t2[k] or {})
            else
                t1[k] = v
            end
        else
            t1[k] = v
        end

    end
    return t1

end

function handleParameterTable( tbl )
    if tbl.numVar == 1 then
        return singleParam( tbl )
    elseif tbl.numVar == 2 then
        return doubleParam( tbl )
    elseif tbl.numVar > 2 then
        return dynamicParam( tbl )
    else
        print("There are no variables to set")
    end
end

function singleParam( tbl )
    return{
        v1 = tbl.v1_ss * math.fmod( currentIteration, ((tbl.v1_t-tbl.v1_f)/tbl.v1_ss) )
    }
end

function doubleParam( tbl )
    return{
        v1 = tbl.v1_ss * math.floor( currentIteration / ((tbl.v1_t-tbl.v1_f)/tbl.v1_ss) ),
        v2 = tbl.v2_ss * math.fmod( currentIteration, ((tbl.v2_t-tbl.v2_f)/tbl.v2_ss) )
    }
end

function dynamicParam( tbl )
    --  TODO
end

function saveTable( tbl, filename )
    local charS,charE = "   ","\n"
    local file,err = io.open( filename, "wb" )

    if err then
        return err
    end

    -- initiate variables for save procedure
    local tables,lookup = { tbl },{ [tbl] = 1 }
    file:write( "return {"..charE )

    for idx,t in ipairs( tables ) do
        file:write( "-- Table: {"..idx.."}"..charE )
        file:write( "{"..charE )
        local thandled = {}

        for i,v in ipairs( t ) do
            thandled[i] = true
            local stype = type( v )
            if stype == "table" then
                if not lookup[v] then
                    table.insert( tables, v )
                    lookup[v] = #tables
                end
                file:write( charS.."{"..lookup[v].."},"..charE )
            elseif stype == "string" then
                file:write(  charS..exportstring( v )..","..charE )
            elseif stype == "number" then
                file:write(  charS..tostring( v )..","..charE )
            end
        end

        for i,v in pairs( t ) do
            -- escape handled values
            if (not thandled[i]) then
                local str = ""
                local stype = type( i )
                -- handle index
                if stype == "table" then
                    if not lookup[i] then
                        table.insert( tables,i )
                        lookup[i] = #tables
                    end
                    str = charS.."[{"..lookup[i].."}]="
                elseif stype == "string" then
                    str = charS.."["..exportstring( i ).."]="
                elseif stype == "number" then
                    str = charS.."["..tostring( i ).."]="
                end

                if str ~= "" then
                    stype = type( v )
                    -- handle value
                    if stype == "table" then
                        if not lookup[v] then
                            table.insert( tables,v )
                            lookup[v] = #tables
                        end
                        file:write( str.."{"..lookup[v].."},"..charE )
                    elseif stype == "string" then
                        file:write( str..exportstring( v )..","..charE )
                    elseif stype == "number" then
                        file:write( str..tostring( v )..","..charE )
                    end
                end
            end
        end
        file:write( "},"..charE )
    end
    file:write( "}" )
    file:close()
end

function exportstring( s )
    return string.format("%q", s)
end







