/**/


namespace __bacc {

	class Namespace {
	private:
		lua_State* m_L;
		bool m_isEnd;
		Namespace *const m_parent;
		Namespace operator= (Namespace const& other);

	private:
		Namespace(lua_State* L)
			: m_L(L),
				m_isEnd(false),
				m_parent(NULL) {
			lua_rawgeti(m_L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
		}

		Namespace(Namespace* parent, char const* name)
			: m_L(parent->m_L),
				m_isEnd(false),
				m_parent(parent) {
			assert(lua_istable(m_L, -1));
			luaS_rawget(m_L, name);
			if (lua_isnil(m_L, -1)) {
				lua_pop(m_L, 1);
				lua_newtable(m_L);
				luaS_rawset(m_L, name);
				luaS_rawget(m_L, name);
			}
		}

		void pop() {
			if (!m_isEnd) {
				lua_pop(m_L, 1);
				m_isEnd = true;
			}
		}
	
	public:
		~Namespace() {
			pop();
		}

		Namespace(Namespace const& other)
			: m_L(other.m_L),
				m_isEnd(other.m_isEnd),
				m_parent(other.m_parent) {}

		Namespace namespace_(char const* name) {
			return Namespace(this, name);
		}

		Namespace& end() {
			pop();
			if (m_parent == NULL) {
				return *this;
			} else {
				return *m_parent;
			}
		}

		template <typename R, typename... Ps>
		Namespace& def(char const* name, R (*fp)(Ps...)) {
			typedef R (*FP)(Ps...);
			new (lua_newuserdata(m_L, sizeof(fp))) FP (fp);
			lua_pushcclosure(m_L, &__bacc::CFunction<FP>::call, 1);
			luaS_rawset(m_L, name);
			return *this;
		}

		static Namespace module(lua_State* L) {
			return Namespace(L);
		}
	};

}

__bacc::Namespace module(lua_State* L) {
	return __bacc::Namespace::module(L);
}

