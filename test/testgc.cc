#include "base_wrapper.h"

std::string g_msg = "";
void addmsg(char const* msg) {
	g_msg += msg;
	g_msg += "\n";
}

class C {
public:
	~C() {
		addmsg("__gc");
	}
};


std::string expected_msg =
"kill c\n"
"__gc\n"
"end killing\n"
"begin gc\n"
"end gc\n";


void func(LuaState& state) {
	state.module()
		.def("print", &addmsg)
		.class_<C>("C")
			.def(constructor<>())
		.end();
	state.dostring(
			"c = C()\n"
			"print('kill c')\n"
			"c = nil\n"
			"collectgarbage()\n"
			"print('end killing')\n"
	);
	C c;
	state.module()
		.def_object("cc", &c);
	state.dostring(
			"cc = nil\n"
			"print('begin gc')\n"
			"collectgarbage()\n"
			"print('end gc')\n"
	);
	assert(g_msg == expected_msg);
}

