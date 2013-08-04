#include "base_wrapper.h"
#include <cstring>

int g_i;
char const* g_s;

class C1 {
private:
	int i;
	char const* s;
public:
	C1() : i(0), s("hehe") {}
	C1(int _i, char const* _s) : i(_i), s(_s) {}
	void f(int d) {
		g_i = i + d;
		g_s = s;
	}
};

void func(LuaState& state) {
	state.module()
		.class_<C1>("C1")
			.def(constructor<>())
			.def("is", constructor<int, char const*>())
			.def("f", &C1::f)
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
	);
	assert(g_i == 46);
	assert(!strcmp(g_s, "lala"));
}

