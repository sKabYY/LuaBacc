/**/


template <typename... Ps>
struct constructor {};


namespace __bacc {

	template <typename C, typename... Ps>
	struct Constructor;

	template <typename C>
	struct Constructor<C> {
		static int call(lua_State* L) {
			void* self = lua_newuserdata(L, sizeof(C));
			new (self) C();
			lua_pushvalue(L, lua_upvalueindex(1));
			lua_setmetatable(L, -2);
			return 1;
		}
	};

}

