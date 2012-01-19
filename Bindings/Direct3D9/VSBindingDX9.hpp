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
#pragma once

#include "Voodoo_D3D9.hpp"

namespace VoodooShader
{
    namespace Voodoo_D3D9
    {
        // {382CB920-83DD-4570-875F-F4E0C192A476}
        VOODOO_CLASS(VSBindingDX9, IBinding, ({0x38, 0x2c, 0xb9, 0x20, 0x83, 0xdd, 0x45, 0x70, 0x87, 0x5f, 0xf4, 0xe0, 0xc1, 0x92, 0xa4, 0x76}))
        {
        public:
            VSBindingDX9(_In_ ICore * pCore);
            ~VSBindingDX9();

            VOODOO_METHOD_(uint32_t, AddRef)() CONST;
            VOODOO_METHOD_(uint32_t, Release)() CONST;
            VOODOO_METHOD(QueryInterface)(_In_ CONST Uuid refid, _Deref_out_opt_ IObject ** ppOut);
            VOODOO_METHOD_(String, ToString)() CONST;
            VOODOO_METHOD_(ICore *, GetCore)() CONST;
            /**
             * @}
             * @name IBinding Methods
             * @{
             */
            VOODOO_METHOD(Initialize)(uint32_t count, _In_count_(count) Variant * pParams);
            /**
             * @}
             * @name Effect Methods
             * @{
             */
            VOODOO_METHOD_(IEffect *, CreateEffect)(CONST String & source);
            VOODOO_METHOD_(IEffect *, CreateEffectFromFile)(CONST String & filename);
            /**
             * @}
             * @name Parameter Methods
             * @{
             */
            VOODOO_METHOD_(IParameter *, CreateParameter)(CONST String & name, ParameterDesc desc);
            /**
             * @}
             * @name Texture Methods
             * @{
             */
            VOODOO_METHOD_(ITexture *, CreateTexture)(CONST String & name, TextureDesc desc);
            VOODOO_METHOD_(ITexture *, LoadTexture)(CONST String & filename);

        private:
            mutable uint32_t m_Refs;
            ICore * m_Core;
            LPDIRECT3DDEVICE9 m_Device;
        };
    }
}