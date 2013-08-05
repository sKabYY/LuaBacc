/**/


namespace __bacc {

	class Namespace {
		friend class Class;

	private:

		/*
		 * Define the prototype of class T.
		 * The prototype is a table in lua.
		 */
		template <typename T>
		class Class {
			friend class Namespace;

		private:
			lua_State *const m_L;
			Namespace *const m_namespace;
			bool m_isEnd;
			Class operator= (Class const* other);

		private:
			/*
			 * Push the class named "name" on the top of the stack
			 * or create a class named "name" and push it on the top of the stack
			 */
			void load_or_create(char const* name) {
				assert(lua_istable(m_L, -1));
				luaS_rawget(m_L, name);
				if (lua_isnil(m_L, -1)) {
					lua_pop(m_L, 1);
					lua_newtable(m_L);
					luaS_rawset(m_L, name);
					luaS_rawget(m_L, name);
					lua_pushvalue(m_L, -1);
					luaS_rawset(m_L, "__index");
					lua_pushcclosure(m_L, &__bacc::CDestructor<T>::call, 0);
					luaS_rawset(m_L, "__gc");
				}
				assert(lua_istable(m_L, -1));
			}

			Class(Namespace* ns, char const* name)
				: m_L(ns->m_L),
					m_namespace(ns),
					m_isEnd(false) {
				load_or_create(name);
			}

			Class(Namespace* ns, char const* name, char const* superclassname)
				: m_L(ns->m_L),
					m_namespace(ns),
					m_isEnd(false) {
				load_or_create(name);
				luaS_rawgeti(m_L, -2, superclassname);
				assert(lua_istable(m_L, -1));
				lua_setmetatable(m_L, -2);
			}

			void pop() {
				if (!m_isEnd) {
					lua_pop(m_L, 1);
					m_isEnd = true;
				}
			}

		public:
			~Class() {
				pop();
			}

			Class(Class const& other)
				: m_L(other.m_L),
					m_namespace(other.m_namespace),
					m_isEnd(other.m_isEnd) {
				assert(0 && "Calling Class(Class const&)");
			}

			Namespace& end() {
				pop();
				return *m_namespace;
			}

			/*
			 * Define constructor without a name.
			 */
			template <typename... Ps>
			Class<T>& def(constructor<Ps...>) {
				if (lua_getmetatable(m_L, -1) == 0)
					lua_newtable(m_L);
				lua_pushcclosure(m_L, &__bacc::Constructor<T, Ps...>::call, 0);
				luaS_rawset(m_L, "__call");
				lua_setmetatable(m_L, -2);
				return *this;
			}

			/*
			 * Define constructor with a name.
			 */
			template <typename... Ps>
			Class<T>& def(char const* name, constructor<Ps...>) {
				lua_pushcclosure(m_L, &__bacc::Constructor<T, Ps...>::call, 0);
				luaS_rawset(m_L, name);
				return *this;
			}

			/*
			 * Define a member function.
			 */
			template <typename R, typename... Ps>
			Class<T>& def(char const* name, R (T::*mfp)(Ps...)) {
				typedef R (T::*MFP)(Ps...);
				new (lua_newuserdata(m_L, sizeof(mfp))) MFP(mfp);
				lua_pushcclosure(m_L, &__bacc::CFunction<MFP>::call, 1);
				luaS_rawset(m_L, name);
				return *this;
			}

			/*
			 * Define a const member function.
			 */
			template <typename R, typename... Ps>
			Class<T>& def(char const* name, R (T::*mfp)(Ps...) const) {
				typedef R (T::*MFP)(Ps...) const;
				new (lua_newuserdata(m_L, sizeof(mfp))) MFP(mfp);
				lua_pushcclosure(m_L, &__bacc::CFunction<MFP>::call, 1);
				luaS_rawset(m_L, name);
				return *this;
			}

			/*
			 * Define a static member function.
			 */
			template <typename R, typename... Ps>
			Class<T>& def(char const* name, R (*fp)(Ps...)) {
				typedef R (*FP)(Ps...);
				new (lua_newuserdata(m_L, sizeof(fp))) FP(fp);
				lua_pushcclosure(m_L, &__bacc::CFunction<FP>::call, 1);
				luaS_rawset(m_L, name);
				return *this;
			}
		};

	private:
		lua_State *const m_L;
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
			assert(lua_istable(m_L, -1));
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
				m_parent(other.m_parent) {
			assert(0 && "Calling Namespace(Namespace const&)");
		}

		/*
		 * Create or begin a namespace.
		 */
		Namespace namespace_(char const* name) {
			return Namespace(this, name);
		}

		/*
		 * End a namespace.
		 * Do not use this on global namespace (module).
		 */
		Namespace& end() {
			pop();
			assert(m_parent != NULL);
			return *m_parent;
		}

		/*
		 * Add a function
		 */
		template <typename R, typename... Ps>
		Namespace& def(char const* name, R (*fp)(Ps...)) {
			typedef R (*FP)(Ps...);
			new (lua_newuserdata(m_L, sizeof(fp))) FP(fp);
			lua_pushcclosure(m_L, &__bacc::CFunction<FP>::call, 1);
			luaS_rawset(m_L, name);
			return *this;
		}

		/*
		 * Create or begin a class
		 */
		template <typename C>
		Class<C> class_(char const* name) {
			return Class<C>(this, name);
		}

		/*
		 * Superclassname is the lua name of the superclass which
		 * has been registered in lua.
		 * Superclass must be in the same namespace with class C.
		 * The derived class does not inherit the constructors.
		 * Calling the constructors of the superclass just return 
		 * instance of the superclass and may cause unpredictable results.
		 */
		template <typename C>
		Class<C> class_(char const* name, char const* superclassname) {
			return Class<C>(this, name, superclassname);
		}

		/*
		 * Begin binding!
		 */
		static Namespace module(lua_State* L) {
			return Namespace(L);
		}
	};

}

__bacc::Namespace module(lua_State* L) {
	return __bacc::Namespace::module(L);
}

