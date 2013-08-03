#include <iostream>

#include "../luacapi/lua.hpp"
#include "../luabacc/luabacc.h"
#include "../luabacc/dump.h"
using namespace luabacc;

void func(lua_State *L);


inline bool flt_equals(double a, double b) {
	const double ETA = 0.0001;
	return b < a + ETA && b > a - ETA;
}


inline std::string getStackEmptyMsg() {
	static std::string EMPTY_LUA_STACK_MSG = "top=0:\n";
	return EMPTY_LUA_STACK_MSG;
}


int main() {
	lua_State *L = luaS_newstate();
	try {
		func(L);
	} catch (std::string e) {
		std::cerr << e << std::endl;
	}
	assert(dumpLuaState(L) == getStackEmptyMsg()); // Check stack
	luaS_close(L);
	return 0;
}
