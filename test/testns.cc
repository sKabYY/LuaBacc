#include "base.h"


std::string g_df_msg;
int df(int a, const char* s) {
	g_df_msg = s;
	return a;
}


int f1(int a, int b) {
	return a + b;
}

std::string g_msg;
void f2() {
	g_msg = "This is f2.";
}


void func(lua_State *L) {
	module(L)
		.def("f1", &f1)
		.namespace_("ns")
			.def("f2", &f2)
			.namespace_("nss")
				.def("df", &df)
			.end()
		.end();
	luaS_dostring(L, "dfa = ns.nss.df(11, 'qwert')\na = f1(1, 2)\nns.f2()\n");
	int i = getGlobal(L, "a");
	assert(i == 3);
	assert(g_msg == "This is f2.");
	i = getGlobal(L, "dfa");
	assert(i == 11);
	assert(g_df_msg == "qwert");
}

