/**/


struct Nil {};


/*
 * Always use LuaRef as a local variable.
 * NEVER use LuaRef as a member/global/static variable.
 */
class LuaRef {

	class Proxy;
	friend class Proxy;
	friend struct LuaStack<LuaRef>;
	friend struct LuaStack<Proxy>;
	friend LuaRef getGlobal(lua_State*, const char*);
	friend std::ostream& operator<< (std::ostream &os, const LuaRef::Proxy &v);

private:

/*
 * A proxy for assigning table element (operator [] of LuaRef).
 * NEVER use Proxy alone.
 */
	class Proxy {

		friend class LuaRef;
		friend struct LuaStack<Proxy>;

	private:
		/*
		 * The table is in the registry,
		 * and the key is at the top of the stack
		 * The key is popped off the stack
		 */
		Proxy(lua_State *L, int tableRef)
			: m_L(L),
				m_tableRef(tableRef),
				m_keyRef(luaL_ref(L, LUA_REGISTRYINDEX)) {
		}


		/*
		 * This function may trigger a metamethod for the "index" event.
		 */
		int createRef() const {
			push();
			return luaL_ref(m_L, LUA_REGISTRYINDEX);
		}

		/*
		 * Push the value onto the Lua stack.
		 * May invoke metamethods.
		 */
		void push() const {
			lua_rawgeti(m_L, LUA_REGISTRYINDEX, m_tableRef);
			lua_rawgeti(m_L, LUA_REGISTRYINDEX, m_keyRef);
			lua_gettable(m_L, -2); // This may trigger the "index" event.
			lua_remove(m_L, -2);
		}

	private:
		lua_State *m_L;
		int m_tableRef;
		int m_keyRef;

	public:
		~Proxy() {
			luaL_unref(m_L, LUA_REGISTRYINDEX, m_keyRef);
		}

		/*
		 * Assign a value to this table key.
		 * May invoke metamethods.
		 */
		template <typename T>
		Proxy& operator= (T v) {
			lua_rawgeti(m_L, LUA_REGISTRYINDEX, m_tableRef);
			lua_rawgeti(m_L, LUA_REGISTRYINDEX, m_keyRef);
			LuaStack<T>::push(m_L, v);
			lua_settable(m_L, -3); // This may trgger the "newindex" event
			lua_pop(m_L, 1);
			return *this;
		}

		const Proxy& operator= (const Proxy &other) {
			lua_rawgeti(m_L, LUA_REGISTRYINDEX, m_tableRef);
			lua_rawgeti(m_L, LUA_REGISTRYINDEX, m_keyRef);
			other.push();
			lua_settable(m_L, -3); // This may trgger the "newindex" event
			lua_pop(m_L, 1);
			return *this;
		}

		lua_State* state() const {return m_L;}

		int type() const {
			push();
			int type = lua_type(m_L, -1);
			lua_pop(m_L, 1);
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
		 * Explicit conversion.
		 * May invoke metamethods.
		 */
		template <typename T>
		T cast() const {
			push();
			T t = LuaStack<T>::get(m_L, lua_gettop(m_L));
			lua_pop(m_L, 1);
			return t;
		}

		/*
		 * Implicit conversion
		 * May invoke metamethods.
		 */
		template <typename T>
		inline operator T () const {
			return cast<T>();
		}

		/*
		 * Access a table value using a key.
		 * May invoke metamethods.
		 */
		template <typename T>
		Proxy operator[] (T key) const {
			return LuaRef(*this)[key];
		}

		template <typename... Args>
		LuaRef operator() (Args... args) const {
			push();
			int nargs = pushArgs(m_L, args...);
			luaS_pcall(m_L, nargs, 1);
			return popLuaRef(m_L);
		}

		int length() const {
			push(); // push 1
			lua_len(m_L, -1); // push 1
			int len = luaL_checkinteger(m_L, -1);
			lua_pop(m_L, 2); // pop 2
			return len;
		}

		std::string tostring() const {
			lua_getglobal(m_L, "tostring");
			push();
			lua_call(m_L, 1, 1);
			std::string str = lua_tostring(m_L, 1);
			lua_pop(m_L, 1);
			return str;
		}

	private:
		/*
		 * Copy is NOT allowed.
		 */
		Proxy(const Proxy &other);

	};

private:
	lua_State *m_L;
	int m_ref;

	static int pushArgs(lua_State *L) {
		return 0;
	}

	template <typename Head, typename... Args>
	static int pushArgs(lua_State *L, Head h, Args... args) {
		LuaStack<Head>::push(L, h);
		int nargs = 1 + pushArgs(L, args...);
		return nargs;
	}

	static inline LuaRef popLuaRef(lua_State *L) {
		LuaRef v(L);
		v.pop();
		return v;
	}

	int createRef() const {
		if (m_ref != LUA_REFNIL) {
			push();
			return luaL_ref(m_L, LUA_REGISTRYINDEX);
		} else {
			return LUA_REFNIL;
		}
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

	/*
	 * Create a LuaRef from Proxy.
	 * May invoke metamethods.
	 */
	LuaRef(const Proxy &proxy)
		: m_L(proxy.state()),
			m_ref(proxy.createRef()) {
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

	/*
	 * May invoke metamethod.
	 */
	LuaRef& operator= (const Proxy &rhs) {
		luaL_unref(m_L, LUA_REGISTRYINDEX, m_ref);
		m_L = rhs.state();
		m_ref = rhs.createRef();
		return *this;
	}

	std::string tostring() const {
		lua_getglobal(m_L, "tostring");
		push();
		lua_call(m_L, 1, 1);
		std::string str = lua_tostring(m_L, 1);
		lua_pop(m_L, 1);
		return str;
	}

	lua_State* state() const {
		return m_L;
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

	/*
	 * [] for access to element table[key].
	 * May invoke metamethods.
	 * NOT USED
	template <typename T>
	LuaRef operator[] (T key) const {
		push();
		LuaStack<T>::push(m_L, key);
		lua_gettable(m_L, -2);
		LuaRef v = LuaRef::popLuaRef(m_L);
		lua_pop(m_L, 1);
		return v;
	}*/

	/*
	 * Access to element table[key].
	 */
	template <typename T>
	Proxy operator[] (T key) {
		LuaStack<T>::push(m_L, key);
		return Proxy(m_L, m_ref);
	}

	template <typename... Args>
	LuaRef operator() (Args... args) const {
		push();
		int nargs = pushArgs(m_L, args...);
		luaS_pcall(m_L, nargs, 1);
		return popLuaRef(m_L);
	}

};


LuaRef getGlobal(lua_State *L, const char *name) {
	lua_getglobal(L, name);
	return LuaRef::popLuaRef(L);
}


template <> struct LuaStack<Nil> {
	static inline void push(lua_State *L, Nil) {
		lua_pushnil(L);
	}
};


template <> struct LuaStack<LuaRef::Proxy> {
	static inline void push(lua_State *L, const LuaRef::Proxy &v) {
		assert(equalstates(L, v.state()));
		v.push();
	}
};


template <> struct LuaStack<LuaRef> {
	static inline void push(lua_State *L, const LuaRef &v) {
		assert(equalstates(L, v.state()));
		v.push();
	}
	static inline LuaRef get(lua_State *L, int index) {
		lua_pushvalue(L, index);
		return LuaRef::popLuaRef(L);
	}
};


void printLuaRef(std::ostream &os, const LuaRef &v) {
	int type = v.type();
	switch (type) {
	case LUA_TNIL:
		os << "nil";
		break;
	case LUA_TNUMBER:
		os << v.cast<lua_Number>();
		break;
	case LUA_TBOOLEAN:
		os << (v.cast<bool>() ? "true" : "false");
		break;
	case LUA_TSTRING:
		os << '"' << v.cast<std::string>() << '"';
		break;
	case LUA_TTABLE:
	case LUA_TFUNCTION:
	case LUA_TUSERDATA:
	case LUA_TTHREAD:
	case LUA_TLIGHTUSERDATA:
		os << v.tostring();
		break;
	default:
		os << "unknown";
		break;
	}
}

inline std::ostream& operator<< (std::ostream &os, const LuaRef &v) {
	printLuaRef(os, v);
	return os;
}

inline std::ostream& operator<< (std::ostream &os, const LuaRef::Proxy &v) {
	printLuaRef(os, LuaRef(v));
	return os;
}

