#include "base.h"

void func(lua_State *L) {
	luaS_dofile(L, "test2.lua");
	LuaRef v = getGlobal(L, "f");
	std::cout << v << std::endl;
	std::cout << v("abc", "xyz") << std::endl;
	LuaRef t = getGlobal(L, "t");
	std::cout << t << std::endl;
	std::cout << t["add"](12, 32) << std::endl;
	std::cout << t["mul"](12, 3.2) << std::endl;
}

