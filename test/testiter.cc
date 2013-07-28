#include "base.h"

void func(lua_State *L) {
	luaS_dofile(L, "iter.lua");
	LuaRef t = getGlobal(L, "t");
	std::cout << t << std::endl;
	int len = t.length();
	std::cout << "integer iter" << std::endl;
	for (int i = 1; i <= len; ++i) {
		std::cout << "  " << i << ": " << t[i] << std::endl;
	}
	std::cout << "key-value iter" << std::endl;
	for (Iterator iter(t); !iter.isNil(); ++iter) {
		std::cout << "  " << iter.key() << ": " << iter.value() << std::endl;
	}
}

