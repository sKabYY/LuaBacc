LuaBacc (IN PROCESS)
=======

[LuaBacc][luabacc] is a Lua binder. It is similar to [LuaBridge][luabridge]. By the way, [Luabind][] is an amazing Lua binder.

Need a compiler with <strong>C++0x</strong> feature support.

###The meaning of "bacc":
> - Maybe "b" means binding,
> - "bac" is the shortcut of "bachelor" (If you meet a chinese bachelor, just say "Lua" to him and then you will be friends),
> - and "cc" means C++.
> - On the other hand, "bacc" is pronounced like "idiot" in japanese.
> - I hope LuaBridge will be easy to use even if the user is an idiot XD. 
> - Anyways, finally, "bacc" means nothing but being fun ^ n^.

Support
-------
###Accessing lua from C++
- Accessing global variables.
- Accessing elements in a table by indexes. This will invoke metamethod (index/newindex event).
- Calling lua function from C++. Supporting variable arguements.
- Iterating lua table.

###Accessing C++ from lua
- Calling C++ function from lua. Supporting variable arguements.
- Binding C++ class to lua.
- Binding constructors to lua.
- Auto binding destructor to lua.
- Binding member functions to lua.
- Binding static member functions to lua.
- Binding a derived class to lua.

###TODO
- Add a feature to bind a pointer of a C++ object to lua.
- Write manual.
- There is always something to be done.

External links
--------------
- [Lua 5.2 Reference Manual][luaManual]
- [LuaBridge 2.0 Reference Manual][luabridgeReference]


  [luabacc]: https://github.com/sKabYY/LuaBacc
  [luabridge]: https://github.com/vinniefalco/LuaBridge
  [luabind]: http://www.rasterbar.com/products/luabind.html
  [luaManual]: http://www.lua.org/manual/5.2/manual.html
  [luabridgeReference]: http://vinniefalco.com/LuaBridge/Manual.html
