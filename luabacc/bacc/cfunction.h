/**/

namespace __bacc {
/*
	template <typename FP, typename R, typename T, typename... T>
	struct RecursiveMemberCaller;

	template <typename FP, typename T>
	struct RecursiveMemberCaller<FP, void, T> {
		template <typename... U>
		static int call(lua_State* L, U... u) {
			FP const& fp = *static_cast<FP*>(lua_touserdata(L, lua_upvalueindex(1)));
			fp(u...);
			return 0;
		}
	};

	template <typename FP, typename R, typename T>
	struct RecursiveMemberCaller<FP, R, T> {
		template <typename... U>
		static int call(lua_State* L, U... u) {
			FP const& fp = *static_cast<FP*>(lua_touserdata(L, lua_upvalueindex(1)));
			__bacc::LuaStack<R>::push(L, fp(u...));
			return 1;
		}
	};

	template <typename FP, typename R, typename H, typename... T>
	struct RecursiveMemberCaller<FP, R, H, T...> {
		template <typename... U>
		static int call(lua_State* L, U... u) {
			const int n = -static_cast<int>(1 + sizeof...(u));
			H h = __bacc::LuaStack<H>::get(L, n);
			return RecursiveCaller<FP, R, T...>::call(L, h, u...);
		}
	};*/

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
			//const int n = -static_cast<int>(1 + sizeof...(T));
			const int n = static_cast<int>(1 + sizeof...(U));
			H h = __bacc::LuaStack<H>::get(L, n);
			return RecursiveCaller<FP, R, T...>::call(L, u..., h);
		}
	};


	template <typename FP>
	struct CFunction;

	template <typename R, typename... Ps>
	struct CFunction<R (*)(Ps...)> {
		typedef R (*FP)(Ps...);
		static int call(lua_State* L) {
			return RecursiveCaller<FP, R, Ps...>::call(L);
		}
	};
/*
	template <typename R, typename T, typename... Ps>
	struct CFunction<R (T::*)(Ps...)> {
		typedef R(T::*MFP)(Ps...);
		static int call(lua_State* L) {
			return RecursiveMemberCaller<MFP, R, T, Ps...>::call(L);
		}
	}*/

}


