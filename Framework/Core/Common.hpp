
#pragma once
#include "Core_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>

#include <Cg/Cg.h>

extern const HRESULT E_BADTHING     = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0000);
extern const HRESULT E_INVALIDCFG   = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0100);
extern const HRESULT E_BADCLSID     = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0200);

// IVoodooLogger
extern const HRESULT E_FILEERROR    = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0001);

extern const HRESULT E_NULLIMPL     = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0xFFFF);