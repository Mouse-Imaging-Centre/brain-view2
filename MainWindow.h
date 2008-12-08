#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

/* MainWindow
 * code for the main window and docking widgets.
 */

#include <QtGui>

#include <Quarter/QuarterWidget.h>

//#include "ColourBarForm.h"
#include "BrainQuarter.h"
#include "ResourceForm.h"

using namespace SIM::Coin3D::Quarter;
class MainWindow : public QMainWindow {
	Q_OBJECT
public:
	// constructor
	MainWindow();
	// destructor
	virtual ~MainWindow();
	// opens file based on extension
	bool openFile(QFile &filename);

public slots:
	// prompts user for filename from file dialog
        void openFileFromDialog();
        // exits the application
        void quitApplication();
private:
	// set up the actions (file open, etc.)
	void createActions();
	// set up the menu
	void createMenus();
		
	QMenu *fileMenu;
        QAction *openAction;
        QAction *quitAction;
	
	// holds the main viewer application
	BrainQuarter *viewer;
	// holds the widget to display the colour bar
	//ColourBarForm *colourBar;
	// dock for the colour bar widget
	//QDockWidget *colourBarDock;
	// tree to keep track of open resources
	//QTreeView *treeview;
	ResourceForm *resourceForm;
	// dock to hold the tree view
	QDockWidget *resourceDock;
	//void sampleTreeSetup();

};

#endif /*MAINWINDOW_H_*/
