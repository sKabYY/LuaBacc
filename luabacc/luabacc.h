#ifndef LUABACC_LUABACC_H
#define LUABACC_LUABACC_H

#include <cassert>
#include <string>

/*
 * Nerver use class/function in namespace __bacc
 * unless you exactly know what you are doing.
 */
namespace luabacc {

#include "bacc/utils.h"

#include "bacc/luastack.h"
#include "bacc/cfunction.h"
#include "bacc/constructor.h"

#include "bacc/namespace.h"
#include "bacc/luaref.h"
#include "bacc/iterator.h"

#include "bacc/luastate.h"

}

#endif

