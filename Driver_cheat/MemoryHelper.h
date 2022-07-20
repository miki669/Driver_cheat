#pragma once

#include "ntapi.h"

class MemoryHelper
{
public:
	PEPROCESS TargetPEprocess;
	HANDLE pid;

	NTSTATUS attach(LPCWSTR process_name);
	ULONGLONG get_module_handle(LPCWSTR module_name);
	INT virtual_protect(PVOID address, size_t size, INT protect);


	template <typename type>
	type readmem(PVOID src)
	{
		type ret = {};
		SIZE_T bytes;
		MmCopyVirtualMemory(TargetPEprocess, src, (PEPROCESS)PsInitialSystemProcess, (PVOID)&ret, sizeof(type), KernelMode, &bytes);
		return ret;
	}

	template <typename type>
	void writemem(PVOID dst_addr, type var)
	{
		SIZE_T bytes;
		NTSTATUS status=MmCopyVirtualMemory((PEPROCESS)PsInitialSystemProcess, (PVOID)&var, TargetPEprocess, dst_addr, sizeof(type), KernelMode, &bytes);
		//DbgPrintEx(0, 0, "status : %u", status);
		//DbgPrintEx(0, 0, "bytes copy: %u", bytes);
	}

	UINT GetProcessId(PCWSTR ImageName);
	
};

extern MemoryHelper _MemoryHelper;

#define read(src_addr,type)  _MemoryHelper.readmem<type>(reinterpret_cast<PVOID>(src_addr))
#define write(dst_addr,var,type)  _MemoryHelper.writemem<type>(reinterpret_cast<PVOID>(dst_addr),var)

