#include <iostream>

#include "../luacapi/lua.hpp"
#include "../luabacc/luabacc.h"
using namespace luabacc;


class C {
private:
	int i;
	const char *s;
public:
	C() : i(0), s("hehe") {}
	C(int _i, const char* _s) : i(_i), s(_s) {}
	void p () {
		std::cout << "This is C::p." << std::endl;
		std::cout << "i=" << i << std::endl;
		std::cout << "s=" << s << std::endl;
	}
};


void func(lua_State *L) {
	module(L)
		.class_<C>("C")
			.def(constructor<>())
			.def("w2", constructor<int, const char*>())
			.def("p", &C::p)
		.end();
	luaS_dostring(L, "c = C()\nc:p()\nc = C:w2(12, 'lala')\nc:p()\n");
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

