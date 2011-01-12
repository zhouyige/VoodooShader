/**************************************************************************************************\
* This file is part of the Voodoo Shader Framework, a comprehensive shader support library.
* Copyright (c) 2010-2011 by Sean Sube
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

#include "Frost_Module.hpp"

typedef std::map<std::string, TRACED_HOOK_HANDLE> HookMap;

class HookManager
{
public:
	HookManager();
	~HookManager();

	//void AddThread(ULONG threadID);
	//void ResetThread();

	bool InstallHook(std::string name, void * src, void * dest);
	void UninstallHook(std::string name);
	void UninstallAllHooks();

private:
	HookMap mHooks;
	ULONG * mThreadIDs;
	ULONG mThreadCount;
};
