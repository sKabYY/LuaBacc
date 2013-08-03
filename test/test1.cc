#include "base.h"
#include <cstring>


void func(lua_State *L) {
	luaS_dofile(L, "test1.lua");
	LuaRef v(L);
	v = getGlobal(L, "a");
	const char *s = v;
	assert(!strcmp(s, "abc"));
	assert(v.tostring() == "abc");
	v = getGlobal(L, "b");
	bool b = v;
	assert(b);
	assert(v.tostring() == "true");
	v = getGlobal(L, "c");
	int i = v;
	assert(i == 42);
	assert(v.tostring() == "42");
	v = getGlobal(L, "d");
	double d = v;
	assert(d == 3.14159265);
	assert(v.tostring() == "3.14159265");
	v = getGlobal(L, "t");
	assert(v["a"].cast<std::string>() == "a");
	v["a"] = "abc";
	assert(v["a"].cast<std::string>() == "abc");
	v["a"] = v["b"];
	assert(v["a"].cast<std::string>() == v["b"].cast<std::string>());
	assert(v["c"]["x"].cast<int>() == 12);
	i = v["c"]["y"];
	assert(i == 21);
}

