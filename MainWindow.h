#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

/** MainWindow
 ** code for the main window and docking widgets.
 **/

#include <QtGui>

#include <Quarter/QuarterWidget.h>

#include "BrainQuarter.h"
#include "ResourceForm.h"

using namespace SIM::Coin3D::Quarter;

class MainWindow : public QMainWindow {
	Q_OBJECT
public:
	/* constructor */
	MainWindow();
	/* destructor */
	virtual ~MainWindow();

	/* opens file based on extension */
	bool openFile(QFile &filename);
	void saveTagFileFunc(QFile &filename);
	
public slots:
	/* prompts user for filename from file dialog */
	void openFileFromDialog();
	/* exits the application */
	void quitApplication();
	void saveTagFromDialog();
	void createTagFunc();
	void addTagFunc();
private:
	/* set up the actions (file open, etc.) */
	void createActions();
	/* set up the menu */
	void createMenus();
	/* create the colourbars if they don't already exist */
	void initColourbars();

	QMenu *fileMenu;
	QAction *openAction;
	QAction *saveTagAction;
	QAction *quitAction;
		
	/* holds the main viewer application */
	BrainQuarter *viewer;
	ResourceForm *resourceForm;
	/* dock to hold the tree view */
	QDockWidget *resourceDock;
	QVector <float> tagsVect;
	float *Tagpoint;
	double Tagsize;


};

#endif /*MAINWINDOW_H_*/
