#include "memoryscan.h"

List* scan_list;

DWORD endaddr;
/// <summary>
/// 根据pid扫描进程(yes)
/// </summary>
/// <param name="pid"></param>
/// <returns></returns>
HANDLE process_scan(DWORD pid){   
    return OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid);
}

/// <summary>
/// 获取程序入口地址(yes)
/// </summary>
/// <param name="proc"></param>
/// <param name="addr"></param>
/// <returns></returns>
DWORD read_base_addr(HANDLE proc, DWORD addr){
    MEMORY_BASIC_INFORMATION basic_inform;
    size_t inform;

    while (addr < _END_ADDRESS) {
        inform = VirtualQueryEx(proc, (LPVOID)addr, &basic_inform, sizeof(basic_inform));
        if (basic_inform.State == MEM_COMMIT)//物理地址已被分配，证明此地址为文件标头所在处
            break;
        addr += PAGESIZE;//windows使用分页机制来管理内存，故以页为单位搜寻
    }
    return addr;
}



/// <summary>
/// 从起址到终址(7FFFFFF)开始扫描
/// </summary>
/// <param name="value"></param>
/// <returns></returns>
int find_first(HANDLE proc,BYTE value, DWORD addr){
    MEMORY_BASIC_INFORMATION basic_inform;
    size_t inform=0;
    while (addr < _END_ADDRESS) {
        inform = VirtualQueryEx(proc, (LPVOID)addr, &basic_inform, sizeof(basic_inform));
        if (inform == sizeof(MEMORY_BASIC_INFORMATION)) {
/////////////////////////////<if1>
            if ((basic_inform.Protect == PAGE_READWRITE) && basic_inform.State == MEM_COMMIT) {
/////////////////////////////<if2>
                BYTE arBytes[4096];
                if (ReadProcessMemory(proc, (LPVOID)addr, arBytes, 4096, NULL)) {
/////////////////////////////<if3>
                    for (int i = 0; i < PAGESIZE-6; i++) {
                        if (arBytes[i] == value)
                            list_add(scan_list, addr+i);
                    }
////////////////////////////</if3>
                }
                endaddr = addr + PAGESIZE;
////////////////////////////</if2>
            }
            addr += PAGESIZE;
////////////////////////////</if1>
        }
    }
    return 0;
}

/// <summary>
/// 第二次扫描，与第一次扫描的地址对比，若相同则保留，若不同则删除结点
/// </summary>
/// <param name="next_value"></param>
/// <returns></returns>
int find_next(HANDLE proc,BYTE next_value){
    List* node_check = scan_list;
    List* new_scan_list = list_init();
    int k = list_length(scan_list);
    int i = 0;
    DWORD dwOldProt, dwNewProt = 0;
    
    while (node_check != NULL) {
        BYTE check_val[4096];
        VirtualProtectEx(proc, (void*)node_check->value, 4096, PAGE_READWRITE, &dwOldProt);
        int flag=ReadProcessMemory(proc, (LPVOID)node_check->value, check_val, 4096, NULL);
        DWORD check_error = GetLastError();
        if (flag) {
            if (next_value == check_val[0]) {
                list_add(new_scan_list, node_check->value);
                
            }
        }
        node_check = node_check->next;
    }
    list_destroy(scan_list);
    scan_list = new_scan_list;
    return 0;
}

/// <summary>
/// 写入内存
/// </summary>
/// <param name="proc"></param>
/// <param name="address"></param>
/// <param name="value"></param>
/// <returns></returns>
int write_memory(HANDLE proc, DWORD address, DWORD value){
    return WriteProcessMemory(proc, (LPVOID)address, &value, sizeof(DWORD), NULL);
}
