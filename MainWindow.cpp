#include <QtDebug>

#include "MainWindow.h"
#include "TreeModel.h"

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
    if (ext == "obj") {
        status = resourceForm->insertGeometry(filename);
    }
    else if (ext == "txt" || ext == "vertstats") {
        status = resourceForm->insertVertstats(filename);
    }
    else if (ext == "tag") {
        status = resourceForm->insertTagfile(filename);
    }

	Q_ASSERT_X( status == true, "MainWindow::openFile", "Failed insert into Resource form" );
	qDebug() << "Debug. <<MainWindow::openFile()";
	return true;
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
	qDebug() << "Debug. >>MainWindow::createActions()";

	openAction = new QAction(tr("&Open File"), this);
	connect(openAction, SIGNAL(triggered()), this, SLOT(openFileFromDialog()));

	quitAction = new QAction(tr("&Quit"), this);
	connect(quitAction, SIGNAL(triggered()), this, SLOT(quitApplication()));

	qDebug() << "Debug. <<MainWindow::createActions()";
}


void MainWindow::createMenus() {
	qDebug() << "Debug. >>MainWindow::createMenus()";

	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(openAction);
	fileMenu->addAction(quitAction);

	qDebug() << "Debug. <<MainWindow::createMenus()";
}


void MainWindow::quitApplication() {
	// code to prompt for exit, save stuff, etc. could go here
	exit(0);
}

