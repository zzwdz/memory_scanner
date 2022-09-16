#pragma once

#include <QMainWindow>
#include "ui_hex_disassembly.h"

#ifdef __cplusplus
extern "C" {
#include"memoryscan.h"
}
#endif 

class hex_disassembly : public QMainWindow
{
	Q_OBJECT

public:
	hex_disassembly(QWidget *parent = nullptr);
	~hex_disassembly();
	void action_show_hex(HANDLE process, DWORD baseaddress);

private:
	Ui::hex_disassemblyClass ui;
	HANDLE proc;
	DWORD base_addr;
	
};
