#include "base.h"

void func(lua_State *L) {
	luaS_dofile(L, "test3.lua");
	LuaRef table = getGlobal(L, "table");
	assert(table[1].tostring() == "nil");
	table[1] = "c";
	assert(getGlobal(L, "newindex_msg").tostring() == "__newindex: 1, c");
	LuaRef e = table[1];
	assert(getGlobal(L, "index_msg").tostring() == "__index: 1");
	assert(e.tostring() == "nil");
	table[1] = table[2];
	assert(getGlobal(L, "index_msg").tostring() == "__index: 2");
	assert(getGlobal(L, "newindex_msg").tostring() == "__newindex: 1, nil");
}

