/**/


namespace __bacc {
	/*
	 * An unique Lua registry keys for each member class.
	 * Used with lua_rawsetp and lua_rawgetp.
	 * Binding: lua_rawsetp(L, LUA_REGISTRYINDEX, TypeInfo<T>::key())
	 * Get table: lua_rawgetp(L, LUA_REGISTRYINDEX, TypeInfo<T>::key())
	 */
	template <typename T>
	struct TypeInfo {
		// key for member table
		static void const* key() {
			static char never_use;
			return &never_use;
		}
	};
}

