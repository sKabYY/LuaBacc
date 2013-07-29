/**/

template <typename T> struct LuaStack;

#define LUASTACK_PROTOTYPE(T, _push, _get) \
	template <> struct LuaStack<T> { \
		static inline void push(lua_State *L, T value) { \
			_push; \
		} \
		static inline T get(lua_State *L, T index) { \
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
	static inline void push(lua_State *L, std::string value) {
		lua_pushstring(L, value.c_str());
	}
	static inline std::string get(lua_State *L, int index) {
		return std::string(luaL_checkstring(L, index));
	}
};

template <> struct LuaStack<const char*> {
	static inline void push(lua_State *L, const char *value) {
		if (value != 0) {
			lua_pushstring(L, value);
		} else {
			lua_pushnil(L);
		}
	}
	static inline const char* get(lua_State *L, int index) {
		return lua_isnil(L, index) ? 0 : luaL_checkstring(L, index);
	}
};

