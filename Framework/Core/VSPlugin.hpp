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

#include "VoodooInternal.hpp"

namespace VoodooShader
{
    /**
     * @clsid e6f3129f-05af-11e1-9e05-005056c00008
     */
    VOODOO_CLASS(VSPlugin, IPlugin, ({0x9F, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08}))
    {
    public:
        static VSPlugin * Load(_In_ IPluginServer * pServer, _In_ CONST String & path);

        VSPlugin(_In_ IPluginServer * pServer, _In_ HMODULE hmodule);

        VOODOO_METHOD_(uint32_t, AddRef)() CONST;
        VOODOO_METHOD_(uint32_t, Release)() CONST;
        VOODOO_METHOD(QueryInterface)(_In_ CONST Uuid refid, _Outptr_result_maybenull_ IObject ** ppOut);
        VOODOO_METHOD_(String, ToString)() CONST;
        VOODOO_METHOD_(ICore *, GetCore)() CONST;

        VOODOO_METHOD_(CONST Version *, PluginInit)(_In_ ICore * pCore) CONST;
        VOODOO_METHOD_(void, PluginReset)(_In_ ICore * pCore) CONST;
        VOODOO_METHOD_(uint32_t, ClassCount)() CONST;
        VOODOO_METHOD_(CONST wchar_t *, ClassInfo)(_In_ CONST uint32_t number, _Out_ Uuid * pUuid) CONST;
        VOODOO_METHOD_(IObject *, CreateClass)(_In_ CONST uint32_t number, _In_ ICore * pCore) CONST;

    private:
        // Private these to prevent copying internally (external libs never will).
        VSPlugin(CONST VSPlugin & other);
        VSPlugin & operator=(CONST VSPlugin & other);
        ~VSPlugin();

        mutable uint32_t m_Refs;
        IPluginServer * m_Server;

        HMODULE m_Handle;
        Functions::PluginInitFunc m_PluginInit;
        Functions::PluginResetFunc m_PluginReset;
        Functions::ClassCountFunc m_ClassCount;
        Functions::ClassInfoFunc m_ClassInfo;
        Functions::ClassCreateFunc m_ClassCreate;
    };
}
