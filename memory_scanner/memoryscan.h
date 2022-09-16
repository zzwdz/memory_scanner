//////////////////////////////////////////////////////////////////////////////
//
//  实现进程扫描及基址访问修改
//  scan process, access and modify the value base address
//
//  2022/9/15
//

#pragma once
#include<windows.h>
#include<tlhelp32.h>
#include"mylist.h"

//////////////////


HANDLE process_scan(DWORD pid);

extern List* scan_list;
extern DWORD endaddr;
#ifndef _BASE_ADDRESS 
#define _BASE_ADDRESS 0x00400000
#endif  

#ifndef _END_ADDRESS 
#define _END_ADDRESS 0x2FFFFFFF
#endif  

#define PAGESIZE 4096
/////////////////


DWORD read_base_addr(HANDLE proc, DWORD addr);

int find_first(HANDLE proc,BYTE value, DWORD addr);

int find_next(HANDLE proc,BYTE next_value);

int write_memory(HANDLE proc,DWORD address, DWORD value);










