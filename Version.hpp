/**************************************************************************************************\
* This file is part of the Voodoo Shader Framework, a comprehensive shader support library.
* Copyright (C) 2010 by Sean Sube
*
*
* This program is free software; you can redistribute it and/or modify it under the terms of the 
* GNU General Public License as published by the Free Software Foundation; either version 2 of the 
* License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
* even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; 
* if  not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, 
* Boston, MA  02110-1301 US
*
* Support and more information may be found at http://www.voodooshader.com, or by contacting the
* developer at peachykeen@voodooshader.com
\**************************************************************************************************/

// Functions
#define VOODOO_META__STRING(arg) #arg
#define VOODOO_META_STRING(arg) VOODOO_META__STRING(arg)

#ifdef _DEBUG
#	define VOODOO_META_DEBUG_STRING " (DEBUG)"
#else
#	define VOODOO_META_DEBUG_STRING
#endif

#define VOODOO_META_VERSION_STRING_BASIC(token)\
	VOODOO_META_STRING(VOODOO_##token##_VERSION_MAJOR) "."\
	VOODOO_META_STRING(VOODOO_##token##_VERSION_MINOR) "."\
	VOODOO_META_STRING(VOODOO_##token##_VERSION_PATCH) "."\
	VOODOO_META_STRING(VOODOO_##token##_VERSION_GITREV)\
	VOODOO_META_DEBUG_STRING

#define VOODOO_META_VERSION_STRING_FULL(token)\
	VOODOO_##token##_PRETTYNAME " (version " VOODOO_META_VERSION_STRING_BASIC(token) ")"

#define VOODOO_META_VERSION_CHAIN(token)\
	VOODOO_##token##_VERSION_MAJOR,\
	VOODOO_##token##_VERSION_MINOR,\
	VOODOO_##token##_VERSION_PATCH,\
	VOODOO_##token##_VERSION_GITREV

#define VOODOO_META_NAME(token) VOODOO_##token##_NAME
#define VOODOO_META_PRETTYNAME(token) VOODOO_##token##_PRETTYNAME



// Global defs ----------------------------------------------------------------
#define VOODOO_GLOBAL_VERSION_GITREV	61
#define VOODOO_GLOBAL_COPYRIGHT_BRIEF	"Copyright (c) 2010 by Sean Sube"
//-----------------------------------------------------------------------------


// Core defs ------------------------------------------------------------------
#define VOODOO_CORE_NAME			"Voodoo/_Core"
#define VOODOO_CORE_PRETTYNAME		"Voodoo Shader Framework Core"
#define VOODOO_CORE_VERSION_MAJOR	0
#define VOODOO_CORE_VERSION_MINOR	1
#define VOODOO_CORE_VERSION_PATCH	6
#define VOODOO_CORE_VERSION_GITREV	VOODOO_GLOBAL_VERSION_GITREV
//-----------------------------------------------------------------------------


// DX9 defs -------------------------------------------------------------------
#define VOODOO_DX9_NAME				"Voodoo/_DX9"
#define VOODOO_DX9_PRETTYNAME		"Voodoo/Generic (DX9)"
#define VOODOO_DX9_VERSION_MAJOR	0
#define VOODOO_DX9_VERSION_MINOR	1
#define VOODOO_DX9_VERSION_PATCH	8
#define VOODOO_DX9_VERSION_GITREV	VOODOO_GLOBAL_VERSION_GITREV
//-----------------------------------------------------------------------------


// DX89 defs ------------------------------------------------------------------
#define VOODOO_DX89_NAME			"Voodoo/_DX89"
#define VOODOO_DX89_PRETTYNAME		"Voodoo/Generic (DX8.9)"
#define VOODOO_DX89_VERSION_MAJOR	0
#define VOODOO_DX89_VERSION_MINOR	1
#define VOODOO_DX89_VERSION_PATCH	5
#define VOODOO_DX89_VERSION_GITREV	VOODOO_GLOBAL_VERSION_GITREV
//-----------------------------------------------------------------------------


// Gem defs -------------------------------------------------------------------
#define VOODOO_GEM_NAME				"Voodoo/Gem"
#define VOODOO_GEM_PRETTYNAME		"Voodoo/Gem"
#define VOODOO_GEM_VERSION_MAJOR	0
#define VOODOO_GEM_VERSION_MINOR	1
#define VOODOO_GEM_VERSION_PATCH	3
#define VOODOO_GEM_VERSION_GITREV	VOODOO_GLOBAL_VERSION_GITREV
//-----------------------------------------------------------------------------
