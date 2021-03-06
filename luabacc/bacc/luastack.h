/**/

namespace __bacc {

	template <typename T> struct LuaStack;

#define LUASTACK_PROTOTYPE(T, _push, _get) \
	template <> struct LuaStack<T> { \
		static void push(lua_State* L, T value) { \
			_push; \
		} \
		static T get(lua_State* L, int index) { \
			return _get; \
		} \
	}


	LUASTACK_PROTOTYPE(int,
			lua_pushinteger(L, static_cast<lua_Integer>(value)),
			static_cast<int>(luaL_checkinteger(L, index))
	);

	LUASTACK_PROTOTYPE(float,
			lua_pushnumber(L, static_cast<lua_Number>(value)),
			static_cast<float>(luaL_checknumber(L, index))
	);

	LUASTACK_PROTOTYPE(double,
			lua_pushnumber(L, static_cast<lua_Number>(value)),
			static_cast<double>(luaL_checknumber(L, index))
	);

	LUASTACK_PROTOTYPE(bool,
			lua_pushboolean(L, static_cast<int>(value)),
			lua_toboolean(L, index) != 0
	);

	template <> struct LuaStack<std::string> {
		static void push(lua_State* L, std::string value) {
			lua_pushstring(L, value.c_str());
		}
		static std::string get(lua_State *L, int index) {
			return std::string(luaL_checkstring(L, index));
		}
	};

	template <> struct LuaStack<char const*> {
		static void push(lua_State* L, char const* value) {
			if (value != 0) {
				lua_pushstring(L, value);
			} else {
				lua_pushnil(L);
			}
		}
		static char const* get(lua_State* L, int index) {
			return lua_isnil(L, index) ? 0 : luaL_checkstring(L, index);
		}
	};

	template <typename T>
	struct LuaStack<T*> {
		static T* get(lua_State* L, int index) {
			return static_cast<T*>(lua_touserdata(L, index));
		}
	};

#undef LUASTACK_PROTOTYPE

}

