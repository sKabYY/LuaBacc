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

