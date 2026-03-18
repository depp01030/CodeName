#include "../include/mainwindow.h"
#include <iostream>
#include <QApplication>

int main(int argc, char *argv[])
{           
    QApplication a(argc, argv);
    MainWindow w;
    // if (!w.get_login_status()){  // Check if login was unsuccessful
    //     return -1;  // Exit with a non-zero status code
    // }
    w.show();
    return a.exec();
}
