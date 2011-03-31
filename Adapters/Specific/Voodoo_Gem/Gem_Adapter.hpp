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

#ifndef VOODOO_GEM_ADAPTER_HPP
#define VOODOO_GEM_ADAPTER_HPP

#include "Gem_Module.hpp"

namespace VoodooShader
{
    namespace Gem
    {
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_TEX1)

        struct StandardQuadVert
        {
            FLOAT x, y, z, rhw;
            FLOAT tu, tv;
        };

        class Adapter
            : public VoodooShader::IAdapter
        {

        public:
            Adapter
            (
                _In_ Core * core
            );

            ~Adapter();

            const char * GetObjectClass();

            // IAdapter methods
            bool LoadPass(Pass * pass);
            bool UnloadPass(Pass * pass);

            void BindPass(PassRef pass);
            void UnbindPass();

            void DrawShader(ShaderRef shader);

            void DrawQuad(Vertex * vertexData);

            void ApplyParameter(ParameterRef param);

            bool ConnectTexture(ParameterRef param, TextureRef texture);
            TextureRef CreateTexture(String name, TextureDesc desc);

            void HandleError(CGcontext context, CGerror error, void * core);

            /**
             * Binds a Direct3D9 device to this adapter for use in various rendering
             * calls. Without a device, the adapter has no access to graphics.
             * 
             * @param device The D3D device to bind (should be a valid device, <em>not</em>
             *        an IVoodoo3DDevice9 or any similar wrapped type).
             *        
             * @note When this method is called and a device has already been bound, all 
             *       graphics resources are unloaded and freed. If the new device is not 
             *       null, new resources are allocated and filled.
             */
            void SetDevice
            (
                _In_opt_ LPDIRECT3DDEVICE9 device
            );

        private:
            Core * mCore;
            CGcontext mCgContext;
            LPDIRECT3DDEVICE9 mDevice;

            CGprogram mBoundVP;
            CGprogram mBoundFP;

            IDirect3DSurface9 * mRenderTarget;
            
            LPDIRECT3DVERTEXBUFFER9 mQuadVerts;
        };
    };
}

#endif /*VOODOO_GEM_ADAPTER_HPP*/