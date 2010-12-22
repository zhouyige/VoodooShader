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

#define VOODOO_CORE_META__STRING(arg) #arg
#define VOODOO_CORE_META_STRING(arg) VOODOO_CORE_META__STRING(arg)

#define VOODOO_CORE_VERSION_MAJOR	0
#define VOODOO_CORE_VERSION_MINOR	1
#define VOODOO_CORE_VERSION_GITREV	53
#define VOODOO_CORE_VERSION_BUILD	2
#define VOODOO_CORE_VERSION_STRING	"Voodoo Shader Framework (version "\
	VOODOO_CORE_META_STRING(VOODOO_CORE_VERSION_MAJOR)  "."\
	VOODOO_CORE_META_STRING(VOODOO_CORE_VERSION_MINOR)  "."\
	VOODOO_CORE_META_STRING(VOODOO_CORE_VERSION_GITREV) "."\
	VOODOO_CORE_META_STRING(VOODOO_CORE_VERSION_BUILD)  ")\n\n"