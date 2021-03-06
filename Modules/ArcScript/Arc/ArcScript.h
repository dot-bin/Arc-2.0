/* This file is part of ArcScript.
 * 
 * ArcScript is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * ArcScript is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with ArcScript; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * 
 * Author: Stephen Lane-Walsh
 * 
 */

#ifndef ARC_SCRIPT_ARC_SCRIPT
#define ARC_SCRIPT_ARC_SCRIPT

#include "ArcScriptVersion.h"

extern "C"
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

namespace Arc
{

bool Arc_InitScript( void );
void Arc_TermScript( void );

int Arc_lua_pause( lua_State* pState );
int Arc_lua_log_info( lua_State* pState );
int Arc_lua_log_error( lua_State* pState );

void Arc_RunScript( const string& filename );

extern lua_State *gp_LuaState;

}

#endif // ARC_SCRIPT_ARC_SCRIPT
