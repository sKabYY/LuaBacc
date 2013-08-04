#include <iostream>

#include "../luacapi/lua.hpp"
#include "../luabacc/luabacc.h"
using namespace luabacc;


class C {
public:
	C() {}
	void p () {
		std::cout << "This is C::p." << std::endl;
	}
};


void func(lua_State *L) {
	module(L)
		.class_<C>("C")
			.def(constructor<>())
			.def("p", &C::p)
		.end();
	luaS_dostring(L, "c = C()\nc.p()\n");
}


int main() {
	try {
		lua_State *L = luaS_newstate();
		func(L);
		luaS_close(L);
	} catch (std::string e) {
		std::cerr << "Exception: " << e << std::endl;
	}
	return 0;
}

