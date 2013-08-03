table = setmetatable({}, {
	__index = function(t, k)
		index_msg =  '__index: ' .. tostring(k)
		return nil
	end,
	__newindex = function(t, k, v)
		newindex_msg = '__newindex: ' .. tostring(k) .. ', ' .. tostring(v)
	end,
})

--[[
a = table[1]
print(a)
table[1] = 0
a = table[1]
print(a)
--]]

