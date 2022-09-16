#include "memoryscan.h"

List* scan_list;

DWORD endaddr;
/// <summary>
/// ����pidɨ�����(yes)
/// </summary>
/// <param name="pid"></param>
/// <returns></returns>
HANDLE process_scan(DWORD pid){   
    return OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid);
}

/// <summary>
/// ��ȡ������ڵ�ַ(yes)
/// </summary>
/// <param name="proc"></param>
/// <param name="addr"></param>
/// <returns></returns>
DWORD read_base_addr(HANDLE proc, DWORD addr){
    MEMORY_BASIC_INFORMATION basic_inform;
    size_t inform;

    while (addr < _END_ADDRESS) {
        inform = VirtualQueryEx(proc, (LPVOID)addr, &basic_inform, sizeof(basic_inform));
        if (basic_inform.State == MEM_COMMIT)//�����ַ�ѱ����䣬֤���˵�ַΪ�ļ���ͷ���ڴ�
            break;
        addr += PAGESIZE;//windowsʹ�÷�ҳ�����������ڴ棬����ҳΪ��λ��Ѱ
    }
    return addr;
}



/// <summary>
/// ����ַ����ַ(7FFFFFF)��ʼɨ��
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
/// �ڶ���ɨ�裬���һ��ɨ��ĵ�ַ�Աȣ�����ͬ����������ͬ��ɾ�����
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
/// д���ڴ�
/// </summary>
/// <param name="proc"></param>
/// <param name="address"></param>
/// <param name="value"></param>
/// <returns></returns>
int write_memory(HANDLE proc, DWORD address, DWORD value){
    return WriteProcessMemory(proc, (LPVOID)address, &value, sizeof(DWORD), NULL);
}
