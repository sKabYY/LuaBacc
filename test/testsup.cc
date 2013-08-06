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
	virtual ~C1() {
		g_des_msg = s;
	}
	void f(int d) {
		g_i = i + d;
		g_s = s;
	}
	int getIAdd(int d) const {
		return i + d;
	}
	static double cop(int a, double b) {
		return a + b;
	}
};


char const* g_c2 = "never show this";

class C2 : public C1 {
public:
	void p2() {
		g_c2 = "p2";
	}
};


void func(LuaState& state) {
	state.module()
		.class_<C1>("C1")
			.def(constructor<>())
			.def("is", constructor<int, char const*>())
			.def("f", &C1::f)
			.def("getIAdd", &C1::getIAdd)
			.def("cop", &C1::cop)
		.end()
		.derive_<C2, C1>("C2")
			.def(constructor<>())
			.def("p2", &C2::p2)
		.end();
	state.dostring(
			"c = C2()\n"
			"c:f(3)\n"
			"c:p2()\n"
	);
	assert(g_i == 3);
	assert(!strcmp(g_s, "hehe"));
	assert(!strcmp(g_c2, "p2"));
}

