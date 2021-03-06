#include "NM_Index.h"
#include "NM_Main.h"
#include "Defines.h"

#include <lazy_importer.hpp>

#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "Wintrust.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "MPR")
#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "dbghelp.lib")
#pragma comment(lib, "wtsapi32.lib")
#pragma comment(lib, "userenv.lib")
#pragma comment(lib, "imagehlp.lib")
#pragma comment(lib, "Netapi32.lib")
#pragma comment(lib, "FltLib.lib")

std::shared_ptr <CNM_App> g_nmApp = nullptr;

#pragma optimize("", off)
void CNoMercy::InitTest(bool bClient)
{
	g_nmApp = std::make_shared<CNM_App>();
	if (!IS_VALID_SMART_PTR(g_nmApp))
	{
		Logf(CUSTOM_LOG_FILENAME, xorstr("[TEST] Base allocation failed!\n").crypt_get());
		return;
	}

	auto dwPreInitErrCode = 0UL;
	if (g_nmApp->PreInitialize(NM_TESTAPP, &dwPreInitErrCode) == false)
	{
		Logf(CUSTOM_LOG_FILENAME, xorstr("[TEST] Pre-Initialize failed! Error: %u\n").crypt_get(), dwPreInitErrCode);
		return;
	}

	g_nmApp->SetInitializeStatus(false);

	g_nmApp->DataInstance()->SetAppType(NM_TESTAPP);

	auto dwInitErrCode = 0UL;
	if (g_nmApp->Initialize(bClient, false, &dwInitErrCode) == false)
	{
		Logf(CUSTOM_LOG_FILENAME, xorstr("[TEST] Initialize failed! Error: %u\n").crypt_get(), dwInitErrCode);
		return;
	}

	g_nmApp->SetInitializeStatus(true);

	DEBUG_LOG(LL_SYS, "Test Initilization completed!");
}

bool CNoMercy::InitCore(const char* c_szLicenseCode, LPCVOID c_pModuleInfo, bool bStandaloneProcess)
{
	static auto bClient = true;

	g_nmApp = std::make_shared<CNM_App>();
	if (!IS_VALID_SMART_PTR(g_nmApp))
	{
		Logf(CUSTOM_LOG_FILENAME, xorstr("[CORE] Base allocation failed!\n").crypt_get());
		return false;
	}

	auto dwPreInitErrCode = 0UL;
	if (g_nmApp->PreInitialize(NM_CLIENT, &dwPreInitErrCode) == false)
	{
		Logf(CUSTOM_LOG_FILENAME, xorstr("[CORE] Pre-Initialize failed! Error: %u\n").crypt_get(), dwPreInitErrCode);
		return false;
	}

	g_nmApp->SetInitializeStatus(false);

	g_nmApp->DataInstance()->SetMainThreadId(LI_FIND(GetCurrentThreadId)());
	g_nmApp->DataInstance()->SetLicenseCode(c_szLicenseCode);
	g_nmApp->DataInstance()->SetAntiModuleInformations(c_pModuleInfo);

	std::string szStandaloneLicense = xorstr("ABCDEF123490").crypt_get();
	if (bStandaloneProcess /* debug */ || !strcmp(c_szLicenseCode, szStandaloneLicense.c_str()) /* release */)
		g_nmApp->DataInstance()->SetAppType(NM_TESTAPP);
	else
		g_nmApp->DataInstance()->SetAppType(NM_CLIENT);

	auto dwInitErrCode = 0UL;
	if (g_nmApp->Initialize(bClient, false, &dwInitErrCode) == false)
	{
		Logf(CUSTOM_LOG_FILENAME, xorstr("[CORE] Initialize failed! Error: %u\n").crypt_get(), dwInitErrCode);
		return false;
	}

	g_nmApp->SetInitializeStatus(true);

	DEBUG_LOG(LL_SYS, "Core Initilization completed!");
	return true;
}

bool CNoMercy::InitMasterServer(WORD wPort)
{
	static auto bClient = false;

	g_nmApp = std::make_shared<CNM_App>();
	if (!IS_VALID_SMART_PTR(g_nmApp))
	{
		Logf(CUSTOM_LOG_FILENAME, xorstr("[MSERVER] Base allocation failed!\n").crypt_get());
		return false;
	}

	auto dwPreInitErrCode = 0UL;
	if (g_nmApp->PreInitialize(NM_SERVER, &dwPreInitErrCode) == false)
	{
		Logf(CUSTOM_LOG_FILENAME, xorstr("[MSERVER] Pre-Initialize failed! Error: %u\n").crypt_get(), dwPreInitErrCode);
		return false;
	}

	g_nmApp->SetInitializeStatus(false);

	g_nmApp->DataInstance()->SetAppType(NM_SERVER);

	auto dwInitErrCode = 0UL;
	if (g_nmApp->Initialize(bClient, false, &dwInitErrCode) == false)
	{
		Logf(CUSTOM_LOG_FILENAME, xorstr("[MSERVER] Initialize failed! Error: %u\n").crypt_get(), dwInitErrCode);
		return false;
	}

	g_nmApp->SetInitializeStatus(true);

	DEBUG_LOG(LL_SYS, "MServer Initilization completed!");
	return true;
}

bool CNoMercy::InitLauncher(HINSTANCE hInstance, const char* c_szLicenseCode, LPCVOID c_pModuleInfo, bool bProtected)
{
	static auto bClient = true;

	g_nmApp = std::make_shared<CNM_App>();
	if (!IS_VALID_SMART_PTR(g_nmApp))
	{
		Logf(CUSTOM_LOG_FILENAME, xorstr("[LAUNCHER] Base allocation failed!\n").crypt_get());
		return false;
	}

	auto dwPreInitErrCode = 0UL;
	if (g_nmApp->PreInitialize(NM_LAUNCHER, &dwPreInitErrCode) == false)
	{
		Logf(CUSTOM_LOG_FILENAME, xorstr("[LAUNCHER] Pre-Initialize failed! Error: %u\n").crypt_get(), dwPreInitErrCode);
		return false;
	}

	g_nmApp->SetInitializeStatus(false);

	g_nmApp->DataInstance()->SetAppType(NM_LAUNCHER);
	g_nmApp->DataInstance()->SetMainThreadId(LI_FIND(GetCurrentThreadId)());
	g_nmApp->DataInstance()->SetAntiModuleInformations(c_pModuleInfo);
	g_nmApp->DataInstance()->SetLicenseCode(c_szLicenseCode);
	g_nmApp->DataInstance()->SetMainWnd(hInstance);

	auto dwInitErrCode = 0UL;
	if (g_nmApp->Initialize(bClient, bProtected, &dwInitErrCode) == false)
	{
		Logf(CUSTOM_LOG_FILENAME, xorstr("[LAUNCHER] Initialize failed! Error: %u\n").crypt_get(), dwInitErrCode);
		return false;
	}

	g_nmApp->SetInitializeStatus(true);

	DEBUG_LOG(LL_SYS, "Launcher Initilization completed!");
	return true;
}

void CNoMercy::OnServiceMessage(int iMessageID)
{
	if (!IS_VALID_SMART_PTR(g_nmApp))
	{
		Logf(CUSTOM_LOG_FILENAME, xorstr("[SVCMSG] Base class not found!\n").crypt_get());
		return;
	}

	if (g_nmApp->AppIsInitiliazed() == false)
	{
		Logf(CUSTOM_LOG_FILENAME, xorstr("[SVCMSG] Base is not initialized!\n").crypt_get());
		return;
	}

	g_nmApp->OnServiceMessageHandled(iMessageID);
}

bool CNoMercy::InitService(bool bProtected, LPCVOID c_pModuleInfo)
{
	static auto bClient = true;

	g_nmApp = std::make_shared<CNM_App>();
	if (!IS_VALID_SMART_PTR(g_nmApp))
	{
		Logf(CUSTOM_LOG_FILENAME, xorstr("[SERVICE] Base allocation failed!\n").crypt_get());
		return false;
	}

	auto dwPreInitErrCode = 0UL;
	if (g_nmApp->PreInitialize(NM_SERVICE, &dwPreInitErrCode) == false)
	{
		Logf(CUSTOM_LOG_FILENAME, xorstr("[SERVICE] Pre-Initialize failed! Error: %u\n").crypt_get(), dwPreInitErrCode);
		return false;
	}

	g_nmApp->SetInitializeStatus(false);

	g_nmApp->DataInstance()->SetAppType(NM_SERVICE);
	g_nmApp->DataInstance()->SetMainThreadId(LI_FIND(GetCurrentThreadId)());
	g_nmApp->DataInstance()->SetAntiModuleInformations(c_pModuleInfo);

	auto dwInitErrCode = 0UL;
	if (g_nmApp->Initialize(bClient, bProtected, &dwInitErrCode) == false)
	{
		Logf(CUSTOM_LOG_FILENAME, xorstr("[SERVICE] Initialize failed! Error: %u\n").crypt_get(), dwInitErrCode);
		return false;
	}

	g_nmApp->SetInitializeStatus(true);

	DEBUG_LOG(LL_SYS, "Service Initilization completed!");
	return true;
}

bool CNoMercy::InitShadow(HMODULE hModule, HWND hWnd, HINSTANCE hInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	static auto bClient = true;

	g_nmApp = std::make_shared<CNM_App>();
	if (!IS_VALID_SMART_PTR(g_nmApp))
	{
		Logf(CUSTOM_LOG_FILENAME, xorstr("[SHADOW] Base allocation failed!\n").crypt_get());
		return false;
	}

	ANTI_MODULE_INFO ami = { hModule, nullptr, NULL, NULL, NULL };
	if (g_nmApp->InitSelfProtection(&ami) == false)
	{
		Logf(CUSTOM_LOG_FILENAME, xorstr("[SHADOW] Self-protection failed!\n").crypt_get());
		return false;
	}

	auto dwPreInitErrCode = 0UL;
	if (g_nmApp->PreInitialize(NM_SHADOW, &dwPreInitErrCode) == false)
	{
		Logf(CUSTOM_LOG_FILENAME, xorstr("[SHADOW] Pre-Initialize failed! Error: %u\n").crypt_get(), dwPreInitErrCode);
		return false;
	}

	if (g_nmApp->DataInstance()->IsShadowInitialized() == true)
		return false;
	g_nmApp->DataInstance()->SetShadowInitialized(true);

	g_nmApp->SetInitializeStatus(false);

	g_nmApp->DataInstance()->SetAppType(NM_SHADOW);

	auto dwInitErrCode = 0UL;
	if (g_nmApp->Initialize(bClient, false, &dwInitErrCode) == false)
	{
		Logf(CUSTOM_LOG_FILENAME, xorstr("[SHADOW] Initialize failed! Error: %u\n").crypt_get(), dwInitErrCode);
		return false;
	}

	g_nmApp->SetInitializeStatus(true);

	g_nmApp->FunctionsInstance()->InitShadow(hWnd, hInstance, lpszCmdLine, nCmdShow);

	DEBUG_LOG(LL_SYS, "Shadow Initilization completed!");
	return true;
}

bool CNoMercy::InitHelper(LPCVOID c_pModuleInfo)
{
	static auto bClient = true;

	g_nmApp = std::make_shared<CNM_App>();
	if (!IS_VALID_SMART_PTR(g_nmApp))
	{
		Logf(CUSTOM_LOG_FILENAME, xorstr("[HELPER] Base allocation failed!\n").crypt_get());
		return false;
	}

	auto dwPreInitErrCode = 0UL;
	if (g_nmApp->PreInitialize(NM_HELPERAPP, &dwPreInitErrCode) == false)
	{
		Logf(CUSTOM_LOG_FILENAME, xorstr("[HELPER] Pre-Initialize failed! Error: %u\n").crypt_get(), dwPreInitErrCode);
		return false;
	}

	g_nmApp->SetInitializeStatus(false);

	g_nmApp->DataInstance()->SetAppType(NM_HELPERAPP);
	g_nmApp->DataInstance()->SetMainThreadId(LI_FIND(GetCurrentThreadId)());
	g_nmApp->DataInstance()->SetAntiModuleInformations(c_pModuleInfo);

	auto dwInitErrCode = 0UL;
	if (g_nmApp->Initialize(bClient, false, &dwInitErrCode) == false)
	{
		Logf(CUSTOM_LOG_FILENAME, xorstr("[HELPER] Initialize failed! Error: %u\n").crypt_get(), dwInitErrCode);
		return false;
	}

	g_nmApp->SetInitializeStatus(true);

	DEBUG_LOG(LL_SYS, "Helper Initilization completed!");
	return true;
}


bool CNoMercy::Release()
{
	if (!IS_VALID_SMART_PTR(g_nmApp))
	{
		Logf(CUSTOM_LOG_FILENAME, xorstr("[FINAL] Base class not found!\n").crypt_get());
		return false;
	}

	if (g_nmApp->AppIsInitiliazed() == false)
	{
		Logf(CUSTOM_LOG_FILENAME, xorstr("[FINAL]Base is not initialized!\n").crypt_get());
		return false;
	}

	auto dwFinalErrCode = 0UL;
	if (g_nmApp->Finalize(&dwFinalErrCode) == false)
	{
		Logf(CUSTOM_LOG_FILENAME, xorstr("[FINAL] Finalize failed! Error: %u\n").crypt_get(), dwFinalErrCode);
		return false;
	}

	Logf(CUSTOM_LOG_FILENAME, xorstr("[FINAL] Finalization completed!").crypt_get());
	return true;
}


bool CNoMercy::CreateMessageHandler(TNMCallback lpMessageHandler)
{
	if (IS_VALID_SMART_PTR(g_nmApp) && IS_VALID_SMART_PTR(g_nmApp->SDKHelperInstance()))
	{
		return g_nmApp->SDKHelperInstance()->CreateMessageHandler(lpMessageHandler);
	}

	return false;
}
bool CNoMercy::SendNMMessage(int Code, LPVOID lpMessage)
{
	if (IS_VALID_SMART_PTR(g_nmApp) && IS_VALID_SMART_PTR(g_nmApp->SDKHelperInstance()))
	{
		return g_nmApp->SDKHelperInstance()->ProcessClientMessage(Code, lpMessage);
	}
	return false;
}

void CNoMercy::OnGameTick()
{
	if (IS_VALID_SMART_PTR(g_nmApp) && IS_VALID_SMART_PTR(g_nmApp->SDKHelperInstance()))
	{
		g_nmApp->SDKHelperInstance()->OnGameTick();
	}
}

#pragma optimize("", on)
