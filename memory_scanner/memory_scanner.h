#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_memory_scanner.h"
#include"show_process.h"
#include"hex_disassembly.h"
#include<qaction.h>
#include<qpushbutton.h>
#include<qmap.h>
#include<qlistwidget.h>
class memory_scanner : public QMainWindow
{
    Q_OBJECT

public:
    memory_scanner(QWidget *parent = nullptr);
    ~memory_scanner();
    void action_open_process();
    void action_view_memory();
    void action_receive_handle(DWORD PID);
    void action_first_scan();
    void action_next_scan();
    void action_select_address(QListWidgetItem* a);
    void action_modify(QListWidgetItem* select);
signals:
    void open_process_signal();
    void open_view_memory(HANDLE p,DWORD base_addr);

private:
    Ui::memory_scannerClass ui;
    hex_disassembly hex_disasm;
    show_process show_proc;
    HANDLE proc;
    DWORD address;
    QMap<int, DWORD>process_map;
};
