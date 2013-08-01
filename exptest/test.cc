// Compile: g++ -std=c++0x thisfile.cc

#include <iostream>
using namespace std;


void func(int i1, int i2) {
	cout << i1 << endl;
	cout << i2 << endl;
}


int counter() {
	static int i = 0;
	return ++i;
}


template <typename... T>
struct Caller;

template <>
struct Caller<> {
	template <typename... U>
	static void call(U... u) {
		func(u...);
	}
};

template <typename H, typename... T>
struct Caller<H, T...> {
	template <typename... U>
	static void call(U... u) {
		H h = counter();
		Caller<T...>::call(u..., h);
	}
};

template <typename FP>
struct CFuncWrapper;

template <typename R, typename... Ps>
struct CFuncWrapper<R (*)(Ps...)> {
	static void call() {
		Caller<Ps...>::call();
	}
};

template <typename FP>
static void deffunction(FP fp) {
	CFuncWrapper<FP>::call();
}


int main() {
	deffunction(func);
	return 0;
}

