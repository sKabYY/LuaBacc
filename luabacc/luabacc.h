#ifndef LUABACC_LUABACC_H
#define LUABACC_LUABACC_H

#include <cassert>
#include <string>

namespace luabacc {

inline bool equalstates(lua_State *L1, lua_State *L2) {
	return lua_topointer(L1, LUA_REGISTRYINDEX) ==
			lua_topointer(L2, LUA_REGISTRYINDEX);
}

#include "bacc/luastack.h"
#include "bacc/luaref.h"

inline lua_State* luaS_newstate() {
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	return L;
}

inline void luaS_close(lua_State *L) {
	lua_close(L);
}

inline void luaS_dofile(lua_State *L, std::string path) {
	if (luaL_dofile(L, path.c_str())) {
		std::string errstr = lua_tostring(L, -1);
		throw errstr;
	}
}

}

#endif

