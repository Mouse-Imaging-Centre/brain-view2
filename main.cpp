#include <Quarter/QuarterApplication.h>

//#include <QApplication>

#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QuarterApplication app(argc, argv);
    MainWindow mainWin;
    mainWin.show();
    return app.exec();
}
