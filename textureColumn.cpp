#include "textureColumn.h"

textureColumn::textureColumn(SoSwitch *root, const QVector <QVariant> &data,
			    ResourceForm *rf, TreeItem *parent)
	: TreeItem(data, rf, parent) {

	low = new float(0.0);
	high = new float(0.0);
	minValue = new float(0.0);
	maxValue = new float(0.0);
	textureGroup = new SoGroup;

	textureSwitch = root;
	root->addChild(textureGroup);

	textureComplexity = new SoComplexity;
	textureComplexity->textureQuality = 1;
	textureGroup->addChild(textureComplexity);

	textureImage = new SoTexture2;
	//textureImage->filename.setValue("/tmp/spectral.png");
	textureImage->wrapT = SoTexture2::CLAMP;
	textureImage->wrapS = SoTexture2::CLAMP;
	textureGroup->addChild(textureImage);

	texture = new SoTextureCoordinate2;
	this->data = new vertexColumn;
	textureGroup->addChild(texture);

	textureBinding = new SoTextureCoordinateBinding;
	textureBinding->value.setValue(SoTextureCoordinateBinding::PER_VERTEX_INDEXED);
	textureGroup->addChild(textureBinding);

	textureFiles = new QVector<QString>;
	fillTextureVector();
		
		
	haveProps = true;
	formInstantiated = false;
	//root->whichChild = 0;
}

void textureColumn::fillTextureVector() {
        // get the path to the textures from either the
        // settings or the default path
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "MINC", "brain-view");
        QString defaultDir = QString(QFileInfo(settings.fileName()).path()).append("/textures/");
        QString textureDir = settings.value("textureDir", defaultDir).toString();
        std::cout << "USING TEXTUREDIR: " << textureDir.toStdString() << std::endl;
        settings.setValue("textureDir", textureDir);
	QDir d = QDir(textureDir);
	QFileInfoList list = d.entryInfoList();
	for (int i=0; i<list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);
		QString fileName = fileInfo.fileName();
		if (fileName.endsWith(".png")) {
			fileName.prepend("/");
			fileName.prepend(textureDir);
			cout << fileName.toStdString() << endl;
			textureFiles->append(fileName);
		}
	}
}
	
QWidget* textureColumn::createForm() {
	if (!formInstantiated) {
		formWidget = new QWidget;
		ui.setupUi(formWidget);
		formInstantiated = true;
                /*
		ui.upperSpinBox->setValue(*high);
		ui.lowerSpinBox->setValue(*low);
		ui.upperSpinBox->setRange(*minValue, *maxValue);
		ui.lowerSpinBox->setRange(*minValue, *maxValue);
		ui.upperSpinBox->setSingleStep(0.05);
		ui.lowerSpinBox->setSingleStep(0.05);
                */

                // the validator does not appear to actually work - hrm?
                const QDoubleValidator dv((double)*low, (double)*high, 3, this);
                ui.lowerLimit->setValidator(&dv);
                ui.upperLimit->setValidator(&dv);
                ui.lowerLimit->setText(QString::number(*low));
                ui.upperLimit->setText(QString::number(*high));
		QFile texFile(textureImage->filename.getValue().getString());

		ui.imageLabel->setBackgroundRole(QPalette::Base);
                ui.imageLabel->setScaledContents(true);
		
		// populate the combo box with texture filenames
		for (int i=0; i < textureFiles->size(); ++i) {
			ui.textureCombo->addItem(textureFiles->at(i));
		}

		// connect signals and slots
                connect(ui.upperLimit, SIGNAL(editingFinished()),
                                this, SLOT(upperValueChanged()));
                connect(ui.lowerLimit, SIGNAL(editingFinished()),
                                this, SLOT(lowerValueChanged()));
		connect(ui.textureCombo, SIGNAL(currentIndexChanged(int)),
				this, SLOT(newTextureSelected(int)));
		
		// set default texture to be the first one.
		newTextureSelected(0);
	}
	return formWidget;
}

void textureColumn::newTextureSelected(int index) {
	// set the texture in Coin
	textureImage->filename.setValue(textureFiles->at(index).toLatin1());
	// and set the texture in the resourceForm
	QImage image(textureFiles->at(index));
	ui.imageLabel->setPixmap(QPixmap::fromImage(image));
}

void textureColumn::destroyForm() {
	if (formInstantiated) {
		delete formWidget;
		formInstantiated = false;
	}
}

void textureColumn::upperValueChanged() {
        *high = ui.upperLimit->text().toFloat();
	scaleTexture(*low, *high);
}

void textureColumn::lowerValueChanged() {
        *low = ui.lowerLimit->text().toFloat();
	scaleTexture(*low, *high);
}

void textureColumn::wasSelected() {
	textureSwitch->whichChild = this->childNumber();
}

void textureColumn::loadTextureColumn(mniVertstatsFile *file, QString columnName) {
	setData(0, columnName);
	*this->data = file->getDataColumn(columnName.toLatin1().data());
	// determine the low and high
	vertexColumn::iterator it;
	*this->low = *data->begin();
	*this->high = *this->low;
	for (it = data->begin(); it < data->end(); it++) {
		if ( *it > *high )
			*high = *it;
		else if ( *it < *low )
			*low = *it;
	}
	*minValue = *low;
	*maxValue = *high;
	scaleTexture(*low, *high);
}

void textureColumn::scaleTexture(float low, float high) {
	*this->low = low;
	*this->high = high;
	vertexColumn::iterator it;
	int i = 0;
	//SbVec2f *vertValues = texture->point.startEditing();
	for (it = data->begin(); it < data->end(); it++) {
		float newVal = ( *it - *this->low ) / ( *this->high - *this->low);
		if (newVal < 0.0) {
			newVal = 0.0;
		}
		else if (newVal > 1.0) {
			newVal = 1.0;
		}
		//vertValues[i].setValue(0.5, newVal);
		texture->point.set1Value(i, SbVec2f(0.5, newVal));
		i++;
	}
	//texture->point.finishEditing();

}

textureColumn::~textureColumn()
{
}
