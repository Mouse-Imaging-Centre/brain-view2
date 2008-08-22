#include <Quarter/QuarterApplication.h>

//#include <QApplication>

#include "MainWindow.h"

int main(int argc, char *argv[]) {
	// instantiate as a Quarter application
    QuarterApplication app(argc, argv);
    // initialize and show the main window
    MainWindow mainWin;
    mainWin.show();
    return app.exec();
}
