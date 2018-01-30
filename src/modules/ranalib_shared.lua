local RanaLibShared = {}

local utility = require "ranalib_utility"
-- Value controlling whether type check should be performed on all submitted values.
local valueCheck = false

-- Submit a shared table to a central register available to all agents.
-- The table is stored in serialized form in a hashmap, using a string key as index
-- @param check, it's possible to enable check for table type, 
-- this will adversely affect performance)
-- Example: API.shareTable("agent1",{myX, myY, myID}, true)
function RanaLibShared.storeTable(key, table, check)

	local check = check or valueCheck
	local tableString

	if check==true then

                if type(table)=="table" and type(key)~="table" then
                        tableString = utility.serializeTable(table)
                        l_addSharedString(key, tableString)
                else
			l_debug("Failure to store table, table and/or key is not of correct type")			
			l_stopSimulation()
		end

	else 
		tableString = utility.serializeTable(table)
		l_addSharedString(key, tableString)
	end 

end

-- Retrieve the shared table from the central string register, using a known key.
function RanaLibShared.getTable(key)

	local tableString

	if type(key)~="table" then
		tableString = l_getSharedString(key)
	end

	return utility.deserializeTable(tableString)
end

-- Nearly the same as shareTable except the value argument will be interpreted as a
-- string, this uses the same register as shareTable(). It is much faster as no 
-- serialization is performed. 
-- If a table serilized implicitly(see example). RanaLibShared.getTable() can
-- be used to realize it as a regular lua table.
-- Example: Share.storeString("agent1", "{"..myX..","myY..","..myID.."}")
function RanaLibShared.storeString(key, string, check)

	local check = check or valueCheck

	if check==true then
		if type(string)=="string" and type(skey)~="table" then
			l_addSharedString(key, string)
		else
			l_debug("Failure to store string, string and/or key is not of correct type")
			l_stopSimulation()
		end
	else
		l_addSharedString(key, string)
	end

end

-- Retrieve a shared string from the central string register.
function RanaLibShared.getString(key)
	return l_getSharedString(key)
end

-- Submit a shared number to a central register, similar to @RanalibShared.sharetable()
-- @param check, default is false, if set to true the type of key and number
-- will be checked, and again this will adversely affect performance.
function RanaLibShared.storeNumber(key, number, check)
	
	local check = check or valueCheck

	if check==true then
		if type(number)=="number" and type(key)~="table" then
			l_addSharedNumber(key, number)
			
		else
			l_debug("Failure to store number, number and/or key is not of correct type")
			l_stopSimulation()
		end
	else
		l_addSharedNumber(key, number)
	end

end

-- Retrieves the key from the central register.
function RanaLibShared.getNumber(key)

	return l_getSharedNumber(key)
end

-- Set a boolean which enables type check on all submitted keys and values
-- enabling this will adversely affect performance.
function RanaLibShared.checkValues(boolean)

	if type(boolean) == "boolean" then
		valueCheck = boolean
	end

end

-- Submit a shared agent to a central register available to all agents.
-- The agent is stored in a pair form in a hashmap, using a string key as index
-- Example: API.shareAgent("agent1",path, num)
function RanaLibShared.storeAgent(key, path, num)

        l_addSharedAgent(key, path, num)
end

function RanaLibShared.getTcpInput(key)

        return l_getSharedTcpInput(key)
end

return RanaLibShared






































