#include "base.h"

void func(lua_State *L) {
	luaS_dofile(L, "test1.lua");
	LuaRef v(L);
	v = getGlobal(L, "a");
	const char *s = v;
	std::cout << s << std::endl;
	std::cout << v.tostring() << std::endl;
	v = getGlobal(L, "b");
	bool b = v;
	std::cout << b << std::endl;
	std::cout << v.tostring() << std::endl;
	v = getGlobal(L, "c");
	int i = v;
	std::cout << i << std::endl;
	std::cout << v.tostring() << std::endl;
	v = getGlobal(L, "d");
	double d = v;
	std::cout << d << std::endl;
	std::cout << v.tostring() << std::endl;
	v = getGlobal(L, "t");
	std::cout << v.tostring() << std::endl;
	v["a"] = "abc";
	std::cout << v["a"].cast<std::string>() << std::endl;
	v["a"] = v["b"];
	std::cout << v["a"].cast<std::string>() << std::endl;
	std::cout << v["b"].cast<std::string>() << std::endl;
}

