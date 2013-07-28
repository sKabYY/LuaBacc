/**/


class Iterator {
private:
	/* assert m_L == m_table.state() == m_key.state() == m_value.state() */
	lua_State *m_L;
	LuaRef m_table;
	LuaRef m_key;
	LuaRef m_value;

	void next() {
		m_table.push();
		m_key.push();
		if (lua_next(m_L, -2)) {
			m_value.pop();
			m_key.pop();
		} else {
			m_key = Nil();
			m_value = Nil();
		}
		lua_pop(m_L, 1);
	}

public:
	explicit Iterator(LuaRef table)
		: m_L(table.state()),
			m_table(table),
			m_key(table.state()),
			m_value(table.state()) {
		next();
	}

	lua_State* state() const {
		return m_L;
	}

	Iterator& operator++() {
		if (isNil()) {
			// if the iterator reaches the end, do nothing
			return *this;
		} else {
			next();
			return *this;
		}
	}

	inline bool isNil() const {
		return m_key.isNil();
	}

	inline LuaRef key() const {
		return m_key;
	}

	inline LuaRef value() const {
		return m_value;
	}

private:
	// Don't use postfix increment, it is less efficient
	Iterator& operator++(int);

};

