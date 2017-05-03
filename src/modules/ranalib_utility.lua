local RanaLibUtility = {}

-- Deserialize a string representation of a table.
-- This is usually used to deserialize event tables.
function RanaLibUtility.deserializeTable(string)

	--say(string)
	if string.len(string) < 3 then 
		return nil

	elseif string == nil then
		say("No data available")
		return nil
	end

	loadstring("stable="..string)()
	return stable
end

-- Generate a string representation of a lua table recursively.
function RanaLibUtility.serializeTable(val, name, depth)
	--skipnewlines = skipnewlines or false
	depth = depth or 0
	local tbl = string.rep("", depth)
	if name then
		if type(name)=="number" then
			namestr = "["..name.."]"
			tbl= tbl..namestr.."="
		elseif name then 
			tbl = tbl ..name.."="
			--else tbl = tbl .. "systbl="
		end	
	end
	if type(val) == "table" then
		tbl = tbl .. "{"
		local i = 1
		for k, v in pairs(val) do
			if i ~= 1 then
				tbl = tbl .. ","
			end	
			tbl = tbl .. RanaLibUtility.serializeTable(v,k, depth +1) 
			i = i + 1
		end
		tbl = tbl .. string.rep(" ", depth) ..  "}"
	elseif type(val) == "number" then
		tbl = tbl .. tostring(val) 
	elseif type(val) == "string" then
		tbl = tbl .. string.format("%q", val)
	else
		tbl = tbl .. "[datatype not serializable:".. type(val) .. "]"
	end

	return tbl
end

function RanaLibUtility.loadTable( key, filename )

    local ftables,err = loadfile( filename )
    if err then
        return _,err
    end

    local tables = ftables()

    for idx = 1,#tables do

        local tolinki = {}

        for i,v in pairs( tables[idx] ) do
            if i == "name" and v == key then

                if type( v ) == "table" then
                    tables[idx][i] = tables[v[1]]
                end

                if type( i ) == "table" and tables[i[1]] then
                    table.insert( tolinki,{ i,tables[i[1]] } )
                end

            end

        end

        for _,v in ipairs( tolinki ) do
            tables[idx][v[2]],tables[idx][v[1]] =  tables[idx][v[1]],nil
        end

    end

    for i = 1,#tables do
        if tables[i].name == key then
            return tables[i]
        end
    end

    return nil

end

-- Save table to a file
function RanaLibUtility.saveTable( tbl, filename )
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

function RanaLibUtility.tableMerge(t1, t2)

    if t2 ~= nil then

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

end

--  Used to calculate the change in variables.
function RanaLibUtility.handleParameterTable( tbl, it )
    if tbl.numVar == nil then
        return nil
    elseif tbl.numVar == 1 then
        return singleParam( tbl, it )
    elseif tbl.numVar == 2 then
        return doubleParam( tbl, it )
    elseif tbl.numVar > 2 then
        return dynamicParam( tbl, it )
    else
        print("There are no variables to set")
    end
end

function singleParam( tbl, it )
    return{
        v1 = tbl.v1_ss * math.fmod( it, ((tbl.v1_t-tbl.v1_f)/tbl.v1_ss) )
    }
end

function doubleParam( tbl, it )
    return{
        v1 = tbl.v1_ss * math.floor( it / ((tbl.v1_t-tbl.v1_f)/tbl.v1_ss) ),
        v2 = tbl.v2_ss * math.fmod( it, ((tbl.v2_t-tbl.v2_f)/tbl.v2_ss) )
    }
end

function dynamicParam( tbl, it )
    --  TODO
end

return RanaLibUtility
