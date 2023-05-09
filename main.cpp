#include "dialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.setWindowTitle("CubeSass EPS Test Bench");
    //w.setFixedSize(400,112);

    w.show();

    return a.exec();
}
