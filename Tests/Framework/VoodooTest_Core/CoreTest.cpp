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

/*
 * This file is part of the Voodoo Shader unit test suite.
 * 
 * These tests focus on core functionality.
 */

#include "WinUnit.h"

#define VOODOO_STATIC_LINK
#include "VoodooFramework.hpp"
using namespace VoodooShader;

namespace
{
    ICore * gpCore;
    InitParams gParams;
}

FIXTURE(DefaultCore);

SETUP(DefaultCore)
{
    wchar_t cwd[MAX_PATH + 1];
    GetCurrentDirectory(MAX_PATH, cwd);
    cwd[MAX_PATH] = 0;

    gParams.Config = L"D:\\Code\\VoodooShader\\Tests\\Resources\\default.xmlconfig";
    gParams.GlobalRoot = gParams.LocalRoot = gParams.RunRoot = cwd;
    gParams.Loader = L"TestLoader.exe";
    gParams.Target = L"TestCore.exe";

    gpCore = CreateCore();
    gpCore->AddRef();
    gpCore->Initialize(&gParams);
    WIN_ASSERT_NOT_NULL(gpCore, L"DefaultCore::Setup: gpCore is nullptr.");
}

TEARDOWN(DefaultCore)
{
    gpCore->Release();
}

BEGIN_TESTF(Core_GetParser, DefaultCore)
{
    IParser* parser = gpCore->GetParser();
    WIN_ASSERT_NOT_NULL(parser, L"Core_GetParser: parser is nullptr.");
}
END_TESTF;

BEGIN_TESTF(Core_GetHookManager, DefaultCore)
{
    IHookManager* pHookManager = gpCore->GetHookManager();
    WIN_ASSERT_NOT_NULL(pHookManager, L"Core_GetHookManager: hook manager is nullptr.");
}
END_TESTF;

BEGIN_TESTF(Core_GetFileSystem, DefaultCore)
{
    IFileSystem* pFilesystem = gpCore->GetFileSystem();
    WIN_ASSERT_NOT_NULL(pFilesystem, L"Core_GetFileSystem: filesystem is nullptr.");
}
END_TESTF;

BEGIN_TESTF(Core_GetAdapter, DefaultCore)
{
    IAdapter* pAdapter = gpCore->GetAdapter();
    WIN_ASSERT_NOT_NULL(pAdapter, L"Core_GetAdapter: adapter is nullptr.");
}
END_TESTF;

BEGIN_TESTF(Core_GetLogger, DefaultCore)
{
    ILogger* pLogger = gpCore->GetLogger();
    WIN_ASSERT_NOT_NULL(pLogger, L"Core_GetLogger: logger is nullptr.");
}
END_TESTF;

BEGIN_TESTF(Core_GetConfig, DefaultCore)
{
    void *pConfig = gpCore->GetConfig();
    WIN_ASSERT_NOT_NULL(pConfig, L"Core_GetConfig: pConfig is nullptr.");
}
END_TESTF;

// Test covers: ICore::GetCgContext ICore::SetCgContext
BEGIN_TESTF(Core_CgContext, DefaultCore)
{
    CGcontext inCg = CGcontext(0xFFFFFFFF);
    WIN_ASSERT_TRUE(gpCore->SetCgContext(inCg), L"Core_CgContext: SetCgContext(%p) returned false.", inCg);

    CGcontext pCg = gpCore->GetCgContext();
    WIN_ASSERT_EQUAL(inCg, pCg, L"Core_CgContext: inCg != pCg.");

    inCg = CGcontext(0x88888888);
    WIN_ASSERT_FALSE(gpCore->SetCgContext(inCg), L"Core_CgContext: SetCgContext(%p) allowed overwriting.", inCg);

    inCg = nullptr;
    WIN_ASSERT_TRUE(gpCore->SetCgContext(inCg), L"Core_CgContext: SetCgContext(%p) returned false (2).", inCg);

    pCg = gpCore->GetCgContext();
    WIN_ASSERT_EQUAL(inCg, pCg, L"Core_CgContext: inCg != pCg.");
}
END_TESTF;
