/**/


inline bool equalstates(lua_State *L1, lua_State *L2) {
	return lua_topointer(L1, LUA_REGISTRYINDEX) ==
			lua_topointer(L2, LUA_REGISTRYINDEX);
}


inline lua_State* luaS_newstate() {
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	return L;
}


inline void luaS_close(lua_State *L) {
	lua_close(L);
}


inline void luaS_pcall(lua_State *L, int nargs=0, int nresults=0) {
	if (lua_pcall(L, nargs, nresults, 0) != LUA_OK) {
		std::string errstr = lua_tostring(L, -1);
		lua_pop(L, 1);
		throw errstr;
	}
}


inline void luaS_dofile(lua_State *L, std::string path) {
	if (luaL_dofile(L, path.c_str())) {
		std::string errstr = lua_tostring(L, -1);
		lua_pop(L, 1);
		throw errstr;
	}
}


inline void luaS_dostring(lua_State *L, std::string content) {
	if (luaL_dostring(L, content.c_str())) {
		std::string errstr = lua_tostring(L, -1);
		lua_pop(L, 1);
		throw errstr;
	}
}


/*
 * Push t[key] onto the stack, where t is the value at the top of the stack.
 * Bypassing metamethods.
 */
inline void luaS_rawget(lua_State *L, const char *key) {
	lua_pushstring(L, key);
	lua_rawget(L, -2);
}


/*
 * t[key] = v, where v is the value at the top of the stack
 * and t is the value just below the top.
 * This function will pop v from the stack.
 * Bypassing metamethods.
 */
inline void luaS_rawset(lua_State *L, const char *key) {
	lua_pushstring(L, key);
	lua_insert(L, -2);
	lua_rawset(L, -3);
}
