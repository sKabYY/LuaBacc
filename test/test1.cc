#include <iostream>

#include "../luacapi/lua.hpp"
#include "../luabacc/luabacc.h"
#include "../luabacc/dump.h"
using namespace luabacc;

void func(lua_State *L);


int main() {
	lua_State *L = luaS_newstate();
	try {
		func(L);
	} catch (std::string e) {
		std::cerr << e << std::endl;
	}
	std::cout << "check stack: " << std::endl;
	std::cout << dumpLuaState(L) << std::endl;
	luaS_close(L);
	return 0;
}


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
	std::cout << LuaRef(v["a"]).cast<std::string>() << std::endl;
	v["a"] = "zxc";
	std::cout << LuaRef(v["a"]).cast<std::string>() << std::endl;
}

