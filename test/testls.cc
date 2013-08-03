#include <iostream>

#include "../luacapi/lua.hpp"
#include "../luabacc/luabacc.h"
#include "../luabacc/dump.h"
using namespace luabacc;


inline std::string getStackEmptyMsg() {
	static std::string EMPTY_LUA_STACK_MSG = "top=0:\n";
	return EMPTY_LUA_STACK_MSG;
}


int main() {
	LuaState state;
	try {
		state.dofile("test1.lua");
	} catch (std::string e) {
		std::cerr << e << std::endl;
	}
	LuaRef v = state.getGlobal("a");
	assert(v.tostring() == "abc");
	assert(dumpLuaState(state.state()) == getStackEmptyMsg()); // Check stack
	return 0;
}

