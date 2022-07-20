#include "ntapi.h"
#include "MemoryHelper.h"
#include "RustSDK.h"
#include "security.h"
#include <stdarg.h>
#include "Wdf.h"
#include "common.h"
#include <time.h>
void NTAPI time_thread(PVOID);
void NTAPI main_thread(PVOID);
NTSTATUS Sleep(LONGLONG ms_duration);


/*  LOAD ROUTINE
typedef PCHAR(*GET_PROCESS_IMAGE_NAME) (PEPROCESS Process);
GET_PROCESS_IMAGE_NAME gGetProcessImageFileName;
_______________________
UNICODE_STRING sPsGetProcessImageFileName = RTL_CONSTANT_STRING(L"PsGetProcessImageFileName");
gGetProcessImageFileName = (GET_PROCESS_IMAGE_NAME)MmGetSystemRoutineAddress(&sPsGetProcessImageFileName);

*/


bool is_work = true;

NTSTATUS DriverEntry(PDRIVER_OBJECT driver_object, PUNICODE_STRING registry_path)
{

	UNREFERENCED_PARAMETER(driver_object);
	UNREFERENCED_PARAMETER(registry_path);

	//EraseHeader();

	
	HANDLE time_thread_handle = nullptr;
	HANDLE main_thread_handle = nullptr;
	 auto status = PsCreateSystemThread(
		&main_thread_handle,
		GENERIC_ALL,
		nullptr,
		nullptr,
		nullptr,
		main_thread,
		nullptr
	);
	 NtClose(main_thread_handle);


	 /*
	 status |= PsCreateSystemThread(
		&time_thread_handle,
		GENERIC_ALL,
		nullptr,
		nullptr,
		nullptr,
		time_thread,
		nullptr
	);
		NtClose(time_thread_handle);
	*/




	return STATUS_SUCCESS;
}


void NTAPI time_thread(PVOID)
{
	for (int i = 0; i < 86'400; i++)
	{
		Sleep(1000);
		if (!is_work)break;
	}
	is_work = false;
}

void NTAPI main_thread(PVOID)
{
	
//UNICODE_STRING driver_name = RTL_CONSTANT_STRING(L"iqvw64e.sys");//Capcom.sys 0x57cd1415 (timeDateStamp)
//clearCache(driver_name, 0x5284EAC3);

/*
if (!FindMmDriverData())
{
	DbgPrint("MmDriverData find");
     if ( ClearUnloadedDriver(&driver_name, true) == STATUS_SUCCESS)DbgPrint("ClearUnloadedDriver sucessful");
	else DbgPrint("ClearUnloadedDriver error");
}
else
{
	DbgPrint("MmDriverData not find");
}
*/

	auto rust_client = skCrypt(L"RustClient.exe"); //шифруем строку
	while (_MemoryHelper.attach(rust_client.decrypt()) != STATUS_SUCCESS)//цикл пока не найдем процесс раста
	{
		DBG("wait attach");
		Sleep(1000);
		if (_MemoryHelper.GetProcessId(L"closee.exe"))return;//это нахуй не нужно
	}
	Sleep(1000);
	DBG("attach success");
 
	

	DWORD64 BaseNetworkable;
	auto game_assembly = skCrypt(L"GameAssembly.dll");//шифруем строку

	//_____________________________________________________________________________________________

	DWORD64 game_assembly_base= _MemoryHelper.get_module_handle(game_assembly.decrypt()); //получаем адресс загрузки модуля GameAssembly.dll
	if (!game_assembly_base)//если не нашли модуль тикаем нахуй
	{
		DBG("!game_assembly_base");
		return;
	}
	
	DWORD64 target = 0x2E6D45 + game_assembly_base;

	
	NTSTATUS status=NULL;
	PEPROCESS target_proc;
	status = PsLookupProcessByProcessId((HANDLE)_MemoryHelper.GetProcessId(rust_client.decrypt()), &target_proc);
	if (!NT_SUCCESS(status))
	{
		DBG("PEPROCESS get error");
		return;
	}

	KAPC_STATE apc;
	KeStackAttachProcess(target_proc, &apc);

	SIZE_T size = 100;
	ULONG oldProtect;
	DWORD64 baseProtect=
	status=ZwProtectVirtualMemory(ZwCurrentProcess(), (PVOID*)&target, &size, PAGE_EXECUTE_READWRITE, &oldProtect);
	if (!NT_SUCCESS(status))
	{
		DBG("ZwProtectVirtualMemory 1 error");
		return;
	}
	memset((void*)target,0x90, 100);
	status = ZwProtectVirtualMemory(ZwCurrentProcess(), (PVOID*)&target, &size, oldProtect, &oldProtect);
	if (!NT_SUCCESS(status))
	{
		DBG("ZwProtectVirtualMemory 2 error");
		return;
	}

	KeUnstackDetachProcess(&apc);
	

	DBG("SUCCESS");

	return;
	
	//_____________________________________________________________________________________________
	//цикл пока раст работает и BaseNetworkable не найден
	while ((is_work=_MemoryHelper.GetProcessId(rust_client.decrypt())) && !(BaseNetworkable = read(_MemoryHelper.get_module_handle(game_assembly.decrypt()) + oBaseNetworkable, DWORD64)))
	{
		DBG("wait B1seNetw0rk1ble");
		Sleep(1000);
		if (_MemoryHelper.GetProcessId(L"closee.exe"))return;
	}
	DBG("B1seNetw0rk1ble : %I64x", BaseNetworkable);


	while (_MemoryHelper.GetProcessId(rust_client.decrypt()) && is_work)//основной цикл, пока раст работает
	{
		BaseNetworkable_loop(BaseNetworkable);//рутина поиска игрока
		Sleep(100);
		if (_MemoryHelper.GetProcessId(L"closee.exe"))return;
	}



	//DbgPrintEx(0, 0, "close");
	//DbgPrintEx(0, 0, "UINT64 size: %i", sizeof(UINT64));
	//if(_MemoryHelper.virtual_protect((PVOID)GA_base, 2, PAGE_EXECUTE_READWRITE)==0)	DbgPrintEx(0, 0, "virtual_protect error");
	//write((PVOID)GA_base,0xff56, WORD);
	//DbgPrintEx(0, 0, "WORD base: %x", read((PVOID)GA_base, WORD));
	//write((PVOID)GA_base,0, int);
}


NTSTATUS Sleep(LONGLONG ms_duration)
{
	LARGE_INTEGER delay;
	delay.QuadPart = -1 * (10000 * ms_duration);
	return KeDelayExecutionThread(KernelMode, FALSE, &delay);
}