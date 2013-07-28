#include "base.h"

void func(lua_State *L) {
	luaS_dofile(L, "test3.lua");
	LuaRef table = getGlobal(L, "table");
	std::cout << table[1] << std::endl;
	table[1] = "c";
	LuaRef e = table[1];
	std::cout << e << std::endl;
	std::cout << "###" << std::endl;
	table[1] = table[2];
}

