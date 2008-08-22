#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

/* MainWindow
 * code for the main window and docking widgets.
 */

#include <QtGui>

#include <Quarter/QuarterWidget.h>

#include "ColourBarForm.h"
#include "BrainQuarter.h"

using namespace SIM::Coin3D::Quarter;
class MainWindow : public QMainWindow {
	Q_OBJECT
public:
	// constructor
	MainWindow();
	// destructor
	virtual ~MainWindow();
	
private:
	// set up the actions (file open, etc.)
	void createActions();
	// set up the menu
	void createMenus();
		
	// holds the main viewer application
	BrainQuarter *viewer;
	// holds the widget to display the colour bar
	ColourBarForm *colourBar;
	// dock for the colour bar widget
	QDockWidget *colourBarDock;
	

};

#endif /*MAINWINDOW_H_*/
