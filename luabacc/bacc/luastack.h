/**/

template <typename T> struct LuaStack;

#define LUASTACK_PROTOTYPE(T, _push, luaT, _check) \
	template <> struct LuaStack<T> { \
		static inline void push(lua_State *L, T value) { \
			_push(L, static_cast<luaT>(value)); \
		} \
		static inline T get(lua_State *L, T index) { \
			return static_cast<T>(_check(L, index)); \
		} \
	}


LUASTACK_PROTOTYPE(int, lua_pushinteger, lua_Integer, luaL_checkinteger);
LUASTACK_PROTOTYPE(float, lua_pushnumber, lua_Number, luaL_checknumber);
LUASTACK_PROTOTYPE(double, lua_pushnumber, lua_Number, luaL_checknumber);
LUASTACK_PROTOTYPE(bool, lua_pushboolean, int, lua_toboolean);


template <> struct LuaStack<const char*> {
	static inline void push(lua_State *L, const char *str) {
		if (str != 0) {
			lua_pushstring(L, str);
		} else {
			lua_pushnil(L);
		}
	}
	static inline const char* get(lua_State *L, int index) {
		return lua_isnil(L, index) ? 0 : luaL_checkstring(L, index);
	}
};


template <> struct LuaStack<std::string> {
	static inline void push(lua_State *L, std::string str) {
		lua_pushstring(L, str.c_str());
	}
	static inline std::string get(lua_State *L, int index) {
		return std::string(luaL_checkstring(L, index));
	}
};

