#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <QtGui>



#include <Quarter/QuarterWidget.h>

#include "ColourBarForm.h"
#include "BrainQuarter.h"

using namespace SIM::Coin3D::Quarter;
class MainWindow : public QMainWindow {
	Q_OBJECT
public:
	MainWindow();
	virtual ~MainWindow();
	
private:
	void createActions();
	void createMenus();
		
	BrainQuarter *viewer;
	ColourBarForm *colourBar;
	QDockWidget *colourBarDock;
	

};

#endif /*MAINWINDOW_H_*/
