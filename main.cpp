#include <Quarter/Quarter.h>
#include <iostream>
#include "MainWindow.h"

using namespace SIM::Coin3D::Quarter;

int main(int argc, char *argv[]) {
	// instantiate as a Quarter application
    QApplication app(argc, argv);
    Quarter::init();
    // initialize and show the main window
    MainWindow mainWin;

    // command line arguments treated as files to be loaded
    for (int i=1; i < argc; i++) {
        std::cout << "Loading " << argv[i] << std::endl;
        QFile file(argv[i]);
        mainWin.openFile(file);
    }
    mainWin.show();
    return app.exec();
}
