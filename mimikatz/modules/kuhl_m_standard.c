/*	Benjamin DELPY `gentilkiwi`
	http://blog.gentilkiwi.com
	benjamin@gentilkiwi.com
	Licence : https://creativecommons.org/licenses/by/4.0/
*/
#include "kuhl_m_standard.h"

const KUHL_M_C kuhl_m_c_standard[] = {
	//{kuhl_m_standard_test,		L"test",	L"Test routine (you don\'t want to see this !)"},
	{kuhl_m_standard_exit,		L"exit",		L"Quit mimikatz"},
	{kuhl_m_standard_cls,		L"cls",			L"Clear screen (doesn\'t work with redirections, like PsExec)"},
	{kuhl_m_standard_answer,	L"answer",		L"Answer to the Ultimate Question of Life, the Universe, and Everything"},
	{kuhl_m_standard_coffee,	L"coffee",		L"Please, make me a coffee!"},
	{kuhl_m_standard_sleep,		L"sleep",		L"Sleep an amount of milliseconds"},
    {kuhl_m_standard_waitfor,   L"waitfor",     L"Sleep until sepcific time of a day" },
	{kuhl_m_standard_log,		L"log",			L"Log mimikatz input/output to file"},
	{kuhl_m_standard_base64,	L"base64",		L"Switch file input/output base64"},
	{kuhl_m_standard_version,	L"version",		L"Display some version informations"},
	{kuhl_m_standard_cd,		L"cd",			L"Change or display current directory"},
	{kuhl_m_standard_localtime,	L"localtime",	L"Displays system local date and time (OJ command)"},
	{kuhl_m_standard_hostname,	L"hostname",	L"Displays system local hostname"},
};
const KUHL_M kuhl_m_standard = {
	L"standard",	L"Standard module",	L"Basic commands (does not require module name)",
	ARRAYSIZE(kuhl_m_c_standard), kuhl_m_c_standard, NULL, NULL
};
/*
NTSTATUS kuhl_m_standard_test(int argc, wchar_t * argv[])
{
	return STATUS_SUCCESS;
}
*/
NTSTATUS kuhl_m_standard_exit(int argc, wchar_t * argv[])
{
	kprintf(L"Bye!\n");
	return STATUS_FATAL_APP_EXIT;
}

NTSTATUS kuhl_m_standard_cls(int argc, wchar_t * argv[])
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = {0, 0};
	DWORD count;
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	GetConsoleScreenBufferInfo(hStdOut, &csbi);
	FillConsoleOutputCharacter(hStdOut, L' ', csbi.dwSize.X * csbi.dwSize.Y, coord, &count);
	SetConsoleCursorPosition(hStdOut, coord);
	return STATUS_SUCCESS;
}

NTSTATUS kuhl_m_standard_answer(int argc, wchar_t * argv[])
{
	kprintf(L"42.\n");
	return STATUS_SUCCESS;
}

NTSTATUS kuhl_m_standard_coffee(int argc, wchar_t * argv[])
{
	kprintf(L"\n    ( (\n     ) )\n  .______.\n  |      |]\n  \\      /\n   `----'\n");
	return STATUS_SUCCESS;
}

NTSTATUS kuhl_m_standard_sleep(int argc, wchar_t * argv[])
{
	DWORD dwMilliseconds = argc ? wcstoul(argv[0], NULL, 0) : 1000;
	kprintf(L"Sleep : %u ms... ", dwMilliseconds);
	Sleep(dwMilliseconds);
	kprintf(L"End !\n");
	return STATUS_SUCCESS;
}

NTSTATUS kuhl_m_standard_waitfor(int argc, wchar_t * argv[])
{
    WORD wHour = (WORD) (argc ? wcstoul(argv[0], NULL, 0) : 23);
    WORD wMinute = (WORD) ((argc > 1) ? wcstoul(argv[1], NULL, 0) : 59);
    wHour = (wHour > 23) ? 23 : wHour;
    wMinute = (wMinute > 59) ? 59 : wMinute;

    HANDLE hTimer = NULL;
    LARGE_INTEGER liDueTime;
    SYSTEMTIME st = { 0 };
    // Local filetime
    FILETIME lft = { 0 };
    // UTC filetime
    FILETIME uft = { 0 };
    GetLocalTime(&st);
    st.wDay = ((wHour <= st.wHour) && (wMinute <= st.wMinute)) ? st.wDay + 1 : st.wDay;
    st.wHour = wHour;
    st.wMinute = wMinute;
    // Set due time seconds to 0
    st.wSecond = 0;

    SystemTimeToFileTime(&st, &lft);
    LocalFileTimeToFileTime(&lft, &uft);
    liDueTime.QuadPart = (__int64) uft.dwLowDateTime + ((__int64) (uft.dwHighDateTime) << 32LL);
    kprintf(L"Waiting for %04d-%02d-%02d %02d:%02d:%02d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

    if (hTimer = CreateWaitableTimer(NULL, TRUE, NULL))
        SetWaitableTimer(hTimer, &liDueTime, 0, NULL, NULL, 0) ? WaitForSingleObject(hTimer, INFINITE) : PRINT_ERROR_AUTO(L"SetWaitableTimer(init)");
    else
        PRINT_ERROR_AUTO(L"CreateWaitableTimer(init)");

    kprintf(L"Waitfor end!\n");
    return STATUS_SUCCESS;
}

NTSTATUS kuhl_m_standard_log(int argc, wchar_t * argv[])
{
    BOOL is_stop = kull_m_string_args_byName(argc, argv, L"stop", NULL, NULL);
    BOOL is_clear = kull_m_string_args_byName(argc, argv, L"clear", NULL, NULL);
    BOOL is_first_clear = (argc ? (((_wcsicmp(argv[0], L"/clear") == 0) || (_wcsicmp(argv[0], L"-clear") == 0)) ? 1 : 0) : 0);
    PCWCHAR filename = (is_stop ? NULL : ((argc && !is_first_clear) ? argv[0] : MIMIKATZ_DEFAULT_LOG));
    PCWCHAR mode = (is_clear ? L"w" : L"a");
    kprintf(L"Using \'%s\' for logfile with mode \'%s\' : %s\n", filename, mode, kull_m_output_file(filename, mode) ? L"OK" : L"KO");
    return STATUS_SUCCESS;
}

NTSTATUS kuhl_m_standard_base64(int argc, wchar_t * argv[])
{
	if(!kull_m_string_args_bool_byName(argc, argv, L"in", &isBase64InterceptInput))
		kull_m_string_args_bool_byName(argc, argv, L"input", &isBase64InterceptInput);

	if(!kull_m_string_args_bool_byName(argc, argv, L"out", &isBase64InterceptOutput))
		kull_m_string_args_bool_byName(argc, argv, L"output", &isBase64InterceptOutput);

	kprintf(L"isBase64InterceptInput  is %s\nisBase64InterceptOutput is %s\n", isBase64InterceptInput ? L"true" : L"false", isBase64InterceptOutput ? L"true" : L"false");
	return STATUS_SUCCESS;
}

const wchar_t *version_libs[] = {
	L"lsasrv.dll", L"msv1_0.dll", L"tspkg.dll", L"wdigest.dll", L"kerberos.dll", L"livessp.dll", L"dpapisrv.dll",
	L"kdcsvd.dll", L"cryptdll.dll", L"lsadb.dll", L"samsrv.dll", L"rsaenh.dll", L"ncrypt.dll", L"ncryptprov.dll",
	L"eventlog.dll", L"wevtsvc.dll", L"termsrv.dll",
};
NTSTATUS kuhl_m_standard_version(int argc, wchar_t * argv[])
{
	NTSTATUS status;
	HMODULE hModule;
	PNTQUERYSYSTEMINFORMATIONEX pNtQuerySystemInformationEx;
	SYSTEM_ISOLATED_USER_MODE_INFORMATION iumi = {TRUE, FALSE /* 0 */};
	DWORD i, len;
	PVOID buffer;
	UINT lenVer;
	VS_FIXEDFILEINFO *verInfo;
	BOOL isWow64;

	#ifdef _M_X64
	isWow64 = TRUE;
	#else
	if(IsWow64Process(GetCurrentProcess(), &isWow64))
	#endif
	{
		kprintf(
			L"\n" MIMIKATZ L" " MIMIKATZ_VERSION L" (arch " MIMIKATZ_ARCH L")\n"
			L"Windows NT %u.%u build %u (arch x%s)\n"
			L"msvc %u %u\n",
			MIMIKATZ_NT_MAJOR_VERSION, MIMIKATZ_NT_MINOR_VERSION, MIMIKATZ_NT_BUILD_NUMBER, isWow64 ? L"64" : L"86", _MSC_FULL_VER, _MSC_BUILD
			);
	}

	if((MIMIKATZ_NT_BUILD_NUMBER >= KULL_M_WIN_MIN_BUILD_10) && (hModule = GetModuleHandle(L"ntdll")))
	{
		if(pNtQuerySystemInformationEx = (PNTQUERYSYSTEMINFORMATIONEX) GetProcAddress(hModule, "NtQuerySystemInformationEx"))
		{
			status = pNtQuerySystemInformationEx(SystemIsolatedUserModeInformation, &iumi, 8, &iumi, sizeof(iumi), NULL);
			if(NT_SUCCESS(status))
			{
				if(iumi.SecureKernelRunning)
					kprintf(L"\n> SecureKernel is running\n");
				if(iumi.Spare0[0] & 1)
					kprintf(L"> Credential Guard may be running\n");
			}
			else PRINT_ERROR(L"NtQuerySystemInformationEx: %08x\n", status);
		}
	}

	if(argc)
	{
		kprintf(L"\n");
		for(i = 0; i < ARRAYSIZE(version_libs); i++)
		{
			if(len = GetFileVersionInfoSize(version_libs[i], NULL))
			{
				kprintf(L"%s\t: ", version_libs[i]);
				if(buffer = LocalAlloc(LPTR, len))
				{
					if(GetFileVersionInfo(version_libs[i], 0, len, buffer))
					{
						if(VerQueryValue(buffer, L"\\", (LPVOID *) &verInfo, &lenVer) && (verInfo->dwSignature == VS_FFI_SIGNATURE))
							kprintf(L"%hu.%hu.%hu.%hu\n", verInfo->dwFileVersionMS >> 16, verInfo->dwFileVersionMS, verInfo->dwFileVersionLS >> 16, verInfo->dwFileVersionLS);
						else PRINT_ERROR_AUTO(L"VerQueryValue");
					}
					else PRINT_ERROR_AUTO(L"GetFileVersionInfoEx");
					LocalFree(buffer);
				}
			}
		}
	}
	return STATUS_SUCCESS;
}

NTSTATUS kuhl_m_standard_cd(int argc, wchar_t * argv[])
{
	wchar_t * buffer;
	if(kull_m_file_getCurrentDirectory(&buffer))
	{
		if(argc)
			kprintf(L"Cur: ");
		kprintf(L"%s\n", buffer);
		LocalFree(buffer);
	}
	else PRINT_ERROR_AUTO(L"kull_m_file_getCurrentDirectory");

	if(argc)
	{
		if(SetCurrentDirectory(argv[0]))
		{
			if(kull_m_file_getCurrentDirectory(&buffer))
			{
				kprintf(L"New: %s\n", buffer);
				LocalFree(buffer);
			}
			else PRINT_ERROR_AUTO(L"kull_m_file_getCurrentDirectory");
		}
		else PRINT_ERROR_AUTO(L"SetCurrentDirectory");
	}
	return STATUS_SUCCESS;
}

NTSTATUS kuhl_m_standard_localtime(int argc, wchar_t * argv[])
{
	FILETIME ft;
	TIME_ZONE_INFORMATION tzi;
	DWORD dwTzi;
	GetSystemTimeAsFileTime(&ft);
	dwTzi = GetTimeZoneInformation(&tzi);
	kprintf(L"Local: "); kull_m_string_displayLocalFileTime(&ft); kprintf(L"\n");
	if(dwTzi != TIME_ZONE_ID_INVALID && dwTzi != TIME_ZONE_ID_UNKNOWN)
		kprintf(L"Zone : %.32s\n", (dwTzi == TIME_ZONE_ID_STANDARD) ? tzi.StandardName : tzi.DaylightName);
	kprintf(L"UTC  : "); kull_m_string_displayFileTime(&ft); kprintf(L"\n");
	return STATUS_SUCCESS;
}

NTSTATUS kuhl_m_standard_hostname(int argc, wchar_t * argv[])
{
	wchar_t *buffer;
	DWORD dwSize = 0;
	if(!GetComputerNameEx(ComputerNamePhysicalDnsFullyQualified, NULL, &dwSize) && (GetLastError() == ERROR_MORE_DATA))
	{
		if(buffer = (wchar_t *) LocalAlloc(LPTR, dwSize * sizeof(wchar_t)))
		{
			if(GetComputerNameEx(ComputerNamePhysicalDnsFullyQualified, buffer, &dwSize))
				kprintf(L"%s\n", buffer);
			else PRINT_ERROR_AUTO(L"GetComputerNameEx(data)");
			LocalFree(buffer);
		}
	}
	else PRINT_ERROR_AUTO(L"GetComputerNameEx(init)");
	return STATUS_SUCCESS;
}