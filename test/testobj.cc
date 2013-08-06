#include "base_wrapper.h"


class C {
private:
	int i;
public:
	C(int _i) : i(_i) {
	}
	~C() {
	}
	int add(int a) {
		return a + i;
	}
};


void func(LuaState& state) {
	C c(1);
	state.module()
		.class_<C>("C")
			.def(constructor<int>())
			.def("add", &C::add)
		.end()
		.def_object("c", &c);
	state.dostring(
			"a = c:add(2)\n"
	);
	int a = state.getGlobal("a");
	assert(a == 3);
}

