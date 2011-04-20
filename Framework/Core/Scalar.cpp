#include "Scalar.hpp"

namespace VoodooShader
{
    Scalar::Scalar( _In_ IVoodooCore * pCore, _In_ BSTR pName, _In_ ParameterType Type )
        : m_Core(pCore), m_Shader(NULL), m_Name(pName), m_Type(Type), m_Refrs(0)
    {
        m_Value.Create(16);
    }

    Scalar::Scalar( _In_ IVoodooShader * pShader, CGparameter pParameter)
        : m_Shader(pShader), m_Parameter(pParameter), m_Refrs(0)
    {
        m_Value.Create(16);
        m_Shader->GetCore(&m_Core);
        const char * name = cgGetParameterName(m_Parameter);
        if ( name == NULL )
        {
            CStringW mname;
            mname.Format(L"parameter_%p", m_Parameter);
            m_Name = mname;
        } else {
            m_Name = name;
        }
    }

    HRESULT Scalar::QueryInterface(REFIID iid, void ** pp)
    {
        if ( pp == NULL )
        {
            return E_POINTER;
        } else if ( iid == IID_IUnknown || iid == IID_VoodooObject || iid == IID_VoodooParameter || iid == IID_VoodooScalar ) {
            *pp = this;
            return S_OK;
        } else {
            *pp = NULL;
            return E_NOINTERFACE;
        }
    }

    ULONG Scalar::AddRef()
    {
        return (++m_Refrs);
    }

    ULONG Scalar::Release()
    {
        --m_Refrs;
        if ( m_Refrs == 0 )
        {
            delete this;
            return 0;
        } else {
            return m_Refrs;
        }
    }

    HRESULT Scalar::GetName(LPBSTR pName)
    {
        if ( pName == NULL )
        {
            return E_INVALIDARG;
        }

        return m_Name.CopyTo(pName);
    }

    HRESULT Scalar::GetCore(IVoodooCore ** ppCore)
    {
        if ( ppCore == NULL )
        {
            return E_INVALIDARG;
        }

        *ppCore = m_Core;
        return S_OK;
    }

    BOOL Scalar::get_IsVirtual()
    {
        return m_Virtual;
    }

    HRESULT Scalar::GetShader(IVoodooShader ** ppShader)
    {
        if ( m_Virtual == TRUE )
        {
            return E_VIRTUAL;
        } else if ( ppShader == NULL ) {
            return E_INVALIDARG;
        }

        *ppShader = m_Shader;
        return S_OK;
    }

    HRESULT Scalar::GetParameterType(ParameterType * pType)
    {
        if ( pType == NULL ) return E_INVALIDARG;

        *pType = m_Type;
        return S_OK;
    }

    HRESULT Scalar::AttachParameter(IVoodooScalar * pParameter)
    {
        if ( m_Virtual == FALSE ) return E_NOTVIRTUAL;
        if ( pParameter == NULL ) return E_INVALIDARG;

        CGparameter other = NULL;
        pParameter->GetCgParameter((void**)&other);

        cgConnectParameter(m_Parameter, other);

        return S_OK;
    }

    HRESULT Scalar::GetCgParameter(void ** ppCgParameter)
    {
        if ( ppCgParameter == NULL ) return E_INVALIDARG;

        *ppCgParameter = m_Parameter;
        return S_OK;
    }

    HRESULT Scalar::put_Value(SAFEARRAY * pData)
    {
        if ( pData == NULL ) return E_INVALIDARG;

        m_Value.CopyFrom(pData);
        return S_OK;
    }

    HRESULT Scalar::get_Value(SAFEARRAY ** ppData)
    {
        if ( ppData == NULL ) return E_INVALIDARG;

        m_Value.CopyTo(ppData);
        return S_OK;
    }

    UINT Scalar::get_Components()
    {
        if ( m_Type == PT_Float1 ) return 1;
        if ( m_Type == PT_Float2 ) return 2;
        if ( m_Type == PT_Float3 ) return 3;
        if ( m_Type == PT_Float4 ) return 4;
        if ( m_Type == PT_Matrix ) return 16;
        return 0;
    }
}