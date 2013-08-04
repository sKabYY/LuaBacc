/**/


template <typename... Ps>
struct constructor {};


namespace __bacc {

	/*
	 * NewUserData::call will construct a instance of class C
	 * and push it as userdata on the top of the stack.
	 */
	template <typename C, typename... T>
	struct NewUserData;

	template <typename C>
	struct NewUserData<C> {
		template <typename... U>
		static void call(lua_State* L, U... u) {
			void* self = lua_newuserdata(L, sizeof(C));
			new (self) C(u...);
		}
	};

	template <typename C, typename H, typename... T>
	struct NewUserData<C, H, T...> {
		template <typename... U>
		static void call(lua_State* L, U... u) {
			const int n = static_cast<int>(2 + sizeof...(U));
			H h = __bacc::LuaStack<H>::get(L, n);
			NewUserData<C, T...>::call(L, u..., h);
		}
	};

	template <typename C, typename... Ps>
	struct Constructor {
		static int call(lua_State* L) {
			NewUserData<C, Ps...>::call(L);
			lua_pushvalue(L, 1);
			lua_setmetatable(L, -2);
			return 1;
		}
	};

}

