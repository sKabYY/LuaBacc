#include "base_wrapper.h"
#include <cstring>

int g_i = -1234;
char const* g_s = "never show this";
char const* g_des_msg = "never show this";

class C1 {
private:
	int i;
	char const* s;
public:
	C1() : i(0), s("hehe") {}
	C1(int _i, char const* _s) : i(_i), s(_s) {}
	~C1() {g_des_msg = s;}
	void f(int d) {
		g_i = i + d;
		g_s = s;
	}
	int getIAdd(int d) {
		return i + d;
	}
};

void func(LuaState& state) {
	state.module()
		.class_<C1>("C1")
			.def(constructor<>())
			.def("is", constructor<int, char const*>())
			.def("f", &C1::f)
			.def("getIAdd", &C1::getIAdd)
		.end();
	state.dostring(
			"c = C1()\n"
			"c:f(3)\n"
	);
	assert(g_i == 3);
	assert(!strcmp(g_s, "hehe"));
	state.dostring(
			"c = C1:is(42, 'lala')\n"
			"c:f(4)\n"
			"i = c:getIAdd(4)\n"
			"c:__gc()\n"
	);
	assert(g_i == 46);
	assert(!strcmp(g_s, "lala"));
	int i = state.getGlobal("i");
	assert(i == 46);
	assert(!strcmp(g_des_msg, "lala"));
}

