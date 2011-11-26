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

#include "CVoodoo3DDevice8.hpp"

#include "CVoodoo3DSurface8.hpp"
#include "CVoodoo3DTexture8.hpp"

#include "DX9_Version.hpp"

namespace VoodooShader
{
    namespace VoodooDX8
    {
        CVoodoo3DDevice8::CVoodoo3DDevice8(IDirect3DDevice9 *realDevice) :
            m_RealDevice(realDevice)
        {
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::CVoodoo3DDevice8(%p) == %p", realDevice, this);
        }

        // IUnknown methods
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::QueryInterface(REFIID riid, void **ppvObj)
        {
            return m_RealDevice->QueryInterface(riid, ppvObj);
        }
        ULONG STDMETHODCALLTYPE CVoodoo3DDevice8::AddRef()
        {
            ULONG refCount = m_RealDevice->AddRef();

            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::AddRef() == %u", refCount);

            return refCount;
        }
        ULONG STDMETHODCALLTYPE CVoodoo3DDevice8::Release()
        {
            ULONG refCount = m_RealDevice->Release();

            if (refCount == 0)
            {
                delete this;
                return 0;
            }
            else
            {
                return refCount;
            }
        }

        // IDirect3DDevice8 methods
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::ApplyStateBlock(DWORD Token)
        {
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::ApplyStateBlock(%u) == UNUSED", Token);

            return D3DERR_INVALIDCALL;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::BeginScene()
        {
            HRESULT hr = m_RealDevice->BeginScene();

            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::BeginScene() == %d", hr);

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::BeginStateBlock()
        {
            HRESULT hr = m_RealDevice->BeginStateBlock();

            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::BeginStateBlock() == %d", hr);

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::CaptureStateBlock(DWORD Token)
        {
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::CaptureStateBlock(%u) == UNUSED", Token);

            return D3DERR_INVALIDCALL;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::Clear(DWORD Count, const D3DRECT *pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil)
        {
            HRESULT hr = m_RealDevice->Clear(Count, pRects, Flags, Color, Z, Stencil);

            gpVoodooLogger->Log
                (
                LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::Clear(%u, %p, %u, %u, %f, %u) == %d", Count, pRects,
                Flags, Color, Z, Stencil, hr
                );

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::TestCooperativeLevel()
        {
            HRESULT hr = m_RealDevice->TestCooperativeLevel();

            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::TestCooperativeLevel() == %d", hr);

            return hr;
        }

        UINT STDMETHODCALLTYPE CVoodoo3DDevice8::GetAvailableTextureMem()
        {
            UINT mem = m_RealDevice->GetAvailableTextureMem();

            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::GetAvailableTextureMem() == %u", mem);

            return mem;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::ResourceManagerDiscardBytes(DWORD Bytes)
        {
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::ResourceManagerDiscardBytes(%u) == UNUSED", Bytes);

            return D3DERR_INVALIDCALL;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetDirect3D(IDirect3D8 **ppD3D8)
        {
            (*ppD3D8) = (IDirect3D8 *)m_RealObject;

            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::GetDirect3D(%p) == D3D_OK", *ppD3D8);

            return D3D_OK;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetDeviceCaps(D3DCAPS8 *pCaps)
        {
            D3DCAPS9 rCaps;

            HRESULT hr = m_RealDevice->GetDeviceCaps(&rCaps);

            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::GetDeviceCaps(%p) == %d", pCaps, hr);

            if (SUCCEEDED(hr))
            {
                memcpy(pCaps, &rCaps, sizeof(D3DCAPS8));
            }
            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetDisplayMode(D3DDISPLAYMODE *pMode)
        {
            HRESULT hr = m_RealDevice->GetDisplayMode(0, pMode);

            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::GetDisplayMode(%p) == %d", pMode, hr);

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS *pParameters)
        {
            HRESULT hr = m_RealDevice->GetCreationParameters(pParameters);

            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::GetCreationParameters(%p) == %d", pParameters, hr);

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface8 *pCursorBitmap)
        {
            CVoodoo3DSurface8 *rCursor = (CVoodoo3DSurface8 *) pCursorBitmap;

            HRESULT hr = m_RealDevice->SetCursorProperties(XHotSpot, YHotSpot, rCursor->RealSurface());

            gpVoodooLogger->Log
                (
                LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::SetCursorProperties(%u, %u, %p) == %d", XHotSpot, YHotSpot,
                pCursorBitmap, hr
                );

            return hr;
        }

        void STDMETHODCALLTYPE CVoodoo3DDevice8::SetCursorPosition(int X, int Y, DWORD Flags)
        {
            m_RealDevice->SetCursorPosition(X, Y, Flags);

            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::SetCursorPosition(%i, %i, %u)", X, Y, Flags);
        }

        BOOL STDMETHODCALLTYPE CVoodoo3DDevice8::ShowCursor(BOOL bShow)
        {
            BOOL show = m_RealDevice->ShowCursor(bShow);

            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::ShowCursor(%i) == %d", bShow, show);

            return show;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::CreateAdditionalSwapChain
            (
            D3DPRESENT_PARAMETERS8  *pPresentationParameters,
            IDirect3DSwapChain8 **pSwapChain
            )
        {
            HRESULT hr = m_RealDevice->CreateAdditionalSwapChain
                ((D3DPRESENT_PARAMETERS *) pPresentationParameters, (IDirect3DSwapChain9 **) pSwapChain);

            gpVoodooLogger->Log
                (
                LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::CreateAdditionalSwapChain(%p, %p) == %d",
                pPresentationParameters, *pSwapChain, hr
                );

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::Reset(D3DPRESENT_PARAMETERS8 *pPresentationParameters)
        {
            HRESULT hr = m_RealDevice->Reset((D3DPRESENT_PARAMETERS *) pPresentationParameters);

            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::Reset(%p) == %d", pPresentationParameters, hr);

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::Present
        (
            CONST RECT *pSourceRect,
            CONST RECT   *pDestRect,
            HWND hDestWindowOverride,
            CONST RGNDATA *pDirtyRegion
        )
        {
            HRESULT hr = m_RealDevice->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);

            gpVoodooLogger->Log
                (
                LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::Present(%p, %p, %p, %p) == %d", pSourceRect, pDestRect,
                hDestWindowOverride, pDirtyRegion, hr
                );

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetBackBuffer(UINT BackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface8 **ppBackBuffer)
        {
            IDirect3DSurface9 *rBackbuffer;
            HRESULT hr = m_RealDevice->GetBackBuffer(0, BackBuffer, Type, &rBackbuffer);

            gpVoodooLogger->Log
                (
                LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::GetBackBuffer(%u, %u, %p) == %d", BackBuffer, Type, *
                ppBackBuffer, hr
                );

            if (SUCCEEDED(hr))
            {
                CVoodoo3DSurface8 *wBackbuffer = new CVoodoo3DSurface8(this, rBackbuffer);
                (*ppBackBuffer) = (IDirect3DSurface8 *) wBackbuffer;
            }
            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetRasterStatus(D3DRASTER_STATUS *pRasterStatus)
        {
            HRESULT hr = m_RealDevice->GetRasterStatus(0, pRasterStatus);

            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::GetRasterStatus(%p) == %d", pRasterStatus, hr);

            return hr;
        }

        void STDMETHODCALLTYPE CVoodoo3DDevice8::SetGammaRamp(DWORD Flags, const D3DGAMMARAMP *pRamp)
        {
            m_RealDevice->SetGammaRamp(0, Flags, pRamp);

            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::GetRasterStatus(%u, %p) == void", Flags, pRamp);

            return;
        }

        void STDMETHODCALLTYPE CVoodoo3DDevice8::GetGammaRamp(D3DGAMMARAMP *pRamp)
        {
            m_RealDevice->GetGammaRamp(0, pRamp);

            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::GetRasterStatus(%p) == void", pRamp);

            return;
        }

        /**
        * Creates a texture in the IVoodoo runtime. The new texture must be registered
        * with the core and, for shader purposes, we should attempt to create it as a
        * render-target.
        */
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::CreateTexture
        (
            UINT Width,
            UINT Height,
            UINT Levels,
            DWORD Usage,
            D3DFORMAT Format,
            D3DPOOL Pool,
            IDirect3DTexture8 **ppTexture
        )
        {
            IDirect3DTexture9 *rTexture;
            HRESULT hr = D3DERR_INVALIDCALL;

            // If not a render-target, attempt to create it as one
            BOOL rtt = true;

            if (!(Usage & D3DUSAGE_RENDERTARGET))
            {
                hr = m_RealDevice->CreateTexture(Width, Height, Levels, Usage | D3DUSAGE_RENDERTARGET, Format, Pool, &rTexture, nullptr);
            }

            // That failed, create as a standard texture
            if (FAILED(hr))
            {
                rtt = false;
                hr = m_RealDevice->CreateTexture(Width, Height, Levels, Usage, Format, Pool, &rTexture, nullptr);
            }
            gpVoodooLogger->Log
                (
                LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::CreateTexture(%u, %u, %u, %u, %u, %u, %p) == %d", Width,
                Height, Levels, Usage, Format, Pool, *ppTexture, hr
                );

            // One succeeded, the texture exists. We need to register it with the Voodoo core.
            if (SUCCEEDED(hr))
            {

                // ! @todo Set up the core texture registration for Voodoo DX8.9 and track
                // filenames somehow
                CVoodoo3DTexture8 *wTexture = new CVoodoo3DTexture8(this, rTexture);
                (*ppTexture) = (IDirect3DTexture8 *) wTexture;
            }
            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::CreateVolumeTexture
            (
            UINT Width,
            UINT Height,
            UINT Depth,
            UINT Levels,
            DWORD Usage,
            D3DFORMAT Format,
            D3DPOOL Pool,
            IDirect3DVolumeTexture8 **ppVolumeTexture
            )
        {
            IDirect3DVolumeTexture9 *rTexture;
            HRESULT hr = m_RealDevice->CreateVolumeTexture
                (Width, Height, Depth, Levels, Usage, Format, Pool, &rTexture, nullptr);

            gpVoodooLogger->Log
                (
                LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::CreateVolumeTexture(%u, %u, %u, %u, %u, %u, %u, %p) == %d",
                Width, Height, Depth, Levels, Usage, Format, Pool, *ppVolumeTexture, hr
                );

            // ! @todo Set up the core volume texture registration for Voodoo DX8.9
            if (SUCCEEDED(hr))
            {
                CVoodoo3DTexture8 *wTexture = new CVoodoo3DTexture8(this, (IDirect3DTexture9 *) rTexture);
                (*ppVolumeTexture) = (IDirect3DCubeTexture8 *) wTexture;
            }
            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::CreateCubeTexture
            (
            UINT EdgeLength,
            UINT Levels,
            DWORD Usage,
            D3DFORMAT Format,
            D3DPOOL Pool,
            IDirect3DCubeTexture8 **ppCubeTexture
            )
        {
            IDirect3DCubeTexture9 *rTexture;
            HRESULT hr = m_RealDevice->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, &rTexture, nullptr);

            gpVoodooLogger->Log
                (
                LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::CreateCubeTexture(%u, %u, %u, %u, %u, %p) == %d",
                EdgeLength, Levels, Usage, Format, Pool, *ppCubeTexture, hr
                );

            if (SUCCEEDED(hr))
            {
                CVoodoo3DTexture8 *wTexture = new CVoodoo3DTexture8(this, (IDirect3DTexture9 *) rTexture);
                (*ppCubeTexture) = (IDirect3DCubeTexture8 *) wTexture;
            }
            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::CreateVertexBuffer
            (
            UINT Length,
            DWORD Usage,
            DWORD FVF,
            D3DPOOL Pool,
            IDirect3DVertexBuffer8 **ppVertexBuffer
            )
        {
            HRESULT hr = m_RealDevice->CreateVertexBuffer
                (Length, Usage, FVF, Pool, (IDirect3DVertexBuffer9 **) ppVertexBuffer, nullptr);

            gpVoodooLogger->Log
                (
                LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::CreateVertexBuffer(%u, %u, %u, %u, %p) == %d", Length,
                Usage, FVF, Pool, *ppVertexBuffer, hr
                );

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::CreateIndexBuffer
            (
            UINT Length,
            DWORD Usage,
            D3DFORMAT Format,
            D3DPOOL Pool,
            IDirect3DIndexBuffer8 **ppIndexBuffer
            )
        {
            HRESULT hr = m_RealDevice->CreateIndexBuffer
                (Length, Usage, Format, Pool, (IDirect3DIndexBuffer9 **) ppIndexBuffer, nullptr);

            gpVoodooLogger->Log
                (
                LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::CreateIndexBuffer(%u, %u, %u, %u, %p) == %d", Length, Usage,
                Format, Pool, *ppIndexBuffer, hr
                );

            return hr;
        }

        /**
        * This function creates a stand-alone render target surface in the original D3D8
        * and 9 setups. For Voodoo's purposes, this should create a render target texture
        * if at all possible, then retrieve the surface from that texture. @todo Find a
        * way to force CreateRenderTarget to make RT textures and find a way to name
        * them.
        */
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::CreateRenderTarget
            (
            UINT Width,
            UINT Height,
            D3DFORMAT Format,
            D3DMULTISAMPLE_TYPE MultiSample,
            BOOL Lockable,
            IDirect3DSurface8 **ppSurface
            )
        {
            IDirect3DSurface9 *rSurface;
            HRESULT hr = m_RealDevice->CreateRenderTarget(Width, Height, Format, MultiSample, 0, Lockable, &rSurface, nullptr);

            gpVoodooLogger->Log
                (
                LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::CreateRenderTarget(%u, %u, %u, %u, %u, %p) == %d", Width,
                Height, Format, MultiSample, Lockable, *ppSurface, hr
                );

            if (SUCCEEDED(hr))
            {
                CVoodoo3DSurface8 *wSurface = new CVoodoo3DSurface8(this, rSurface);
                (*ppSurface) = (IDirect3DSurface8 *) wSurface;
            }
            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::CreateDepthStencilSurface
            (
            UINT Width,
            UINT Height,
            D3DFORMAT Format,
            D3DMULTISAMPLE_TYPE MultiSample,
            IDirect3DSurface8 **ppSurface
            )
        {
            IDirect3DSurface9 *rSurface;

            HRESULT hr = m_RealDevice->CreateDepthStencilSurface(Width, Height, Format, MultiSample, 0, 0, &rSurface, nullptr);

            gpVoodooLogger->Log
                (
                LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::CreateDepthStencilSurface(%u, %u, %u, %u, %p) == %d", Width,
                Height, Format, MultiSample, *ppSurface, hr
                );

            if (SUCCEEDED(hr))
            {
                CVoodoo3DSurface8 *wSurface = new CVoodoo3DSurface8(this, rSurface);
                (*ppSurface) = (IDirect3DSurface8 *) wSurface;
            }
            return hr;
        }

        /**
        * Creates a D3D8 image surface, a sort of placeholder surface. Because D3D9 has
        * no equivalent, this is redirected to CreateOffscreenPlainSurface at some point.
        * @warning This uses a different call than the D3D8 original, and may not perform
        * exactly the same. <em>(non-standard behavior)</em> @todo Test the pool these
        * should be created in. May be default, managed or sysmem, not sure yet.
        */
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::CreateImageSurface(UINT Width, UINT Height, D3DFORMAT Format, IDirect3DSurface8 **ppSurface)
        {
            IDirect3DSurface9 *rSurface;

            HRESULT hr = m_RealDevice->CreateOffscreenPlainSurface(Width, Height, Format, D3DPOOL_DEFAULT, &rSurface, nullptr);

            gpVoodooLogger->Log
                (
                LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::CreateImageSurface(%u, %u, %u, %p) == %d", Width, Height,
                Format, *ppSurface, hr
                );

            if (SUCCEEDED(hr))
            {
                CVoodoo3DSurface8 *wSurface = new CVoodoo3DSurface8(this, rSurface);
                (*ppSurface) = (IDirect3DSurface8 *) wSurface;
            }
            return hr;
        }

        /**
        * Copies rectangular areas of data from one surface to another. There is no such
        * function in D3D9, it instead requires each texture to be stretched separately.
        * It is, however, possible to translate the call as D3D8 applies some very
        * specific restrictions, including a lack of stretching (means the rectangles
        * must be the same dimensions, but at different points) and the same format and
        * surface properties. @warning For testing purposes, this only copies the first
        * given rectangle. This is easy enough to fix, but I want to do more testing
        * first. <em>(non-standard behavior)</em> @todo Test and then make this perform
        * identical to the D3D8 version.
        */
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::CopyRects
            (
            IDirect3DSurface8  *pSourceSurface,
            CONST RECT   *pSourceRectsArray,
            UINT cRects,
            IDirect3DSurface8 *pDestinationSurface,
            CONST POINT *pDestPointsArray
            )
        {
            CVoodoo3DSurface8 *wSource = (CVoodoo3DSurface8 *) pSourceSurface;
            CVoodoo3DSurface8 *wDest = (CVoodoo3DSurface8 *) pDestinationSurface;

            HRESULT hr = m_RealDevice->StretchRect
                (wSource->RealSurface(), pSourceRectsArray, wDest->RealSurface(), pSourceRectsArray, D3DTEXF_NONE);

            gpVoodooLogger->Log
                (
                LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::CopyRects(%p, %p, %u, %p, %p) == %d", pSourceSurface,
                pSourceRectsArray, cRects, pDestinationSurface, pDestPointsArray, hr
                );

            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::UpdateTexture(IDirect3DBaseTexture8 *pSourceTexture, IDirect3DBaseTexture8 *pDestinationTexture)
        {
            CVoodoo3DTexture8 *wSource = (CVoodoo3DTexture8 *) pSourceTexture;
            CVoodoo3DTexture8 *wDest = (CVoodoo3DTexture8 *) pDestinationTexture;

            HRESULT hr = m_RealDevice->UpdateTexture(wSource->RealTexture(), wDest->RealTexture());

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::UpdateTexture(%d, %d) == %d", pSourceTexture, pDestinationTexture, hr);
#endif
            return hr;
        }

        /**
        * Retrieves the front-buffer surface from the D3D8 device. D3D9 has no equivalent
        * call (kinda).
        */
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetFrontBuffer(IDirect3DSurface8 *pDestSurface)
        {
#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::GetFrontBuffer(%d) == UNUSED", pDestSurface);
#endif
            return D3DERR_INVALIDCALL;
        }

        /**
        * Sets a new render-target surface and depth/stencil buffer. D3D9 does not
        * combine these calls, so this automatically splits the calls and returns the
        * combined results.
        */
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::SetRenderTarget(IDirect3DSurface8 *pRenderTarget, IDirect3DSurface8  *pNewZStencil)
        {
            CVoodoo3DSurface8 *wRender = (CVoodoo3DSurface8 *) pRenderTarget;
            CVoodoo3DSurface8 *wStencil = (CVoodoo3DSurface8 *) pNewZStencil;

            HRESULT hr = m_RealDevice->SetRenderTarget(0, wRender->RealSurface());
            hr |= m_RealDevice->SetDepthStencilSurface(wStencil->RealSurface());

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::SetRenderTarget(%d, %d) == %d", pRenderTarget, pNewZStencil, hr);
#endif
            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetRenderTarget(IDirect3DSurface8 **ppRenderTarget)
        {
            IDirect3DSurface9 *rRenderTarget;
            HRESULT hr = m_RealDevice->GetRenderTarget(0, &rRenderTarget);

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::GetRenderTarget(%d) == %d", *ppRenderTarget, hr);
#endif
            if (SUCCEEDED(hr))
            {
                CVoodoo3DSurface8 *wRenderTarget = new CVoodoo3DSurface8(this, rRenderTarget);
                (*ppRenderTarget) = (IDirect3DSurface8 *) wRenderTarget;
            }
            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetDepthStencilSurface(IDirect3DSurface8 **ppZStencilSurface)
        {
            IDirect3DSurface9 *rZStencilSurface;
            HRESULT hr = m_RealDevice->GetDepthStencilSurface(&rZStencilSurface);

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::GetDepthStencilSurface(%d) == %d", *ppZStencilSurface, hr);
#endif
            if (SUCCEEDED(hr))
            {
                CVoodoo3DSurface8 *wZStencilSurface = new CVoodoo3DSurface8(this, rZStencilSurface);
                (*ppZStencilSurface) = (IDirect3DSurface8 *) wZStencilSurface;
            }
            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::EndScene()
        {
            HRESULT hr = m_RealDevice->EndScene();

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::EndScene() == %d", hr);
#endif
            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX *pMatrix)
        {
            HRESULT hr = m_RealDevice->SetTransform(State, pMatrix);

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::SetTransform(%d, %d) == %d", State, pMatrix, hr);
#endif
            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX *pMatrix)
        {
            HRESULT hr = m_RealDevice->GetTransform(State, pMatrix);

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::GetTransform(%d, %d) == %d", State, pMatrix, hr);
#endif
            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::MultiplyTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX *pMatrix)
        {
            HRESULT hr = m_RealDevice->MultiplyTransform(State, pMatrix);

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::MultiplyTransform(%d, %d) == %d", State, pMatrix, hr);
#endif
            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::SetViewport(CONST D3DVIEWPORT8 *pViewport)
        {
            HRESULT hr = m_RealDevice->SetViewport((CONST D3DVIEWPORT9 *) pViewport);

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::SetViewport(%d) == %d", pViewport, hr);
#endif
            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetViewport(D3DVIEWPORT8 *pViewport)
        {
            HRESULT hr = m_RealDevice->GetViewport((D3DVIEWPORT9 *) pViewport);

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::GetViewport(%d) == %d", pViewport, hr);
#endif
            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::SetMaterial(CONST D3DMATERIAL8 *pMaterial)
        {
            HRESULT hr = m_RealDevice->SetMaterial((CONST D3DMATERIAL9 *) pMaterial);

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::SetMaterial(%d) == %d", pMaterial, hr);
#endif
            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetMaterial(D3DMATERIAL8 *pMaterial)
        {
            HRESULT hr = m_RealDevice->GetMaterial((D3DMATERIAL9 *) pMaterial);

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::GetMaterial(%d) == %d", pMaterial, hr);
#endif
            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::SetLight(DWORD Index, CONST D3DLIGHT8 *Light)
        {
            HRESULT hr = m_RealDevice->SetLight(Index, (CONST D3DLIGHT9 *) Light);

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::SetLight(%d, %d) == %d", Index, Light, hr);
#endif
            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetLight(DWORD Index, D3DLIGHT8 *Light)
        {
            HRESULT hr = m_RealDevice->GetLight(Index, (D3DLIGHT9 *) Light);

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::GetLight(%d, %d) == %d", Index, Light, hr);
#endif
            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::LightEnable(DWORD Index, BOOL Enable)
        {
            HRESULT hr = m_RealDevice->LightEnable(Index, Enable);

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::LightEnable(%d, %d) == %d", Index, Enable, hr);
#endif
            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetLightEnable(DWORD Index, BOOL *pEnable)
        {
            HRESULT hr = m_RealDevice->GetLightEnable(Index, pEnable);

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::GetLightEnable(%d, %d) == %d", Index, pEnable, hr);
#endif
            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::SetClipPlane(DWORD Index, CONST float *pPlane)
        {
            HRESULT hr = m_RealDevice->SetClipPlane(Index, pPlane);

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::SetClipPlane(%d, %d) == %d", Index, pPlane, hr);
#endif
            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetClipPlane(DWORD Index, float *pPlane)
        {
            HRESULT hr = m_RealDevice->GetClipPlane(Index, pPlane);

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::GetClipPlane(%d, %d) == %d", Index, pPlane, hr);
#endif
            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::SetRenderState(D3DRENDERSTATETYPE State, DWORD Value)
        {
            HRESULT hr = m_RealDevice->SetRenderState(State, Value);

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::SetRenderState(%d, %d) == %d", State, Value, hr);
#endif
            return hr;
        }

        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetRenderState(D3DRENDERSTATETYPE State, DWORD *pValue)
        {
            HRESULT hr = m_RealDevice->GetRenderState(State, pValue);

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::GetRenderState(%d, %d) == %d", State, pValue, hr);
#endif
            return hr;
        }

        /**
         * Due to differences between the D3D8 and D3D9 APIs, this function is currently non-functional.
         */
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::EndStateBlock(DWORD *pToken)
        {
#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::EndStateBlock(%d) == UNUSED", pToken);
#endif
            return D3DERR_INVALIDCALL;

            /**
            * HRESULT hr = m_RealDevice->EndStateBlock(pToken);
            * #ifdef _DEBUG gpVoodooLogger->Log("Voodoo DX8.9:
            * CVoodoo3DDevice8::EndStateBlock(%d) == %d", pToken, hr);
            * #endif return hr;
            */
        }

        /* This function does not exist in the D3D9 API. */
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::DeleteStateBlock(DWORD Token)
        {
#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::DeleteStateBlock(%d) == UNUSED", Token);
#endif
            return D3DERR_INVALIDCALL;

            /**
            * HRESULT hr = m_RealDevice->DeleteStateBlock(Token);
            * #ifdef _DEBUG gpVoodooLogger->Log("Voodoo DX8.9:
            * CVoodoo3DDevice8::DeleteStateBlock(%d) == %d", Token, hr);
            * #endif return hr;
            */
        }

        /**
        * Due to differences between the D3D8 and D3D9 APIs, this function is currently
        * non- functional
        */
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::CreateStateBlock(D3DSTATEBLOCKTYPE Type, DWORD *pToken)
        {
#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::CreateStateBlock(%d, %d) == UNUSED", Type, pToken);
#endif
            return D3DERR_INVALIDCALL;

            /**
            * HRESULT hr = m_RealDevice->CreateStateBlock(Type, pToken);
            * #ifdef _DEBUG gpVoodooLogger->Log("Voodoo DX8.9:
            * CVoodoo3DDevice8::CreateStateBlock(%d, %d) == %d", Type, pToken, hr);
            * #endif return hr;
            */
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::SetClipStatus(CONST D3DCLIPSTATUS8 *pClipStatus)
        {
            HRESULT hr = m_RealDevice->SetClipStatus((CONST D3DCLIPSTATUS9 *) pClipStatus);

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::SetClipStatus(%d) == %d", pClipStatus, hr);
#endif
            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetClipStatus(D3DCLIPSTATUS8 *pClipStatus)
        {
            HRESULT hr = m_RealDevice->GetClipStatus((D3DCLIPSTATUS9 *) pClipStatus);

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::GetClipStatus(%d) == %d", pClipStatus, hr);
#endif
            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetTexture(DWORD Stage, IDirect3DBaseTexture8 **ppTexture)
        {
            IDirect3DBaseTexture9 *rTexture;
            HRESULT hr = m_RealDevice->GetTexture(Stage, &rTexture);

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::GetTexture(%d, %d) == %d", Stage, *ppTexture, hr);
#endif
            if (SUCCEEDED(hr))
            {
                CVoodoo3DTexture8 *wTexture = new CVoodoo3DTexture8(this, (IDirect3DTexture9 *) rTexture);
                (*ppTexture) = (IDirect3DBaseTexture8 *) wTexture;
            }
            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::SetTexture(DWORD Stage, IDirect3DBaseTexture8 *pTexture)
        {
            HRESULT hr = D3D_OK;

            if (pTexture)
            {
                CVoodoo3DTexture8 *wTexture = (CVoodoo3DTexture8 *) pTexture;
                hr = m_RealDevice->SetTexture(Stage, (IDirect3DBaseTexture9 *) wTexture->RealTexture());
            }
            else
            {
                hr = m_RealDevice->SetTexture(Stage, nullptr);
            }
#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::SetTexture(%d, %d) == %d", Stage, pTexture, hr);
#endif
            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD *pValue)
        {
            HRESULT hr = m_RealDevice->GetTextureStageState(Stage, Type, pValue);

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::GetTextureStageState(%d, %d, %d) == %d", Stage, Type, pValue, hr);
#endif
            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value)
        {
            HRESULT hr = m_RealDevice->SetTextureStageState(Stage, Type, Value);

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::SetTextureStageState(%d, %d, %d) == %d", Stage, Type, Value, hr);
#endif
            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::ValidateDevice(DWORD *pNumPasses)
        {
            HRESULT hr = m_RealDevice->ValidateDevice(pNumPasses);

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::ValidateDevice(%d) == %d", *pNumPasses, hr);
#endif
            return hr;
        }

        /* No D3D9 equivalent. */
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetInfo(DWORD DevInfoID, void *pDevInfoStruct, DWORD DevInfoStructSize)
        {

            // HRESULT hr = m_RealDevice->GetInfo(DevInfoID, pDevInfoStruct, DevInfoStructSize);
#ifdef _DEBUG
            gpVoodooLogger->Log
                (
                LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::GetInfo(%d, %d, %d) == UNUSED", DevInfoID, pDevInfoStruct,
                DevInfoStructSize
                );
#endif
            return D3DERR_INVALIDCALL;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::SetPaletteEntries(UINT PaletteNumber, CONST PALETTEENTRY *pEntries)
        {
            HRESULT hr = m_RealDevice->SetPaletteEntries(PaletteNumber, pEntries);

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::SetPaletteEntries(%d, %d) == %d", PaletteNumber, pEntries, hr);
#endif
            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY *pEntries)
        {
            HRESULT hr = m_RealDevice->GetPaletteEntries(PaletteNumber, pEntries);

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::GetPaletteEntries(%d, %d) == %d", PaletteNumber, pEntries, hr);
#endif
            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::SetCurrentTexturePalette(UINT PaletteNumber)
        {
            HRESULT hr = m_RealDevice->SetCurrentTexturePalette(PaletteNumber);

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::SetCurrentTexturePalette(%d) == %d", PaletteNumber, hr);
#endif
            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetCurrentTexturePalette(UINT *PaletteNumber)
        {
            HRESULT hr = m_RealDevice->GetCurrentTexturePalette(PaletteNumber);

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::GetCurrentTexturePalette(%d) == %d", *PaletteNumber, hr);
#endif
            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
        {
            HRESULT hr = m_RealDevice->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);

#ifdef _DEBUG
            gpVoodooLogger->Log
                (
                LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::DrawPrimitive(%d, %d, %d) == %d", PrimitiveType, StartVertex,
                PrimitiveCount, hr
                );
#endif
            return hr;
        }

        // ! @todo Check the value of the start index in the D3D9 call.
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::DrawIndexedPrimitive
            (
            D3DPRIMITIVETYPE PrimitiveType,
            UINT minIndex,
            UINT NumVertices,
            UINT startIndex,
            UINT primCount
            )
        {
            HRESULT hr = m_RealDevice->DrawIndexedPrimitive
                (PrimitiveType, m_LastBaseIndex, minIndex, NumVertices, startIndex, primCount);

#ifdef _DEBUG
            gpVoodooLogger->Log
                (
                LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::DrawIndexedPrimitive(%d, %d, %d, %d, %d) == %d", PrimitiveType,
                minIndex, NumVertices, startIndex, primCount, hr
                );
#endif
            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::DrawPrimitiveUP
            (
            D3DPRIMITIVETYPE PrimitiveType,
            UINT PrimitiveCount,
            CONST void *pVertexStreamZeroData,
            UINT VertexStreamZeroStride
            )
        {
            HRESULT hr = m_RealDevice->DrawPrimitiveUP
                (PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);

#ifdef _DEBUG
            gpVoodooLogger->Log
                (
                LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::DrawPrimitiveUP(%d, %d, %d, %d) == %d", PrimitiveType, PrimitiveCount,
                pVertexStreamZeroData, VertexStreamZeroStride, hr
                );
#endif
            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::DrawIndexedPrimitiveUP
            (
            D3DPRIMITIVETYPE PrimitiveType,
            UINT MinVertexIndex,
            UINT NumVertexIndices,
            UINT PrimitiveCount,
            CONST void *pIndexData,
            D3DFORMAT IndexDataFormat,
            CONST void *pVertexStreamZeroData,
            UINT VertexStreamZeroStride
            )
        {
            HRESULT hr = m_RealDevice->DrawIndexedPrimitiveUP
                (
                PrimitiveType, MinVertexIndex, NumVertexIndices, PrimitiveCount, pIndexData, IndexDataFormat,
                pVertexStreamZeroData, VertexStreamZeroStride
                );

#ifdef _DEBUG
            gpVoodooLogger->Log
                (
                LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::DrawIndexedPrimitiveUP(%d, %d, %d, %d, %d, %d, %d, %d) == %d",
                PrimitiveType, MinVertexIndex, NumVertexIndices, PrimitiveCount, pIndexData, IndexDataFormat,
                pVertexStreamZeroData, VertexStreamZeroStride, hr
                );
#endif
            return hr;
        }

        // ! @todo Check the parameter of the vertex decl in the D3D9 call. Taken from MGE, so...
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::ProcessVertices
            (
            UINT SrcStartIndex,
            UINT DestIndex,
            UINT VertexCount,
            IDirect3DVertexBuffer8 *pDestBuffer,
            DWORD Flags
            )
        {
            HRESULT hr = m_RealDevice->ProcessVertices
                (SrcStartIndex, DestIndex, VertexCount, (IDirect3DVertexBuffer9 *) pDestBuffer, nullptr, Flags);

#ifdef _DEBUG
            gpVoodooLogger->Log
                (
                LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::ProcessVertices(%d, %d, %d, %d, %d) == %d", SrcStartIndex, DestIndex,
                VertexCount, pDestBuffer, Flags, hr
                );
#endif
            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::CreateVertexShader(CONST DWORD *pDeclaration, CONST DWORD *pFunction, DWORD  *pHandle, DWORD Usage)
        {

            // ! @todo Set this up to keep a running log of shaders (probably a !
            // std::set<IDirect3DVertexShader9**>).
            HRESULT hr = m_RealDevice->CreateVertexShader(pFunction, (IDirect3DVertexShader9 **) pHandle);

#ifdef _DEBUG
            gpVoodooLogger->Log
                (
                LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::CreateVertexShader(%d, %d, %d, %d) == %d", pDeclaration, pFunction,
                pHandle, Usage, hr
                );
#endif
            if (SUCCEEDED(hr))
            {
                m_VertexShaders.insert(*pHandle);
            }
            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::SetVertexShader(DWORD Handle)
        {

            // ! @todo Test the shader setup
            HRESULT hr = D3D_OK;

            if (m_VertexShaders.find(Handle) == m_VertexShaders.end())
            {

                // Not a shader, FVF
                hr = m_RealDevice->SetFVF(Handle);
            }
            else
            {
                hr = m_RealDevice->SetVertexShader((IDirect3DVertexShader9 *) Handle);
            }
#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::SetVertexShader(%d) == %d", Handle, hr);
#endif
            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetVertexShader(DWORD *pHandle)
        {

            // HRESULT hr = m_RealDevice->GetVertexShader((IDirect3DVertexShader9**)pHandle);
            (*pHandle) = m_CurrentVertexShader;

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::GetVertexShader(%d) == D3D_OK", pHandle);
#endif
            return D3D_OK;
        }

        /* D3D9 has no equivalent function. */
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::DeleteVertexShader(DWORD Handle)
        {
            HRESULT hr = D3D_OK; // m_RealDevice->DeleteVertexShader(DWORD
            ///Handle);
            ///
            std::set<DWORD>::iterator vShader = m_VertexShaders.find(Handle);
            if (vShader != m_VertexShaders.end())
            {
                m_VertexShaders.erase(vShader);
            }
            else
            {
                hr = D3DERR_INVALIDCALL;
            }
#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::DeleteVertexShader(%d) == %d", Handle, hr);
#endif
            return D3D_OK;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::SetVertexShaderConstant(DWORD Register, CONST void *pConstantData, DWORD ConstantCount)
        {
            HRESULT hr = m_RealDevice->SetVertexShaderConstantF(Register, (const float *) pConstantData, ConstantCount);

#ifdef _DEBUG
            gpVoodooLogger->Log
                (
                LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::SetVertexShaderConstant(%d, %d, %d) == %d", Register, pConstantData,
                ConstantCount, hr
                );
#endif
            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetVertexShaderConstant(DWORD Register, void *pConstantData, DWORD ConstantCount)
        {
            HRESULT hr = m_RealDevice->GetVertexShaderConstantF(Register, (float *) pConstantData, ConstantCount);

#ifdef _DEBUG
            gpVoodooLogger->Log
                (
                LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::GetVertexShaderConstant(%d, %d, %d) == %d", Register, pConstantData,
                ConstantCount, hr
                );
#endif
            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetVertexShaderDeclaration(DWORD Handle, void *pData, DWORD *pSizeOfData)
        {
#ifdef _DEBUG
            gpVoodooLogger->Log
                (
                LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::GetVertexShaderDeclaration(%d, %d, %d) == UNUSED", Handle, pData,
                pSizeOfData
                );
#endif
            return D3DERR_INVALIDCALL;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetVertexShaderFunction(DWORD Handle, void *pData, DWORD *pSizeOfData)
        {

            // HRESULT hr = m_RealDevice->GetVertexShaderFunction(Handle, pData, pSizeOfData);
#ifdef _DEBUG
            gpVoodooLogger->Log
                (
                LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::GetVertexShaderFunction(%d, %d, %d) == UNUSED", Handle, pData,
                pSizeOfData
                );
#endif
            return D3DERR_INVALIDCALL;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::SetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer8 *pStreamData, UINT Stride)
        {
            HRESULT hr = m_RealDevice->SetStreamSource(StreamNumber, (IDirect3DVertexBuffer9 *) pStreamData, 0, Stride);

#ifdef _DEBUG
            gpVoodooLogger->Log
                (
                LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::SetStreamSource(%d, %d, %d) == %d", StreamNumber, pStreamData, Stride,
                hr
                );
#endif
            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer8 **ppStreamData, UINT *pStride)
        {
            UINT offset;
            HRESULT hr = m_RealDevice->GetStreamSource
                (StreamNumber, (IDirect3DVertexBuffer9 **) ppStreamData, &offset, pStride);

#ifdef _DEBUG
            gpVoodooLogger->Log
                (
                LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::GetStreamSource(%d, %d, %d) == %d", StreamNumber, *ppStreamData, *
                pStride, hr
                );
#endif
            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::SetIndices(IDirect3DIndexBuffer8 *pIndexData, UINT BaseVertexIndex)
        {
            HRESULT hr = m_RealDevice->SetIndices((IDirect3DIndexBuffer9 *) pIndexData);
            m_LastBaseIndex = BaseVertexIndex;

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::SetIndices(%d, %d) == %d", pIndexData, BaseVertexIndex, hr);
#endif
            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetIndices(IDirect3DIndexBuffer8 **ppIndexData, UINT   *pBaseVertexIndex)
        {
            HRESULT hr = m_RealDevice->GetIndices((IDirect3DIndexBuffer9 **) ppIndexData);
            (*pBaseVertexIndex) = m_LastBaseIndex;

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::GetIndices(%d, %d) == %d", *ppIndexData, *pBaseVertexIndex, hr);
#endif
            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::CreatePixelShader(CONST DWORD *pFunction, DWORD  *pHandle)
        {
            HRESULT hr = m_RealDevice->CreatePixelShader(pFunction, (IDirect3DPixelShader9 **) pHandle);

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::CreatePixelShader(%d, %d) == %d", pFunction, *pHandle, hr);
#endif
            if (SUCCEEDED(hr))
            {
                m_PixelShaders.insert(*pHandle);
            }
            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::SetPixelShader(DWORD Handle)
        {

            // ! @todo Test the shader setup
            HRESULT hr = D3D_OK;

            if (m_PixelShaders.find(Handle) == m_PixelShaders.end())
            {

                // ! @todo Check if this is write, D3D8 docs have nothing
                hr = m_RealDevice->SetPixelShader(nullptr);
            }
            else
            {
                hr = m_RealDevice->SetPixelShader((IDirect3DPixelShader9 *) Handle);
            }
#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::SetPixelShader(%d) == %d", Handle, hr);
#endif
            if (SUCCEEDED(hr))
            {
                m_CurrentPixelShader = Handle;
            }
            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetPixelShader(DWORD *pHandle)
        {
            (*pHandle) = m_CurrentPixelShader;

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::GetPixelShader(%d) == D3D_OK", *pHandle);
#endif
            return D3D_OK;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::DeletePixelShader(DWORD Handle)
        {
            HRESULT hr = D3D_OK; // m_RealDevice->DeletePixelShader(DWORD
            ///Handle);
            ///
            std::set<DWORD>::iterator pShader = m_PixelShaders.find(Handle);
            if (pShader != m_PixelShaders.end())
            {
                m_PixelShaders.erase(pShader);
            }
            else
            {

                // hr = D3DERR_INVALIDCALL;
            }
#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::DeletePixelShader(%d) == %d", Handle, hr);
#endif
            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::SetPixelShaderConstant(DWORD Register, CONST void *pConstantData, DWORD ConstantCount)
        {
            HRESULT hr = m_RealDevice->SetPixelShaderConstantF(Register, (float *) pConstantData, ConstantCount);

#ifdef _DEBUG
            gpVoodooLogger->Log
                (
                LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::SetPixelShaderConstant(%d, %d, %d) == %d", Register, pConstantData,
                ConstantCount, hr
                );
#endif
            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetPixelShaderConstant(DWORD Register, void *pConstantData, DWORD ConstantCount)
        {
            HRESULT hr = m_RealDevice->GetPixelShaderConstantF(Register, (float *) pConstantData, ConstantCount);

#ifdef _DEBUG
            gpVoodooLogger->Log
                (
                LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::GetPixelShaderConstant(%d, %d, %d) == %d", Register, pConstantData,
                ConstantCount, hr
                );
#endif
            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::GetPixelShaderFunction(DWORD Handle, void *pData, DWORD *pSizeOfData)
        {

            // HRESULT hr = m_RealDevice->GetPixelShaderFunction(Handle,void* pData,DWORD* pSizeOfData);
#ifdef _DEBUG
            gpVoodooLogger->Log
                (
                LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::GetPixelShaderFunction(%d, %d, %d) == UNUSED", Handle, pData,
                pSizeOfData
                );
#endif
            return D3DERR_INVALIDCALL;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::DrawRectPatch(UINT Handle, CONST float *pNumSegs, CONST D3DRECTPATCH_INFO *pRectPatchInfo)
        {
            HRESULT hr = m_RealDevice->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::DrawRectPatch(%d, %d, %d) == %d", Handle, pNumSegs, pRectPatchInfo, hr);
#endif
            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::DrawTriPatch(UINT Handle, CONST float *pNumSegs, CONST D3DTRIPATCH_INFO *pTriPatchInfo)
        {
            HRESULT hr = m_RealDevice->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::DrawTriPatch(%d, %d, %d) == %d", Handle, pNumSegs, pTriPatchInfo, hr);
#endif
            return hr;
        }
        HRESULT STDMETHODCALLTYPE CVoodoo3DDevice8::DeletePatch(UINT Handle)
        {
            HRESULT hr = m_RealDevice->DeletePatch(Handle);

#ifdef _DEBUG
            gpVoodooLogger->Log(LL_ModDebug, VOODOO_DX9_NAME, L"CVoodoo3DDevice8::DeletePatch(%d) == %d", Handle, hr);
#endif
            return hr;
        }
    }
}