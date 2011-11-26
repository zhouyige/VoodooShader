/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2011 by Sean Sube
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

#include "ILogger.hpp"

namespace VoodooShader
{
    /**
     * @addtogroup voodoo_core_null Null Implementations
     * @ingroup voodoo_core
     * 
     * @{
     */

    /**
     * @addtogroup voodoo_uuids 
     */
    DEFINE_CLSID(VSLogger) = {0x9E, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08};

    /**
     * Voodoo Shader null logger implementation. Does not format or log messages or access any files.
     * 
     * @par CLSID:
     *      e6f3129e-05af-11e1-9e05-005056c00008
     */
    class VSLogger :
        public ILogger
    {
    public:
        VSLogger(ICore * pCore);
        virtual ~VSLogger();

        VOODOO_METHODCALL_(uint32_t, AddRef)() CONST;
        VOODOO_METHODCALL_(uint32_t, Release)() CONST;
        VOODOO_METHODCALL(QueryInterface)(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST;
        VOODOO_METHODCALL_(String, ToString)() CONST;
        VOODOO_METHODCALL_(ICore *, GetCore)() CONST;

        VOODOO_METHODCALL(Open)(_In_ const String & filename, _In_ const bool append) ;
        VOODOO_METHODCALL(Open)(_In_ IFile * const pFile, _In_ const bool append) ;
        VOODOO_METHODCALL_(void, Close)();
        VOODOO_METHODCALL_(void, Flush)();
        VOODOO_METHODCALL_(void, SetLogLevel)(_In_ const LogLevel level);
        VOODOO_METHODCALL_(const LogLevel, GetLogLevel)() const;
        VOODOO_METHODCALL_(void, LogModule)(_In_ const Version * const pModule);
        virtual void Log
        (
            _In_ const LogLevel level,
            _In_ const wchar_t * source,
            _In_ _Printf_format_string_ const wchar_t * format,
            ...
        );
        VOODOO_METHODCALL_(void, SetFlags)(_In_ const LogFlags flush);
        VOODOO_METHODCALL_(const LogFlags, GetFlags)() const;

    private:
        mutable uint32_t m_Refs;
        ICore * m_Core;
    };
}
