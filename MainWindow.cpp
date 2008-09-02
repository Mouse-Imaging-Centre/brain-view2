#include "MainWindow.h"
#include "TreeModel.h"

MainWindow::MainWindow() : QMainWindow(){

	createActions();
	createMenus();

	viewer = new BrainQuarter();
	colourBar = new ColourBarForm(this);
	colourBarDock = new QDockWidget(tr("Colour Bar"), this);
	addDockWidget(Qt::LeftDockWidgetArea, colourBarDock);
	colourBarDock->setWidget(colourBar);

	resourceForm = new ResourceForm(this, viewer);
	resourceDock = new QDockWidget(tr("Resources"), this);
	addDockWidget(Qt::RightDockWidgetArea, resourceDock);
	resourceDock->setWidget(resourceForm);



	setCentralWidget(viewer);

}



MainWindow::~MainWindow() {
}



void MainWindow::createActions() {

}

void MainWindow::createMenus() {

}


