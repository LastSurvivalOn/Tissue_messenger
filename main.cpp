#include "mainwindow.h"
#include "tissue_server.h"
#include "useraccount.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //Tissue_server w(1488);
    //w.show();
    UserAccount c("localhost", 1488);
    c.show();
    // UserAccount c1("localhost", 1488);
    // c1.show();
    return a.exec();
}
