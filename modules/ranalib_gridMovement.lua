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

	if type(xx) = "number" and type(yy)="number" then

		return l_checkPosition(x, y)

	end

	return nil
end

-- Returns a boolean denoting whether or not there is an
-- agent at the queried position.
function ranaLibCollisionGrid.checkCollision(x, y)

	local xx = x or PositionX
	local yy = y or PositionY

	if type(xx) = "number" and type(yy)="number" then

		return l_checkCollision(x, y)

	end

	return nil
end

-- Add another position to the grid 
-- If the agent wants this to change it has to use 
-- ranaLibCollsionGrid.updatePosition.
function ranaLibCollisionGrid.addPosition(x,y,id)

	local iid = id or ID
	
	if type(x) = "number" and type(y)="number" then
		l_addPosition(x,y,iid)
	end
end

-- Updates a position with ID in the collision grid... if it exists.
function ranaLibCollisionGrid.updatePosition(oldY,oldY,newX,oldY,id)
	
	if type(newX) = "number" and type(newY)="number" and type(oldX) = "number" and type(oldY) = "number" and type(id)="number" then

		l_updatePosition(oldX,oldY,newX,newY,iid)

	end

end

return ranaLibCollisionGrid
