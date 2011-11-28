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

#include "DX9_Module.hpp"

namespace VoodooShader
{
    namespace VoodooDX9
    {
        /**
         * @addtogroup voodoo_uuids 
         */
        DEFINE_CLSID(DX9Texture) = {0xC2, 0xC3, 0x4A, 0xF8, 0x3F, 0x07, 0xE1, 0x11, 0x83, 0xD4, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08};
    
        /**
         * @par CLSID
         *      f84ac3c2-073f-11e1-83d4-005056c00008
         */
        class DX9Texture :
            public ITexture
        {
        public:
            DX9Texture(_In_ ICore * pCore, _In_ String name, _In_ IDirect3DTexture9 * pTexture);
            virtual ~DX9Texture();

            VOODOO_METHODCALL_(uint32_t, AddRef)() CONST;
            VOODOO_METHODCALL_(uint32_t, Release)() CONST;
            VOODOO_METHODCALL(QueryInterface)(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST;
            VOODOO_METHODCALL_(String, ToString)() CONST;
            VOODOO_METHODCALL_(ICore *, GetCore)() CONST;

            VOODOO_METHODCALL_(String, GetName)() CONST;
            VOODOO_METHODCALL_(void *, GetData)() const;
            VOODOO_METHODCALL_(void *, SetData)(_In_ void * pData) ;
            VOODOO_METHODCALL_(const TextureDesc *, GetDesc)() const;

        private:
            mutable uint32_t m_Refs;
            ICore * m_Core;
            String m_Name;

            IDirect3DTexture9 * m_Texture;
            TextureDesc m_Desc;
        };
    }
}
