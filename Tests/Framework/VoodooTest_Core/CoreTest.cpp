// Voodoo Shader Framework Unit Tests: Voodoo/ICore:VoodooShader::ICore
#include "WinUnit.h"

#include "VoodooFramework.hpp"
#pragma comment(lib, "Voodoo_Core.lib")
using namespace VoodooShader;

namespace
{
    ICore *gpCore;
    InitParams gParams;
} 

FIXTURE(DefaultCore);

SETUP(DefaultCore)
{
    char cwd[MAX_PATH + 1];
    GetCurrentDirectory(MAX_PATH, cwd);
    cwd[MAX_PATH] = 0;

    gParams.Config = "D:\\Code\\VoodooShader\\Tests\\Resources\\default.xmlconfig";
    gParams.GlobalRoot = gParams.LocalRoot = gParams.RunRoot = cwd;
    gParams.Loader = "TestLoader.exe";
    gParams.Target = "TestCore.exe";

    gpCore = CreateCore(&gParams);
    WIN_ASSERT_NOT_nullptr(gpCore, "DefaultCore::Setup: gpCore is nullptr.");
}

TEARDOWN(DefaultCore)
{
    delete gpCore;
}

BEGIN_TESTF(Core_GetParser, DefaultCore)
{
    IParser* parser = gpCore->GetParser();
    WIN_ASSERT_NOT_nullptr(parser.get(), "Core_GetParser: parser is nullptr.");
}

END_TESTF BEGIN_TESTF(Core_GetHookManager, DefaultCore)
{
    IHookManager* hook = gpCore->GetHookManager();
    WIN_ASSERT_NOT_nullptr(hook.get(), "Core_GetHookManager: hook manager is nullptr.");
}

END_TESTF BEGIN_TESTF(Core_GetFileSystem, DefaultCore)
{
    IFileSystem* filesystem = gpCore->GetFileSystem();
    WIN_ASSERT_NOT_nullptr(filesystem.get(), "Core_GetFileSystem: filesystem is nullptr.");
}

END_TESTF BEGIN_TESTF(Core_GetAdapter, DefaultCore)
{
    IAdapter* adapter = gpCore->GetAdapter();
    WIN_ASSERT_NOT_nullptr(adapter.get(), "Core_GetAdapter: adapter is nullptr.");
}

END_TESTF BEGIN_TESTF(Core_GetLogger, DefaultCore)
{
    ILogger* Logger = gpCore->GetLogger();
    WIN_ASSERT_NOT_nullptr(Logger.get(), "Core_GetLogger: logger is nullptr.");
}

END_TESTF BEGIN_TESTF(Core_GetConfig, DefaultCore)
{
    void *pConfig = gpCore->GetConfig();
    WIN_ASSERT_NOT_nullptr(pConfig, "Core_GetConfig: pConfig is nullptr.");
}
END_TESTF

// Test covers: ICore::GetCgContext ICore::SetCgContext
BEGIN_TESTF(Core_CgContext, DefaultCore)
{
    CGcontext inCg = CGcontext(0xFFFFFFFF);
    WIN_ASSERT_TRUE(gpCore->SetCgContext(inCg), "Core_CgContext: SetCgContext(%p) returned false.", inCg);

    CGcontext pCg = gpCore->GetCgContext();
    WIN_ASSERT_EQUAL(inCg, pCg, "Core_CgContext: inCg != pCg.");

    inCg = CGcontext(0x88888888);
    WIN_ASSERT_FALSE(gpCore->SetCgContext(inCg), "Core_CgContext: SetCgContext(%p) allowed overwriting.", inCg);

    inCg = nullptr;
    WIN_ASSERT_TRUE(gpCore->SetCgContext(inCg), "Core_CgContext: SetCgContext(%p) returned false (2).", inCg);

    pCg = gpCore->GetCgContext();
    WIN_ASSERT_EQUAL(inCg, pCg, "Core_CgContext: inCg != pCg.");
}
END_TESTF