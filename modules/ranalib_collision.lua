local ranaLibCollisionGrid = {}

-- Initializes the movement grid, if one exists all data will be deletet and 
-- a new dataset with agent positions will be generated with the provided scale.
-- @Param scale of the grids precision level in meters.
function ranaLibCollisionGrid.reinitializeGrid(scale)

	local scale = scale or 1

	if type(scale)=="number" then
		l_initializeGrid(1/scale)
	end

end

-- Returns a Table with a list of agents at the queried
-- positions.
function ranaLibCollisionGrid.checkPosition(x, y)

	local xx = x or PositionX
	local yy = y or PositionY

	if type(xx) == "number" and type(yy) == "number" then

		return l_checkPosition(x, y)

	end

	return nil
end

-- Returns a boolean denoting whether or not there is an
-- agent at the queried position.
function ranaLibCollisionGrid.checkCollision(x, y)

	local xx = x or PositionX
	local yy = y or PositionY

	if type(xx) == "number" and type(yy) == "number" then

		return l_checkCollision(x, y)

	end

	return nil
end

-- Add another position to the grid 
-- If the agent wants this to change it has to use 
-- ranaLibCollsionGrid.updatePosition.
function ranaLibCollisionGrid.addPosition(x,y,id)

	local iid = id or ID
	
	if type(x) == "number" and type(y) == "number" then
		l_addPosition(x,y,iid)
	end
end

-- Updates a position with ID in the collision grid... if it exists.
--fuinction ranaLibCollisionGrid.updatePosition(oldY,oldY,newX,newY,id)
	
--	if type(newX) == "number" and type(newY) == "number" and type(oldX) == "number" and type(oldY) == "number" and type(id) == "number" then
--
--		l_updatePosition(oldX,oldY,newX,newY,iid)

--	end

--end

--
function ranaLibCollisionGrid.updatePosition(newX, newY)

	l_updatePosition(PositionX, PositionY, newX, newY, ID)

end

-- Performs a radial scan with a given radius. It utilizes Ranas api for 
-- generating masks that are saved on each different radius so the calculations only
-- has to be done once.
-- This function only returns agents other than the scanning agents
-- @Param radius of the scan.
-- @Return nil if there are no collisions within the radius or a nested table 
-- with collision information.
--
-- E.g. {[1]={posX=161,posY=146,id=52 },[2]={posX=174,posY=147,id=21 }}
function ranaLibCollisionGrid.radialCollisionScan(radius)

	local table
	
	if type(radius) == "number" and radius > 0 then
		
		table = l_radialCollisionScan(ID, radius, PositionX, PositionY)

	end
	
	if table[1] == nil then return nil end

	return table	

end

return ranaLibCollisionGrid
