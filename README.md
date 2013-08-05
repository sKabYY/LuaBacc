LuaBacc (IN PROCESS)
=======

[LuaBacc][luabacc] is a Lua binder. It is similar to [LuaBridge][luabridge].

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

###TODO
- There is always something to be done.


  [luabacc]: https://github.com/sKabYY/LuaBacc
  [luabridge]: https://github.com/vinniefalco/LuaBridge
