#include "base.h"

void func(lua_State *L) {
	luaS_dofile(L, "test2.lua");
	LuaRef v = getGlobal(L, "f");
	assert(v("abc", "xyz").cast<std::string>() == "abc,xyz");
	LuaRef t = getGlobal(L, "t");
	assert(t["add"](12, 32).cast<int>() == 44);
	double d = t["mul"](12, 3.2);
	assert(flt_equals(d, 38.4));
}

