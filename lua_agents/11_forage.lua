
local 10_forage = {}

local forage_scan_radius
local grass = {0,255,0}

local _Configure
local _Search
local _Move
local _Eat

function 10_forage.forage()

	if not Moving then

	end

end

_Configure = function(configuration)

	forage_scan_radius = configuration.scan_radius


end

_Search = function()

	table = Map.radialScanMap(forage_scan_radius)
	
	for i=1, #table do
		r = table[i].R
		g = table[i].G
		b = table[i].B
		if _CompareColor({r,g,b}, grass) then
			DestinationX = table[i].posX
			DestinationY = table[i].posY
			Moving = true
		end
	end

end

_CompareColor = function(color1, color2)
	
	if color1[1] == color2[1] and color1[2] == color2[2] and color1[3] == color2[3] then

		return true

	end

	return false

end


return 10_forage
