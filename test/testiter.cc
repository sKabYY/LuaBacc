#include "base.h"
#include <sstream>
#include <map>

const char* integer_iter_msg = 
"1: \"a\"\n"
"2: \"b\"\n"
"3: true\n"
"4: false\n"
"5: 5.5\n";


void func(lua_State *L) {
	luaS_dofile(L, "iter.lua");
	LuaRef t = getGlobal(L, "t");
	int len = t.length();
	std::stringstream ostr;
	for (int i = 1; i <= len; ++i) {
		ostr << i << ": " << t[i] << std::endl;
	}
	assert(ostr.str() == integer_iter_msg);
	std::map<std::string, std::string> m;
	for (Iterator iter(t); !iter.isNil(); ++iter) {
		m[iter.key().tostring()] = iter.value().tostring();
	}
	assert(m["1"] == "a");
	assert(m["2"] == "b");
	assert(m["3"] == "true");
	assert(m["4"] == "false");
	assert(m["5"] == "5.5");
	assert(m["is"] == "=>");
	assert(m["anything"] == "false");
	assert(m["everything"] == "42");
	assert(m.find("love") == m.end());
}

