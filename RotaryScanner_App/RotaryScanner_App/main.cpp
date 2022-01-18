#include "mainwindow.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setFixedSize(1200,520);
    w.setWindowTitle("Arduino Rotary Scanner Controller");
    w.show();
    return a.exec();
}
