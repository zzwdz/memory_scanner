#include "show_process.h"
#include<qmap.h>
#include<qdebug.h>
#include<qpushbutton.h>
#include<qmessagebox.h>

show_process::show_process(QWidget *parent)
	: QMainWindow(parent)
{   
	ui.setupUi(this);
	pid = 0;
	int process_count = 1;
	int flag = 0;
	proc=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	current_process.dwSize = sizeof(current_process);
	flag = Process32First(proc, &current_process);

////////////////////////////////////////////

	while (flag != 0) {
		QString stringitem = "0x" + QString::number(current_process.th32ProcessID,16) + "   " + QString::fromWCharArray(current_process.szExeFile);
		ui.listWidget->addItem(stringitem);
		list_map.insert(process_count, current_process.th32ProcessID);
		flag = Process32Next(proc, &current_process);
		process_count++;
	}
/////////////////////////////
	connect(ui.btn_cancel, &QPushButton::clicked, this, &show_process::action_cancel);
	connect(ui.listWidget, &QListWidget::currentRowChanged, this, &show_process::action_get_pid);
	connect(ui.btn_open, &QPushButton::clicked, this, &show_process::action_open);
}

show_process::~show_process(){

}


void show_process::action_cancel(){

	emit signals_cancel();
	this->hide();
}



void show_process::action_get_pid(int row){

	pid = list_map[row+1];
}



void show_process::action_open()
{
	
	if (pid == 0) {

		QMessageBox::critical(this, tr("Warning"), tr("Choose one process!!"));

	}	
	else{

		emit signals_open(pid);
		this->hide();
		
	}
	
}
