/**
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

#include "IParser.hpp"

namespace VoodooShader
{
    /* VSParser: e6f312a2-05af-11e1-9e05-005056c00008 */
    DEFINE_UUID_IMPL(VSParser, 0xA2, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08);

    class VSParser :
        public IParser
    {
    public:
        VSParser(_In_ ICore * const pCore) throw();
        virtual ~VSParser(void) throw();

        virtual uint32_t AddRef(void) const throw();
        virtual uint32_t Release(void) const throw();
        virtual bool CheckedCast(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) const throw();
        virtual String ToString(void) const throw();
        virtual ICore * GetCore(void) const throw();

        virtual void Add(_In_ const String & name, _In_ const String & value, _In_ const VariableType type = VT_Normal) throw();
        virtual void Remove(_In_ const String & name) throw();
        virtual String Parse(_In_ const String & input, _In_ const ParseFlags flags = PF_None) const throw();

        static const uint32_t VarMaxDepth    = 8;
        static const wchar_t VarDelimPre    = L'$';
        static const wchar_t VarDelimStart  = L'(';
        static const wchar_t VarDelimEnd    = L')';
        static const wchar_t VarMarkerState = L':';
        static const wchar_t VarMarkerRaw   = L'!';
        static const wchar_t VarMarkerSupp  = L'?';

    private:
        String ParseStringRaw(_In_ String input, _In_ ParseFlags flags, _In_ uint32_t depth, _In_ Dictionary & state) const;

        mutable uint32_t m_Refs;
        ICore * m_Core;

        Dictionary m_Variables;
        Dictionary m_SysVariables;
    };
}