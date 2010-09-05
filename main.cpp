#include <QtDebug>
#include <Quarter/Quarter.h>
#include "MainWindow.h"

using namespace SIM::Coin3D::Quarter;

int main(int argc, char *argv[]) {
	// instantiate as a Quarter application
    QApplication app(argc, argv);

	// init the Coin3d libraries
	//
    Quarter::init();

    // initialize and show the main window
    MainWindow mainWin;

    // command line arguments treated as files to be loaded
    for (int i=1; i < argc; ++i) {
        qDebug() << "Debug: Loading ... " << argv[i];
        QFile file(argv[i]);
        mainWin.openFile(file);
    }
//	return 0;
    
	// show main window and start the event loop
    mainWin.show();
	app.exec();

	// detach  the Coin3d libraries
    Quarter::clean();

	// sleep young prince
	return 0;

}
