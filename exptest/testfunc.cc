#include <iostream>
using namespace std;

#include "../luacapi/lua.hpp"
#include "../luabacc/luabacc.h"
using namespace luabacc;


#include "deffunction.h"


void luafunc(int i, double d) {
	cout << "i=" << i << endl;
	cout << "d=" << d << endl;
}


void luafunc2(int a, int b) {
	cout << "a*b=" << a * b << endl;
}


void func(lua_State* L) {
	TestModule(L).deffunction("func", &luafunc);
	luaL_dostring(L, "func(1, 2.3)");
	TestModule(L, "space").deffunction("func", &luafunc2);
	luaL_dostring(L, "space.func(2, 3)");
}


int main() {
	lua_State* L = luaS_newstate();
	func(L);
	luaS_close(L);
	return 0;
}

