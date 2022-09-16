#include "memory_scanner.h"
#include<qstring.h>
#include<qinputdialog.h>
#include<qlineedit.h>
#include<qmessagebox.h>
#include<qlistwidget.h>
#include<qsharedpointer.h>
#include<qdebug.h>
memory_scanner::memory_scanner(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    proc = NULL;
    address = 0;
    scan_list = list_init();
//////////////////////<process select>
    connect(ui.actionOpenProcess, &QAction::triggered, this, &memory_scanner::action_open_process);
    void (show_process:: * fun_get_handle)(DWORD PID) = &show_process::signals_open;
    void (memory_scanner:: * fun_receive_handle)(DWORD pid) = &memory_scanner::action_receive_handle;
    connect(&show_proc, fun_get_handle, this, fun_receive_handle);
/////////////////////</process select>
 
//////////////////////<view memory>
    void (hex_disassembly:: * fun_memory_view)(HANDLE p, DWORD addr) = &hex_disassembly::action_show_hex;
    connect(ui.btn_memory_view, &QPushButton::clicked, this, &memory_scanner::action_view_memory);
    connect(this, &memory_scanner::open_view_memory, &hex_disasm, fun_memory_view);
/////////////////////</view memory>

/////////////////////<first scan>
    connect(ui.btn_first_scan, &QPushButton::clicked, this, &memory_scanner::action_first_scan);
/////////////////////</first scan>

/////////////////////<next scan>
    connect(ui.btn_next_scan, &QPushButton::clicked, this, &memory_scanner::action_next_scan);
////////////////////</next scan>

/////////////////////<select addr>
    connect(ui.list_widget_all,&QListWidget::itemDoubleClicked , this, &memory_scanner::action_select_address);
////////////////////</select addr>

/////////////////////<modify>
    connect(ui.listWidget, &QListWidget::itemDoubleClicked, this, &memory_scanner::action_modify);
////////////////////</modify>
}

memory_scanner::~memory_scanner(){
    close();
}

void memory_scanner::action_open_process(){
    show_proc.show();
    emit open_process_signal();
   
}

void memory_scanner::action_view_memory() {
    emit open_view_memory(proc, address);
}

void memory_scanner::action_receive_handle(DWORD p){
    proc = process_scan(p);
    address = read_base_addr(proc, _BASE_ADDRESS);
    ui.label_2->setText("PID: 0x"+ QString::number(p, 16).toUpper() + "| Base Address: " + "0x" + QString::number(address, 16).toUpper());
    repaint();

}

void memory_scanner::action_first_scan(){
    
    if (ui.lineEdit->text().isEmpty()) {
        QMessageBox::critical(this, tr("Warning"), tr("Please fill something in!!"));
    }
    else {
        list_destroy(scan_list);
        ui.list_widget_all->clear();
        QString str = ui.lineEdit->text();
        find_first(proc, (BYTE)str.toInt(), address);
        List* node = scan_list->next;
        int row = 0;
        while (node!= NULL) {
            QString s = QString("%1").arg((DWORD)node->value, 8, 16, QLatin1Char('0')).toUpper();
            ui.list_widget_all->addItem("0x"+s);
            
            node = node->next;
            
        }
        repaint();
    }
}

void memory_scanner::action_next_scan()
{
    if (ui.lineEdit->text().isEmpty()) {
        QMessageBox::critical(this, tr("Warning"), tr("Please fill something in!!"));
    }
    else {
        ui.list_widget_all->clear();
        QString str = ui.lineEdit->text();
        find_next(proc, (BYTE)str.toInt());
        List* node = scan_list->next;
        int row = 0;
        while (node!= NULL) {
            QString s = QString("%1").arg((DWORD)node->value, 8, 16, QLatin1Char('0')).toUpper();
            ui.list_widget_all->addItem("0x" + s);
            node = node->next;
        }
        repaint();
    }
    
}

void memory_scanner::action_select_address(QListWidgetItem* a){
    QString str = a->text();
    ui.listWidget->addItem(str);
}

void memory_scanner::action_modify(QListWidgetItem* select){
    QString str = select->text();
    bool ok;
    DWORD addr = str.toULong(&ok, 16);
    QString text = QInputDialog::getText(this, tr("Input Value"), tr("What value to change this to?"), QLineEdit::Normal, 0, &ok);
    if (ok && !text.isEmpty())
    {
        DWORD val = text.toULong();
        write_memory(proc, addr, val);
        DWORD k = GetLastError();
    }
    else {
        
        QMessageBox::critical(this, tr("Warning"), tr("Please fill something in!!"));
    }
    
}
