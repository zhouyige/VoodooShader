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

#include "VSParameterDX9.hpp"

namespace VoodooShader
{
    #define VOODOO_DEBUG_TYPE VSParameterDX9
    DeclareDebugCache();

    VSParameterDX9::VSParameterDX9(_Pre_notnull_ IEffect * const pEffect, _In_ LPD3DXEFFECT pDXEffect = nullptr, _In_ D3DXHANDLE pParamHandle = nullptr) :
        m_Refs(0), m_Effect(pEffect), m_DXEffect(pDXEffect), m_DXHandle(pParamHandle)
    {
        if (!pEffect)
        {
            Throw(VOODOO_CORE_NAME, VSTR("Unable to create parameter with no effect."), nullptr);
        }

        m_Core = m_Effect->GetCore();

        if (!m_DXEffect)
        {
            Throw(VOODOO_CORE_NAME, VSTR("Unable to create parameter with no hardware effect."), m_Core);
        }
        else if (!m_DXHandle)
        {
            Throw(VOODOO_CORE_NAME, VSTR("Unable to create parameter with no hardware handle."), m_Core);
        }

        D3DXPARAMETER_DESC desc;
        ZeroMemory(&desc, sizeof(D3DXPARAMETER_DESC));
        if (FAILED(m_DXEffect->GetParameterDesc(m_DXHandle, &desc)))
        {
            Throw(VOODOO_CORE_NAME, VSTR("Unable to retrieve parameter description."), m_Core);
        }

        m_Name = desc.Name;
        m_Desc.Type = (ParameterType)desc.Type;
        m_Desc.Columns = desc.Columns;
        m_Desc.Rows = desc.Rows;
        m_Desc.Elements = desc.Elements;

        // Handle sampler
        if (m_Desc.Type >= PT_Texture && m_Desc.Type <= PT_TextureCube)
        {
            D3DXHANDLE texAnnot = m_DXEffect->GetAnnotationByName(m_DXHandle, "vs_texture");
            if (texAnnot)
            {
                LPCSTR texName = nullptr;
                if (SUCCEEDED(m_DXEffect->GetString(texAnnot, &texName)))
                {
                    m_Texture = m_Core->GetTexture(texName);
                }
                else
                {
                    m_Core->GetLogger()->LogMessage(LL_CoreWarning, VOODOO_CORE_NAME, Format("Sampler %1% has no source texture annotation.") << m_Name);
                }
            }
        }

        // Handle linkage
        D3DXHANDLE sourceAnnot = m_DXEffect->GetAnnotationByName(m_DXHandle, "vs_source");
        if (sourceAnnot)
        {
            LPCSTR sourceName = NULL;
            if (SUCCEEDED(m_DXEffect->GetString(sourceAnnot, &sourceName)))
            {
                IParameter * sourceParam = m_Core->GetParameter(sourceName, m_Desc);
                if (sourceParam)
                {
                    sourceParam->AttachParameter(this);
                }
            }
            else
            {
                m_Core->GetLogger()->LogMessage(LL_CoreWarning, VOODOO_CORE_NAME, 
                    Format("Unable to get source annotation for parameter %1%.") << m_Name);
            }
        }

        AddThisToDebugCache();
    }

    VSParameterDX9::VSParameterDX9(_In_ ICore * pCore, const String & name, ParameterDesc desc) :
        m_Refs(0), m_Core(pCore), m_Effect(nullptr), m_Name(name), m_Desc(desc), m_DXEffect(nullptr), m_DXHandle(nullptr)
    {
        AddThisToDebugCache();

        // Create an artificial description
    }

    VSParameterDX9::~VSParameterDX9()
    {
        RemoveThisFromDebugCache();
    }

    uint32_t VOODOO_METHODTYPE VSParameterDX9::AddRef() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        return SAFE_INCREMENT(m_Refs);
    }

    uint32_t VOODOO_METHODTYPE VSParameterDX9::Release() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        if (SAFE_DECREMENT(m_Refs) == 0)
        {
            delete this;
            return 0;
        }
        else
        {
            return m_Refs;
        }
    }

    VoodooResult VOODOO_METHODTYPE VSParameterDX9::QueryInterface(_In_ Uuid refid, _Deref_out_opt_ const IObject ** ppOut) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());
        if (!ppOut)
        {
            return VSFERR_INVALIDPARAMS;
        }
        else
        {
            if (refid == IID_IObject)
            {
                *ppOut = static_cast<const IObject*>(this);
            }
            else if (refid == IID_IParameter)
            {
                *ppOut = static_cast<const IParameter*>(this);
            }
            else if (refid == CLSID_VSParameterDX9)
            {
                *ppOut = static_cast<const VSParameterDX9*>(this);
            }
            else
            {
                *ppOut = nullptr;
                return VSFERR_INVALIDUUID;
            }

            (*ppOut)->AddRef();
            return VSF_OK;
        }
    }

    String VOODOO_METHODTYPE VSParameterDX9::ToString() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        return Format("VSParameter(%1%)") << m_Name;
    }

    ICore * VOODOO_METHODTYPE VSParameterDX9::GetCore() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        return m_Core;
    }

    String VOODOO_METHODTYPE VSParameterDX9::GetName() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        return m_Name;
    }

    VoodooResult VOODOO_METHODTYPE VSParameterDX9::GetProperty(const Uuid propid, _In_ Variant * pValue) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        if (!pValue) VSFERR_INVALIDPARAMS;

        if (propid == PropIds::D3DX9Handle)
        {
            pValue->Type = UT_PVoid;
            pValue->VPVoid = (PVOID)m_DXHandle;
            return VSF_OK;
        } 
        else if (propid == PropIds::D3DX9Effect)
        {
            pValue->Type = UT_PVoid;
            pValue->VPVoid = (PVOID)m_DXEffect;
            return VSF_OK;
        }
        else
        {
            PropertyMap::const_iterator property = m_Properties.find(propid);
            if (property != m_Properties.end())
            {
                CopyMemory(pValue, &property->second, sizeof(Variant));
                return VSF_OK;
            }
        }

        return VSFERR_INVALIDCALL;
    }

    VoodooResult VOODOO_METHODTYPE VSParameterDX9::SetProperty(const Uuid propid, Variant * pValue)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        if (propid == PropIds::D3DX9Handle || propid == PropIds::D3DX9Effect)
        {
            return VSFERR_INVALIDPARAMS;
        }

        m_Properties[propid] = (*pValue);
        return VSF_OK;
    }

    VOODOO_METHODDEF_(ParameterDesc, VSParameterDX9::GetDesc)() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        return m_Desc;
    }

    VOODOO_METHODDEF(VSParameterDX9::GetBool)(bool * pVal) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        if (!pVal) return VSFERR_INVALIDPARAMS;

        BOOL rv = 0;
        if (SUCCEEDED(m_DXEffect->GetBool(m_DXHandle, &rv)))
        {
            (*pVal) = (rv != 0);
            return VSF_OK;
        }
        else
        {
            return VSFERR_INVALIDCALL;
        }
    }

    VOODOO_METHODDEF(VSParameterDX9::GetFloat)(float * pVal) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        if (!pVal) return VSFERR_INVALIDPARAMS;

        if (SUCCEEDED(m_DXEffect->GetFloat(m_DXHandle, pVal)))
        {
            return VSF_OK;
        }
        else
        {
            return VSFERR_INVALIDCALL;
        }
    }

    VOODOO_METHODDEF(VSParameterDX9::GetInt)(int32_t * pVal) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        if (!pVal) return VSFERR_INVALIDPARAMS;

        if (SUCCEEDED(m_DXEffect->GetInt(m_DXHandle, pVal)))
        {
            return VSF_OK;
        }
        else
        {
            return VSFERR_INVALIDCALL;
        }
    }

    VOODOO_METHODDEF(VSParameterDX9::GetString)(String * pVal) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        if (!pVal) return VSFERR_INVALIDPARAMS;

        LPCSTR rv = nullptr;
        if (SUCCEEDED(m_DXEffect->GetString(m_DXHandle, &rv)) && rv)
        {
            (*pVal) = String(rv);
            return VSF_OK;
        }
        else
        {
            return VSFERR_INVALIDCALL;
        }
    }

    VOODOO_METHODDEF(VSParameterDX9::GetTexture)(ITexture ** ppVal) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        if (!ppVal) return VSFERR_INVALIDPARAMS;

        if (m_Desc.Type >= PT_Texture && m_Desc.Type <= PT_TextureCube)
        {
            (*ppVal) = m_Texture.get();
            return VSF_OK;
        }
        else
        {
            return VSFERR_INVALIDCALL;
        }
    }

    VOODOO_METHODDEF(VSParameterDX9::GetVector)(Float4 * pVal) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        if (!pVal) return VSFERR_INVALIDPARAMS;

        D3DXVECTOR4 rv;
        if (SUCCEEDED(m_DXEffect->GetVector(m_DXHandle, &rv)))
        {
            pVal->X = rv.x;
            pVal->Y = rv.y;
            pVal->Z = rv.z;
            pVal->W = rv.w;
            return VSF_OK;
        }
        else
        {
            return VSFERR_INVALIDCALL;
        }
    }

    VOODOO_METHODDEF(VSParameterDX9::SetBool)(bool val)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        if (SUCCEEDED(m_DXEffect->SetBool(m_DXHandle, val)))
        {
            return VSF_OK;
        }
        else
        {
            return VSFERR_INVALIDCALL;
        }
    }

    VOODOO_METHODDEF(VSParameterDX9::SetFloat)(float val)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        if (SUCCEEDED(m_DXEffect->SetFloat(m_DXHandle, val)))
        {
            return VSF_OK;
        }
        else
        {
            return VSFERR_INVALIDCALL;
        }
    }

    VOODOO_METHODDEF(VSParameterDX9::SetInt)(int32_t val)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        if (SUCCEEDED(m_DXEffect->SetInt(m_DXHandle, val)))
        {
            return VSF_OK;
        }
        else
        {
            return VSFERR_INVALIDCALL;
        }
    }

    VOODOO_METHODDEF(VSParameterDX9::SetString)(const String & val)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        std::string vstr = val.ToStringA();
        if (SUCCEEDED(m_DXEffect->SetString(m_DXHandle, vstr.c_str())))
        {
            return VSF_OK;
        }
        else
        {
            return VSFERR_INVALIDCALL;
        }
    }

    VOODOO_METHODDEF(VSParameterDX9::SetTexture)(ITexture * pVal)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        if (m_Desc.Type == PT_Sampler1D || m_Desc.Type == PT_Sampler2D || m_Desc.Type == PT_Sampler3D)
        {
            return m_Core->GetAdapter()->BindTexture(this, pVal);
        }
        else
        {
            return VSFERR_INVALIDCALL;
        }
    }

    VOODOO_METHODDEF(VSParameterDX9::SetVector)(Float4 val)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        D3DXVECTOR4 sv;
        sv.x = val.X;
        sv.y = val.Y;
        sv.z = val.Z;
        sv.w = val.W;

        if (SUCCEEDED(m_DXEffect->SetVector(m_DXHandle, &sv)))
        {
            return VSF_OK;
        }
        else
        {
            return VSFERR_INVALIDCALL;
        }
    }

    bool VOODOO_METHODTYPE VSParameterDX9::IsVirtual() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        return m_Virtual;
    }

    VoodooResult VOODOO_METHODTYPE VSParameterDX9::AttachParameter(IParameter * const pParam)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        if (!pParam)
        {
            return VSFERR_INVALIDPARAMS;
        }
        else if (!m_Virtual)
        {
            m_Core->GetLogger()->LogMessage
            (
                LL_CoreWarning, VOODOO_CORE_NAME, 
                Format("Cannot attach parameter '%1%' to non-virtual parameter '%2%'.") << pParam << this
            );
            return VSFERR_INVALIDPARAMS;
        }

        m_Attached.push_back(pParam);

        return VSF_OK;
    }

    VoodooResult VOODOO_METHODTYPE VSParameterDX9::DetachParameter(IParameter * const pParam)
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        std::remove_if(m_Attached.begin(), m_Attached.end(), [&pParam](IParameterRef param){return param.get() == pParam;});

        return VSF_OK;
    }

    IEffect * VOODOO_METHODTYPE VSParameterDX9::GetEffect() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Core->GetLogger());

        return m_Effect;
    }
}
