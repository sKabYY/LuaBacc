#pragma once


template <typename FP, typename R, typename... T>
struct Caller;

template <typename FP>
struct Caller<FP, void> {
	template <typename... U>
	static int call(lua_State* L, U... u) {
		FP const& fp = *static_cast<FP*>(lua_touserdata(L, lua_upvalueindex(1)));
		fp(u...);
		return 0;
	}
};

template <typename FP, typename R>
struct Caller<FP, R> {
	template <typename... U>
	static int call(lua_State* L, U... u) {
		FP const& fp = *static_cast<FP*>(lua_touserdata(L, lua_upvalueindex(1)));
		__bacc::LuaStack<R>::push(L, fp(u...));
		return 1;
	}
};

template <typename FP, typename R, typename H, typename... T>
struct Caller<FP, R, H, T...> {
	template <typename... U>
	static int call(lua_State* L, U... u) {
		const int n = -static_cast<int>(1 + sizeof...(u));
		H h = __bacc::LuaStack<H>::get(L, n);
		return Caller<FP, R, T...>::call(L, h, u...);
	}
};


template <typename FP>
struct CFunc;

template <typename R, typename... Ps>
struct CFunc<R (*)(Ps...)> {
	typedef R (*FP)(Ps...);
	static int call(lua_State* L) {
		return Caller<FP, R, Ps...>::call(L);
	}
};


struct TestModule {
	TestModule(lua_State* L) 
		: m_L(L) {
		lua_rawgeti(m_L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
	}

	TestModule(lua_State* L, char const* name)
		: m_L(L) { // namespace
		lua_newtable(m_L);
		lua_setglobal(m_L, name);
		lua_getglobal(m_L, name);
	}

	~TestModule() {
		lua_pop(m_L, 1);
	}

	template <typename FP>
	void deffunction(char const* name, FP fp) {
		new (lua_newuserdata(m_L, sizeof(fp))) FP (fp);
		lua_pushcclosure(m_L, &CFunc<FP>::call, 1);
		luaS_rawset(m_L, name);
	}

	template <typename T>
	void def(char const* name, T t) {
		std::cout << "def T" << endl;
	}

	template <typename R, typename... Ps>
	void def(char const* name, R (*fp)(Ps...)) {
		typedef R (*FP)(Ps...);
		std::cout << "def function" << std::endl;
		new (lua_newuserdata(m_L, sizeof(fp))) FP (fp);
		lua_pushcclosure(m_L, &CFunc<FP>::call, 1);
		luaS_rawset(m_L, name);
	}

private:
	lua_State* const m_L;
};

