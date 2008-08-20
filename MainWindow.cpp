#include "MainWindow.h"

MainWindow::MainWindow() : QMainWindow(){
	
	createActions();
	createMenus();

	viewer = new BrainQuarter();
	colourBar = new ColourBarForm(this);
	colourBarDock = new QDockWidget(tr("Colour Bar"), this);
	addDockWidget(Qt::LeftDockWidgetArea, colourBarDock);
	colourBarDock->setWidget(colourBar);

	setCentralWidget(viewer);
	
}



MainWindow::~MainWindow() {
}

void MainWindow::createActions() {
	
}

void MainWindow::createMenus() {
	
}


