#include "hex_disassembly.h"
#include<qstring.h>
#include<qlistwidget.h>
#include"Disasm.h"
hex_disassembly::hex_disassembly(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
    
}

hex_disassembly::~hex_disassembly(){

}

void hex_disassembly::action_show_hex(HANDLE process, DWORD baseaddress){
    DWORD endaddress = baseaddress+PAGESIZE;
    while (baseaddress < endaddress) {
        BYTE arBytes[4096];
        QString str;
        int flag = ReadProcessMemory(process, (LPVOID)baseaddress, arBytes, 4096, NULL);
        if (flag) {
            size_t Inslength = 0;
            size_t Dislength = 4095;
            CString result;
            BYTE* BPStratAddr = arBytes;
///////////////////////////////////////// disasm
         
            DWORD k = 0;
            while (Dislength > 0) {
                Disasm(BPStratAddr, MODE64, Inslength, result);
                ui.listWidget_disasm->addItem("0x" + QString("%1").arg(baseaddress + k, 8, 16, QLatin1Char('0')) + "   "+QString::fromWCharArray((LPCTSTR)result, result.GetLength()));
                BPStratAddr = BPStratAddr + Inslength;
                Dislength = Dislength - Inslength;
                k += Inslength;
            }

////////////////////////////////////////////////////hex 

            for (int i = 0, k = 1; i < PAGESIZE; i++) {
                str += QString("%1").arg(arBytes[i], 2, 16, QLatin1Char('0')).toUpper() + "  ";
                if (k == 16) {
                    ui.listWidget_hex_view->addItem("0x" + QString("%1").arg(baseaddress + i - 0xf, 8, 16, QLatin1Char('0')).toUpper() + "   " + str);
                    str.clear();
                    k = 0;
                }
                k++;
            }
            baseaddress += PAGESIZE;
        }
    }
    this->show();
}
