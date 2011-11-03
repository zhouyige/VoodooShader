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

#include "VSTexture.hpp"

namespace VoodooShader
{
    VSTexture::VSTexture(String name, void * pTexture) :
        m_Name(name), m_Data(pTexture)
    { }

    VSTexture::~VSTexture(void)
    { }

    uint32_t VSTexture::AddRef() const
    {
        return ++m_Refs;
    }

    uint32_t VSTexture::Release() const
    {
        if (--m_Refs == 0)
        {
            delete this;
            return 0;
        } else {
            return m_Refs;
        }
    }

    bool VSTexture::CheckedCast(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) const
    {
        if (!ppOut)
        {
            if (clsid.is_nil())
            {
                clsid = IID_ITexture;
                return true;
            } else {
                return false;
            }
        } else {
            if (clsid == IID_IObject || clsid == IID_ITexture || clsid == CLSID_VSTexture) {
                *ppOut = this;
                return true;
            } else {
                *ppOut = nullptr;
                return false;
            }
        }
    }

    String VSTexture::ToString(void) const
    {
        return String::Format(L"VSTexture(%s)", m_Name.GetData());
    }

    ICore * VSTexture::GetCore(void) const
    {
        return m_Core;
    }

    String VSTexture::GetName() const
    {
        return m_Name;
    }

    void * VSTexture::GetData(void) const
    {
        return m_Data;
    }

    const TextureDesc * VSTexture::GetDesc(void) const
    {
        return &m_Desc;
    }
}