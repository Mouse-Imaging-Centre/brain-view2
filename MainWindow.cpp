#include <QtDebug>

#include "MainWindow.h"
#include "TreeModel.h"
#include <qtoolbutton.h>
#include <qpushbutton.h>

#include <stdio.h>    //FILE, fopen(), fclose()
#include <stdlib.h>
#include <sys/stat.h> //stat, stat()
#include <string>     //string
#include <fstream>    //fstream 
#include <iostream>   //cout

MainWindow::MainWindow() {
	qDebug() << "Debug. >>MainWindow::MainWindow()";

	// stuff for QSettings
	QCoreApplication::setOrganizationName("MINC");
	QCoreApplication::setApplicationName("brain-view");
	
	QSettings settings;
	qDebug() << "Debug. [ MainWindow() ] Settings path 1: " << settings.fileName();
	QSettings set2(QSettings::IniFormat, QSettings::UserScope, "MINC", "brain-view");
	qDebug() << "Debug. [ MainWindow() ] Settings path 2: " << set2.fileName();
	qDebug() << "Debug. [ MainWindow() ] Application Pid: " << QCoreApplication::applicationPid();
	
	createActions();
	createMenus();
	initColourbars();
// 	createTagbuttons();

	viewer = new BrainQuarter();
	//colourBar = new ColourBarForm(this);
	//colourBarDock = new QDockWidget(tr("Colour Bar"), this);
	//addDockWidget(Qt::LeftDockWidgetArea, colourBarDock);
	//colourBarDock->setWidget(colourBar);

	resourceForm = new ResourceForm(this, viewer);
	resourceDock = new QDockWidget(tr("Resources"), this);
	addDockWidget(Qt::RightDockWidgetArea, resourceDock);
	resourceDock->setWidget(resourceForm);
	
// 	tagForm = new TagForm(this, viewer);
// 	tagDock = new QDockWidget(tr("Tags"), this);
// 	addDockWidget(Qt::BottomDockWidgetArea, tagDock);
// 	tagDock->setWidget(tagForm);
// 	resourceDock->addWidget(tagForm);
	
	
	setCentralWidget(viewer);
	qDebug() << "Debug. <<MainWindow::MainWindow()";
}


void MainWindow::initColourbars() {
	qDebug() << "Debug. >>MainWindow::initColourbars()";

    // get the path from the settings
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "MINC", "brain-view");
    QString defaultDir = QString(QFileInfo(settings.fileName()).path()).append("/textures/");
    QString textureDir = settings.value("textureDir", defaultDir).toString();
    settings.setValue("textureDir", textureDir);
    qDebug() << "Debug. [ initColourbars() ] TextureDir: " << textureDir;

    // create the texture dir if it does not already exist
    QDir td(textureDir);
    if (! td.exists()) {
        td.mkpath(textureDir);
    }

    // for every colourbar in the resource file check to see if it
    // already exists. If not, write it out to file
	QDirIterator it(":/resources/colourbars");
	while ( it.hasNext() ) {
		QString fileNameFull = it.next();
        qDebug() << "Debug. [ initColourbars() ] Texture file full path: " << fileNameFull;
		QFileInfo fileInfo = it.fileInfo();
        QString fileName = fileInfo.baseName();
        qDebug() << "Debug. [ initColourbars() ] Texture file basename: " << fileName;
        fileName.append(".png");
        fileName.prepend("/");
        fileName.prepend(textureDir);
        if (! QFile(fileName).exists()) {
            qDebug() << "Debug. [ initColourbars() ] Adding texture to local texture dir: " << fileInfo.filePath();
            QImage i(fileInfo.filePath());
            bool a = i.isNull();
            bool b = i.save(fileName);
            qDebug() << "Debug. [ initColourbars() ] Created colourbar: " << fileName << " " << a << " " << b;
        }
	}

	qDebug() << "Debug. <<MainWindow::initColourbars()";
}


MainWindow::~MainWindow() {
}


bool MainWindow::openFile(QFile &filename) {
	qDebug() << "Debug. >>MainWindow::openFile()";

    QFileInfo fileInfo(filename);
    QString ext = fileInfo.suffix();

	bool status = false;
    if (ext == "obj" || ext == "h5") {
		status = resourceForm->insertGeometry(filename);
	}
    else if (ext == "txt" || ext == "vertstats") {
        status = resourceForm->insertVertstats(filename);
    }
    else if (ext == "tag") {
		status = resourceForm->insertTagfile(filename);
    }
    else if (ext == "config") {
		status = resourceForm->insertLabelLUfile(filename);
    }

	Q_ASSERT_X( status == true, "MainWindow::openFile", "Failed insert into Resource form" );
	qDebug() << "Debug. <<MainWindow::openFile()";
	return true;
}

void MainWindow::openFileFromDialog() {
	qDebug() << "Debug. >>MainWindow::openFileFromDialog()";
	QString name = QFileDialog::getOpenFileName(this,
                tr("Open File"), QString(), tr("Files (*.obj *.txt *.vertstats *.tag *.h5 *.config)"));
	if (! name.isNull()) {
		QFile filename(name);
		openFile(filename);
	}
	qDebug() << "Debug. <<MainWindow::openFileFromDialog()";
}


void MainWindow::createActions() {

	openAction = new QAction(tr("&Open File"), this);
	connect(openAction, SIGNAL(triggered()), this, SLOT(openFileFromDialog()));

	saveTagAction = new QAction(tr("&Save Tags to File"), this);
	connect(saveTagAction, SIGNAL(triggered()), this, SLOT(saveTagFromDialog()));
	
	quitAction = new QAction(tr("&Quit"), this);
	connect(quitAction, SIGNAL(triggered()), this, SLOT(quitApplication()));

}


void MainWindow::createMenus() {
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(openAction);
	fileMenu->addAction(saveTagAction);
	fileMenu->addAction(quitAction);

}



void MainWindow::createTagFunc(){
	Tagpoint = (float*) malloc(sizeof(float) * 3);
	Tagpoint = resourceForm->addTagpoint();
	std::cout << "\nCreated tag point is : " << Tagpoint[0] <<" , " << Tagpoint[1] << " , " << Tagpoint[2] <<std::endl;
	tagsVect.append(Tagpoint[0]);	//Tag(x)
	tagsVect.append(Tagpoint[1]);	//Tag(y)
	tagsVect.append(Tagpoint[2]);	//Tag(z)
}

void MainWindow::addTagFunc(){
	std::cout << "\nAdded tag point is : " << Tagpoint[0] <<" , " << Tagpoint[1] << " , " << Tagpoint[2] <<std::endl;
	goodtagsVect.append(Tagpoint[0]);	//Tag(x)
	goodtagsVect.append(Tagpoint[1]);	//Tag(y)
	goodtagsVect.append(Tagpoint[2]);	//Tag(z)
}



void MainWindow::saveTagFromDialog() {
	qDebug() << "Debug. >>MainWindow::saveTagFromDialog()";
	QString sname = QFileDialog::getSaveFileName(this,
                tr("Save Tags to File"), QString(), tr("Files (*.tag)"));
	QFile sfilename(sname);
	std::cout << "construct a file " << sfilename.fileName().toLatin1().data()<<std::endl;
	saveTagFileFunc(sfilename);

	qDebug() << "Debug. <<MainWindow::saveTagFromDialog()";
}

void MainWindow::saveTagFileFunc(QFile &filename){
	//create an e,pty file filename and save all the created tags in the file in format of .tag for follow_tree.py

	FILE *pFile;
	pFile = fopen (filename.fileName().toLatin1().data(), "w");
	if (pFile == NULL)
	{
		std::cout <<"No file for saving the tags!" <<std::endl;
		return;
	}
	else
	{
		fprintf (pFile, "MNI Tag Point File\nVolumes = 1;\n\nPoints =\n");
		int tagnum= (goodtagsVect.size())/3;	//number of tags (x,y,z)
		std::cout <<"tagnum: " << tagsVect.size() << std::endl;
		
		//remove any duplication of tag points:
		QVector <float> newtagsVect;
		std::cout <<"newtagnum: " << newtagsVect.size() << std::endl;
		newtagsVect.append(goodtagsVect[0]);
		newtagsVect.append(goodtagsVect[1]);
		newtagsVect.append(goodtagsVect[2]);
		bool tagfound;
		for (int i=1; i<tagnum ; i++){
			tagfound=false;
			for (int j=0; j< (newtagsVect.size())/3; j++){
				if (goodtagsVect[3*i]==newtagsVect[3*j] || goodtagsVect[3*i+1]==newtagsVect[3*j+1] || goodtagsVect[3*i+2]==newtagsVect[3*j+2]){
					tagfound=true;
				}
			}
			if (!tagfound){
				newtagsVect.append(goodtagsVect[3*i]);
				newtagsVect.append(goodtagsVect[3*i+1]);
				newtagsVect.append(goodtagsVect[3*i+2]);	
			}
		}
		
		tagnum= (newtagsVect.size())/3;
		std::cout <<"newtagnum: " << newtagsVect.size() << std::endl;
		for (int i=0; i<(tagnum-1) ; i++){
			fprintf (pFile, "%f %f %f 0.1 1 1 \"Marker\"\n", newtagsVect[3*i], newtagsVect[3*i+1],newtagsVect[3*i+2]);
		}
		fprintf (pFile, "%f %f %f 0.1 1 1 \"Marker\";", newtagsVect[3*(tagnum-1)], newtagsVect[3*(tagnum-1)+1],newtagsVect[3*(tagnum-1)+2]);
		fclose(pFile);
	}
}

void MainWindow::quitApplication() {
	// code to prompt for exit, save stuff, etc. could go here
	qDebug() << "Debug: Closing ... " ;
	exit(0);
}

