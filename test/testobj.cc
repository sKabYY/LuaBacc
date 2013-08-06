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
		std::cout << a << "+" << i << std::endl;
		return a + i;
	}
};


void func(LuaState& state) {
	C c(1);
	std::cout << &c << std::endl;
	state.module()
		.class_<C>("C")
			.def(constructor<int>())
			.def("add", &C::add)
		.end()
		.def_object("c", &c);
	state.dostring(
			"a = c:add(2)\n"
			"print(a)\n"
	);
	int a = state.getGlobal("a");
	std::cout << a << std::endl;
	std::cout << c.add(2) << std::endl;
	assert(a == 3);
}

