#pragma once


struct GetParams {
	GetParam(lua_State *L) {

	}
};


template <typename FP, typename D=FP, FP fp=NULL>
struct FuncTraits {};


template <typename R, typename... Ps, typename FP, FP fp>
struct FuncTraits<R (fp*)(Ps...), FP, fp> {
	static R call(lua_State *L) {
		return fp(ps...);
	}
};


template <typename FP>
void deffunction(char const* name, FP fp) {

}

