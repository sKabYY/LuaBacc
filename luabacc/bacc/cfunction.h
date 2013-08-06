/**/

namespace __bacc {

	template <typename FP>
	struct CFunction;

	// ==========================================================

	template <typename FP, typename R, typename... T>
	struct RecursiveCaller;

	template <typename FP>
	struct RecursiveCaller<FP, void> {
		template <typename... U>
		static int call(lua_State* L, U... u) {
			FP const& fp = *static_cast<FP*>(lua_touserdata(L, lua_upvalueindex(1)));
			fp(u...);
			return 0;
		}
	};

	template <typename FP, typename R>
	struct RecursiveCaller<FP, R> {
		template <typename... U>
		static int call(lua_State* L, U... u) {
			FP const& fp = *static_cast<FP*>(lua_touserdata(L, lua_upvalueindex(1)));
			__bacc::LuaStack<R>::push(L, fp(u...));
			return 1;
		}
	};

	template <typename FP, typename R, typename H, typename... T>
	struct RecursiveCaller<FP, R, H, T...> {
		template <typename... U>
		static int call(lua_State* L, U... u) {
			const int n = static_cast<int>(1 + sizeof...(U));
			H h = __bacc::LuaStack<H>::get(L, n);
			return RecursiveCaller<FP, R, T...>::call(L, u..., h);
		}
	};

	template <typename R, typename... Ps>
	struct CFunction<R (*)(Ps...)> {
		typedef R (*FP)(Ps...);
		static int call(lua_State* L) {
			return RecursiveCaller<FP, R, Ps...>::call(L);
		}
	};

	// ==========================================================

	template <typename MFP, typename R, typename C, typename... T>
	struct RecursiveMemberCaller;

	template <typename MFP, typename C>
	struct RecursiveMemberCaller<MFP, void, C> {
		template <typename... U>
		static int call(lua_State* L, U... u) {
			C **const self = __bacc::LuaStack<C**>::get(L, 1);
			MFP const& mfp = *static_cast<MFP*>(lua_touserdata(L, lua_upvalueindex(1)));
			(*self->*mfp)(u...);
			return 0;
		}
	};

	template <typename MFP, typename R, typename C>
	struct RecursiveMemberCaller<MFP, R, C> {
		template <typename... U>
		static int call(lua_State* L, U... u) {
			C **const self = __bacc::LuaStack<C**>::get(L, 1);
			MFP const& mfp = *static_cast<MFP*>(lua_touserdata(L, lua_upvalueindex(1)));
			__bacc::LuaStack<R>::push(L, (*self->*mfp)(u...));
			return 1;
		}
	};

	template <typename MFP, typename R, typename C,typename H, typename... T>
	struct RecursiveMemberCaller<MFP, R, C, H, T...> {
		template <typename... U>
		static int call(lua_State* L, U... u) {
			const int n = static_cast<int>(2 + sizeof...(U));
			H h = __bacc::LuaStack<H>::get(L, n);
			return RecursiveMemberCaller<MFP, R, C, T...>::call(L, u..., h);
		}
	};

	template <typename R, typename C, typename... Ps>
	struct CFunction<R (C::*)(Ps...)> {
		typedef R(C::*MFP)(Ps...);
		static int call(lua_State* L) {
			return RecursiveMemberCaller<MFP, R, C, Ps...>::call(L);
		}
	};

	// ==========================================================

	template <typename R, typename C, typename... Ps>
	struct CFunction<R (C::*)(Ps...) const> {
		typedef R(C::*MFP)(Ps...) const;
		static int call(lua_State* L) {
			return RecursiveMemberCaller<MFP, R, C, Ps...>::call(L);
		}
	};

	// ==========================================================

	template <typename C>
	struct CDestructor {
		static int call(lua_State* L) {
			C **const self = __bacc::LuaStack<C**>::get(L, 1);
			(*self)->~C();
			return 0;
		}
	};

}


