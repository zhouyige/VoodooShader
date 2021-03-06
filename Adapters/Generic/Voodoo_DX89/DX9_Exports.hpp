/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2013 by Sean Sube
 *
 * The Voodoo Shader Framework is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option)
 * any later version.  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with this program; if not, write to
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 US
 *
 * Support and more information may be found at
 *   http://www.voodooshader.com
 * or by contacting the lead developer at
 *   peachykeen@voodooshader.com
 */
#pragma once

// Voodoo DX9
#include "CVoodoo3D8.hpp"
#include "CVoodoo3D9.hpp"
// System
#define WIN32_LEAN_AND_MEAN
#pragma warning(push,3)
#include <windows.h>
#pragma warning(pop)

HMODULE WINAPI VSLoadLibraryA(_In_ LPCSTR lpFileName);
HMODULE WINAPI VSLoadLibraryW(_In_ LPCWSTR lpFileName);
HMODULE WINAPI VSLoadLibraryExA(_In_ LPCSTR lpFileName, HANDLE hFile, _In_ DWORD dwFlags);
HMODULE WINAPI VSLoadLibraryExW(_In_ LPCWSTR lpFileName, HANDLE hFile, _In_ DWORD dwFlags);

IDirect3D8 * WINAPI VSDirect3DCreate8(UINT SDKVersion);
IDirect3D9 * WINAPI VSDirect3DCreate9(UINT SDKVersion);
