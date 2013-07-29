#include <iostream>

#include "../luacapi/lua.hpp"
#include "../luabacc/luabacc.h"
#include "../luabacc/dump.h"
using namespace luabacc;


int main() {
	LuaState state;
	try {
		state.dofile("test1.lua");
	} catch (std::string e) {
		std::cerr << e << std::endl;
	}
	LuaRef v = state.getGlobal("a");
	std::cout << v << std::endl;
	v = state.getGlobal("t");
	std::cout << v << std::endl;
	for (Iterator iter(v); !iter.isNil(); ++iter) {
		std::cout << iter.key() << "=" << iter.value() << std::endl;
	}
	std::cout << "t.c.x=" << v["c"]["x"] << std::endl;
	std::cout << "check stack: " << std::endl;
	std::cout << dumpLuaState(state.state()) << std::endl;
	return 0;
}

