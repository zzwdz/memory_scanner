#pragma once
#include <QMainWindow>
#include "ui_show_process.h"

#ifdef __cplusplus
extern "C" {
#include"memoryscan.h"
}
#endif 
#include<qstring.h>
#include<qlistwidget.h>

//////////////////////////////////////////////////
class show_process : public QMainWindow
{  
	Q_OBJECT
		
public:
	show_process(QWidget *parent = nullptr);
	~show_process();
	void action_cancel();
	void action_get_pid(int row);
	void action_open();
signals:
	void signals_cancel();
	void signals_open(DWORD PID);

private:
	Ui::show_processClass ui;
	HANDLE proc;
	PROCESSENTRY32 current_process;
	QMap<int, DWORD>list_map;
	DWORD pid;
};



