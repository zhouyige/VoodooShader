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
     * @addtogroup voodoo_core_null Null Implementations
     * @ingroup voodoo_core
     *
     * @{
     */

    /**
     * Voodoo Shader null hook manager implementation. Returns true or nullptr for methods as necessary, does not install
     * any hooks. Does not perform logging or parameter validation.
     *
     * @clsid e6f3129d-05af-11e1-9e05-005056c00008
     */
    VOODOO_CLASS(VSHookManager, IHookManager, ({0x9D, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08}))
    {
    public:
        VSHookManager(_In_ ICore * pCore);

        VOODOO_METHOD_(uint32_t, AddRef)() CONST;
        VOODOO_METHOD_(uint32_t, Release)() CONST;
        VOODOO_METHOD(QueryInterface)(_In_ CONST Uuid refid, _Outptr_result_maybenull_ IObject ** ppOut);
        VOODOO_METHOD_(String, ToString)() CONST;
        VOODOO_METHOD_(ICore *, GetCore)() CONST;

        VOODOO_METHOD(Add)(_In_ CONST String & name, _In_ void * pSrc, _In_ void * pDest);
        VOODOO_METHOD(Remove)(_In_ CONST String & name);
        VOODOO_METHOD(RemoveAll)();

    private:
        // Private these to prevent copying internally (external libs never will).
        VSHookManager(CONST VSHookManager & other);
        VSHookManager & operator=(CONST VSHookManager & other);
        ~VSHookManager();

        mutable uint32_t m_Refs;
        ICore * m_Core;
    };
    /**
     * @}
     */
}
