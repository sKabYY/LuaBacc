#include "base_wrapper.h"


int f1(int a, int b) {
	return a + b;
}

std::string g_msg;
void f2() {
	g_msg = "This is f2.";
}


void func(LuaState& state) {
	state.module()
		.def("f1", &f1)
		.namespace_("ns")
			.def("f2", &f2)
		.end();
	state.dostring("a = f1(1, 2)\nns.f2()\n");
	int i = state.getGlobal("a");
	assert(i == 3);
	assert(g_msg == "This is f2.");
}
