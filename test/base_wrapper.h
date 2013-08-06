#include <iostream>

#include "../luacapi/lua.hpp"
#include "../luabacc/dump.h"
#include "../luabacc/luabacc.h"
using namespace luabacc;


inline bool exp_with_msg(bool exp, std::string msg) {
	if (!exp) {
		std::cout << msg << std::endl;
	}
	return exp;
}


void func(LuaState& state);


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
		LuaState state;
		try {
			func(state);
		} catch (std::string e) {
			std::cerr << e << std::endl;
		}
		std::string dump_msg = dumpLuaState(state.state());
		// Check stack
		assert(exp_with_msg(dump_msg == getStackEmptyMsg(), dump_msg));
	} catch (std::string e) {
		std::cerr << "Exception: " << e << std::endl;
		return 1;
	}
	return 0;
}
