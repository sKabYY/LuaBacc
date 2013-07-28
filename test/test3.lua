table = setmetatable({}, {
	__index = function(t, k)
		print('__index: ' .. tostring(k))
		return nil
	end,
	__newindex = function(t, k, v)
		print('__newindex: ' .. tostring(k) .. ', ' .. tostring(v))
	end,
})

--[[
a = table[1]
print(a)
table[1] = 0
a = table[1]
print(a)
--]]

