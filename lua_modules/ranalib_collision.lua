local RanaLibCollision = {}

-- Update the agents position in the central collision table.
-- If the agent is currently not in the table it will be added automatically.
function RanaLibCollision.updatePosition(newX, newY)
	
	l_updatePosition(myX, myY, newPosX, newPosY, ID)
end

-- Adds the agents position to the contral collision table.
-- Allows the agent to occupy more than one x,y at a time,
-- for collision check puposes only.
function RanaLibCollision.addPosition()

	l_addPosition(myX, myY, ID)
end

-- Perform a check on a given position.
-- Returns table containing the IDs present at that location.
-- Example(getting table and printing it's contents): 
-- table = API.checkPosition(50,50)
-- for i=#table do l_print(positionTable[i]) end
function RanaLibCollision.checkPosition(x, y)
	
	return l_checkPosition(x, y)
end

-- return true or false whether there is a collision at x,y
function RanaLibCollision.checkCollision(x, y)

	return l_checkCollision(x,y)
end

return RanaLibCollision
