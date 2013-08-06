/**/


namespace __bacc {
	/*
	 * Unique Lua registry keys for each class.
	 * Used with lua_rawsetp and lua_rawgetp.
	 */
	template <typename T>
	struct TypeInfo {
		// key for class table
		static void const* classKey() {
			static char never_use;
			return &never_use;
		}
		// key for member table
		static void const* memberKey() {
			static char never_use;
			return &never_use;
		}
	};
}

