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
        qDebug() << "\nDebug. [ main ] Loading ... " << argv[i];
        QFile file(argv[i]);

		mainWin.openFile(file);
    }

    
	// show main window and start the event loop
	qDebug() << "\nDebug. [ main ]  Showing the window ... " ;
    mainWin.show();
    qDebug() << "Debug. [ main ]  Going to event loop ... " ;
	app.exec();

	// detach  the Coin3d libraries
	qDebug() << "\nDebug. [ main ]  Cleaning detach the Coined library ... " ;
    Quarter::clean();

	// sleep young prince
	qDebug() << "\nDebug. [ main ]  Closing ... " ;
	mainWin.quitApplication();
	return 0;

}
