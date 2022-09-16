#ifdef _DEBUG
#define MY_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__) 
#else
#define MY_NEW new
#endif
#include "memory_scanner.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    #ifdef _DEBUG
       _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    #endif
    QApplication a(argc, argv);
    memory_scanner w;
    w.show();
    return a.exec();
}
