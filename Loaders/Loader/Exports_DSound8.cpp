/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2012 by Sean Sube
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

#include "ExportLoadpoints.hpp"
#include "VoodooTie.hpp"

#include "Support.inl"

#include <stdlib.h>

typedef DWORD (WINAPI * Type_DirectSoundCreate8)(LPCGUID, LPVOID *, LPVOID);

HMODULE gModule_DS8;
Type_DirectSoundCreate8 gFunc_DirectSoundCreate8;

/**
 * DirectSound 8
 */
HRESULT WINAPI VSDirectSoundCreate8(LPCGUID lpcGuidDevice, LPVOID * ppDS8, LPVOID pUnkOuter)
{
    VOODOO_DEBUGBREAK;

    if (!gFunc_DirectSoundCreate8)
    {
        gFunc_DirectSoundCreate8 = (Type_DirectSoundCreate8)FindFunction(TEXT("dsound8.dll"), true, "DirectSoundCreate8", &gModule_DS8);
    }

    LPVOID s8obj = nullptr;
    DWORD result = gFunc_DirectSoundCreate8(lpcGuidDevice, &s8obj, pUnkOuter);

    if (FAILED(result))
    {
        *ppDS8 = s8obj;
        return result;
    }
    
    if (InterlockedCompareExchange(&gSingleExport, 1, 0) == 0)
    {
        GlobalLog(VSTR("Loading Voodoo Shader from %s."), VSTR(__FUNCTION__));
        if (LoadVoodoo())
        {
            VoodooShader::Variant DSObj;
            DSObj.Type = VoodooShader::UT_PVoid;
            DSObj.VPVoid = s8obj;

            if (gVoodooCore->GetAdapter()->SetProperty(VoodooShader::PropIds::DS8Device, &DSObj) == VSFOK_PROPERTYCHANGED)
            {
                s8obj = DSObj.VPVoid;
            }
        }
        gSingleExport = false;
    }

    *ppDS8 = s8obj;
    return result;
}
