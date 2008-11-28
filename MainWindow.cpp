#include "MainWindow.h"
#include "TreeModel.h"
#include <iostream>

MainWindow::MainWindow() : QMainWindow(){

	// stuff for QSettings
	QCoreApplication::setOrganizationName("MINC");
	QCoreApplication::setApplicationName("brain-view");
	
	QSettings settings;
	std::cout << "SETTINGS PATH: " << settings.fileName().toStdString() << std::endl;
	QSettings set2(QSettings::IniFormat, QSettings::UserScope, "MINC", "brain-view");
	std::cout << "SETTINGS PATH: " << set2.fileName().toStdString() << std::endl;
	std::cout << "PID: " << QCoreApplication::applicationPid() << std::endl;
	
	createActions();
	createMenus();

	viewer = new BrainQuarter();
	//colourBar = new ColourBarForm(this);
	//colourBarDock = new QDockWidget(tr("Colour Bar"), this);
	//addDockWidget(Qt::LeftDockWidgetArea, colourBarDock);
	//colourBarDock->setWidget(colourBar);

	resourceForm = new ResourceForm(this, viewer);
	resourceDock = new QDockWidget(tr("Resources"), this);
	addDockWidget(Qt::RightDockWidgetArea, resourceDock);
	resourceDock->setWidget(resourceForm);

	setCentralWidget(viewer);

}



MainWindow::~MainWindow() {
}

bool MainWindow::openFile(QFile &filename) {
	QFileInfo fileInfo(filename);
	QString ext = fileInfo.suffix();
	if (ext == "obj") {
		return resourceForm->insertGeometry(filename);
	}
	else if (ext == "txt" || ext == "vertstats") {
		return resourceForm->insertVertstats(filename);
	}
	else {
		return false;
	}
}

void MainWindow::openFileFromDialog() {
	QString name = QFileDialog::getOpenFileName(this,
		tr("Open File"), QString(), tr("Files (*.obj *.txt *.vertstats)"));
	if (! name.isNull()) {
		QFile filename(name);
		openFile(filename);
	}
}

void MainWindow::createActions() {
	openAction = new QAction(tr("&Open File"), this);
	connect(openAction, SIGNAL(triggered()), this, SLOT(openFileFromDialog()));
}

void MainWindow::createMenus() {
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(openAction);
}


