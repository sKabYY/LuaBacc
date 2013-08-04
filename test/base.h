#include <iostream>

#include "../luacapi/lua.hpp"
#include "../luabacc/luabacc.h"
#include "../luabacc/dump.h"
using namespace luabacc;


inline bool exp_with_msg(bool exp, std::string msg) {
	if (!exp) {
		std::cout << msg << std::endl;
	}
	return exp;
}


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
	try {
		lua_State *L = luaS_newstate();
		try {
			func(L);
		} catch (std::string e) {
			std::cerr << e << std::endl;
		}
		std::string dump_msg = dumpLuaState(L);
		// Check stack
		assert(exp_with_msg(dump_msg == getStackEmptyMsg(), dump_msg));
		luaS_close(L);
	} catch (std::string e) {
		std::cerr << "Exception: " << e << std::endl;
	}
	return 0;
}
