// Compile: g++ -std=c++0x thisfile.cc

#include <iostream>
using namespace std;


int counter() {
	static int i = 0;
	return ++i;
}


template <typename R>
struct Invoke {
	template <typename... Ps, typename H, typename... Args>
	static R call(R (*fp)(Ps..., H, Args...), Args... as) {
		int c = counter();
		const int N = sizeof...(Ps);
		if (N == 0) {
			return fp(c, as...);
		} else {
			return Invoke<R>::call<Ps..., H, Args...>(fp, c, as...);
		}
	}
};


template <typename FP, typename D=FP, FP fp=NULL>
struct FuncProxy {};


template <typename R, typename... Ps, typename FP, FP fp>
struct FuncProxy <R (*)(Ps...), FP, fp> {
	static R call() {
		const int N = sizeof...(Ps);
		return Invoke<R>::call<Ps>(fp);
	}
};


void func(int i1, int i2) {
	cout << i1 << endl;
	cout << i2 << endl;
}


int main() {
	FuncProxy<void (*)(int, int), void (*)(int, int), func>::call();
	return 0;
}

