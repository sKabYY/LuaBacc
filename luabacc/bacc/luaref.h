/**/


struct Nil {};


/*
 * Always use LuaRef as local variable.
 * NEVER use LuaRef as member/global/static variable.
 */
class LuaRef {
private:
	lua_State *m_L;
	int m_ref;

	int createRef() const {
		if (m_ref != LUA_REFNIL) {
			push();
			return luaL_ref(m_L, LUA_REGISTRYINDEX);
		} else {
			return LUA_REFNIL;
		}
	}

public:
	explicit LuaRef(lua_State *L)
		: m_L(L),
			m_ref(LUA_REFNIL) {
	}

	template <typename T>
	LuaRef(lua_State *L, T v)
		: m_L(L) {
		LuaStack<T>::push(m_L, v);
		m_ref = luaL_ref(m_L, LUA_REGISTRYINDEX);
	}
	
	LuaRef(const LuaRef &other)
		: m_L(other.m_L),
			m_ref(other.createRef()) {
	}

	~LuaRef() {
		luaL_unref(m_L, LUA_REGISTRYINDEX, m_ref);
	}

	template <typename T>
	LuaRef& operator= (T rhs) {
		luaL_unref(m_L, LUA_REGISTRYINDEX, m_ref);
		LuaStack<T>::push(m_L, rhs);
		m_ref = luaL_ref(m_L, LUA_REGISTRYINDEX);
		return *this;
	}

	LuaRef& operator= (const LuaRef &rhs) {
		luaL_unref(m_L, LUA_REGISTRYINDEX, m_ref);
		m_L = rhs.state();
		rhs.push();
		m_ref = luaL_ref(m_L, LUA_REGISTRYINDEX);
		return *this;
	}

	std::string tostring() const {
		lua_getglobal(m_L, "tostring");
		push();
		lua_call(m_L, 1, 1);
		const char *str = lua_tostring(m_L, 1);
		lua_pop(m_L, 1);
		return std::string(str);
	}

	lua_State* state() const {
		return m_L;
	}

	/*
	 * Push the object onto the Lua stack
	 */
	void push() const {
		lua_rawgeti(m_L, LUA_REGISTRYINDEX, m_ref);
	}

	/*
	 * Pop the top of Lua stack and assign the ref to this->m_ref
	 */
	void pop() {
		luaL_unref(m_L, LUA_REGISTRYINDEX, m_ref);
		m_ref = luaL_ref(m_L, LUA_REGISTRYINDEX);
	}

	/*
	 * Return lua_type
	 */
	int type() const {
		int type;
		if (m_ref != LUA_REFNIL) {
			push();
			type = lua_type(m_L, -1);
			lua_pop(m_L, 1);
		} else {
			type = LUA_TNIL;
		}
		return type;
	}

	inline bool isNil() const {return type() == LUA_TNIL;}
	inline bool isNumber() const {return type() == LUA_TNUMBER;}
	inline bool isString() const {return type() == LUA_TSTRING;}
	inline bool isTable() const {return type() == LUA_TTABLE;}
	inline bool isFunction() const {return type() == LUA_TFUNCTION;}
	inline bool isUserdata() const {return type() == LUA_TUSERDATA;}
	inline bool isThread() const {return type() == LUA_TTHREAD;}
	inline bool isLightUserdata() const {return type() == LUA_TLIGHTUSERDATA;}

	/*
	 * Explicit conversion
	 */
	template <typename T>
	T cast() const {
		push();
		T t = LuaStack<T>::get(m_L, lua_gettop(m_L));
		lua_pop(m_L, 1);
		return t;
	}

	template <typename T>
	inline operator T () const {
		return cast<T>();
	}

	int length() const {
		push(); // push 1
		lua_len(m_L, -1); // push 1
		int len = luaL_checkinteger(m_L, -1);
		lua_pop(m_L, 2); // pop 2
		return len;
	}

	template <typename T>
	LuaRef operator[] (T key) const {
		// TODO
	}

	LuaRef const operator() () const {
		// TODO
	}

};


LuaRef getGlobal(lua_State *L, const char *name) {
	lua_getglobal(L, name);
	LuaRef v(L);
	v.pop();
	return v;
}


template<> struct LuaStack<Nil> {
	static inline void push(lua_State *L, Nil) {
		lua_pushnil(L);
	}
};


template<> struct LuaStack<LuaRef> {
	static inline void push(lua_State *L, const LuaRef &v) {
		assert(equalstates(L, v.state()));
		v.push();
	}
	static inline LuaRef get(lua_State *L, int index) {
		lua_pushvalue(L, index);
		LuaRef v(L);
		v.pop();
		return v;
	}
};

