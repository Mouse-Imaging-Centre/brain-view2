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
	//std::cout << "PID: " << QCoreApplication::applicationPid() << std::endl;
	
	createActions();
	createMenus();
        initColourbars();

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

void MainWindow::initColourbars() {
    // get the path from the settings
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "MINC", "brain-view");
    QString defaultDir = QString(QFileInfo(settings.fileName()).path()).append("/textures/");
    QString textureDir = settings.value("textureDir", defaultDir).toString();
    settings.setValue("textureDir", textureDir);
    QDir td(textureDir);

    std::cout << "TextureDir: " << textureDir.toStdString() << std::endl;

    // create the texture dir if it does not already exist
    if (! td.exists()) {
        td.mkpath(textureDir);
    }

    // for every colourbar in the resource file check to see if it
    // already exists. If not, write it out to file
    QDir d(":/resources/colourbars");
    QFileInfoList list = d.entryInfoList();
    for (int i=0; i<list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        QString fileName = fileInfo.baseName();
        std::cout << "Basename: " << fileName.toStdString() << std::endl;
        fileName.append(".png");
        fileName.prepend("/");
        fileName.prepend(textureDir);
        if (! QFile(fileName).exists()) {
            std::cout << "FILENAME: " << fileInfo.filePath().toStdString() << std::endl;
            QImage i(fileInfo.filePath());
            bool a = i.isNull();
            bool b = i.save(fileName);
            std::cout << "Created colourbar: " << fileName.toStdString() << " " << a << " " << b << std::endl;
        }
    }
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
    else if (ext == "tag") {
        return resourceForm->insertTagfile(filename);
    }
    else {
        return false;
    }
}

void MainWindow::openFileFromDialog() {
	QString name = QFileDialog::getOpenFileName(this,
                tr("Open File"), QString(), tr("Files (*.obj *.txt *.vertstats *.tag)"));
	if (! name.isNull()) {
		QFile filename(name);
		openFile(filename);
	}
}

void MainWindow::createActions() {
	openAction = new QAction(tr("&Open File"), this);
        connect(openAction, SIGNAL(triggered()), this, SLOT(openFileFromDialog()));

        quitAction = new QAction(tr("&Quit"), this);
        connect(quitAction, SIGNAL(triggered()), this, SLOT(quitApplication()));
}

void MainWindow::quitApplication() {
        // code to prompt for exit, save stuff, etc. could go here
        exit(0);
}

void MainWindow::createMenus() {
	fileMenu = menuBar()->addMenu(tr("&File"));
        fileMenu->addAction(openAction);
        fileMenu->addAction(quitAction);
}


