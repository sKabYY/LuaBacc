/**/

/* A wrapper for lua_State */
class LuaState {
private:
	lua_State *m_L;

public:

	LuaState()
		: m_L(luaS_newstate()) {
	}

	~LuaState() {
		luaS_close(m_L);
	}

	lua_State* state() const {
		return m_L;
	}

	void dofile(std::string path) {
		luaS_dofile(m_L, path);
	}

	void dostring(std::string content) {
		luaS_dostring(m_L, content);
	}

	LuaRef getGlobal(const char *name) {
		return LuaRef::getGlobal(m_L, name);
	}

	__bacc::Namespace module() {
		return __bacc::Namespace::module(m_L);
	}

	LuaRef newNil() {
		return LuaRef(m_L);
	}

	template <typename T>
	LuaRef newLuaRef(T v) {
		return LuaRef(m_L, v);
	}

private:
	/*
	 * Copy is not allowed.
	 */
	LuaState(const LuaState &other);
	LuaState& operator= (const LuaState &rhs);

};

