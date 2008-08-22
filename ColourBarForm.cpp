#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoSeparator.h>


#include "ColourBarForm.h"

ColourBarForm::ColourBarForm(QWidget *parent) : QWidget(parent) {
	ui.setupUi(this);
	
	// the famous yellow cone as a place holder
    SoSeparator *root = new SoSeparator;
    root->ref();

    SoBaseColor *col = new SoBaseColor;
    col->rgb = SbColor(1, 1, 0);
    root->addChild(col);

    root->addChild(new SoCone);
    ui.colourBarViewer->setSceneGraph(root); 
	
}

ColourBarForm::~ColourBarForm() {
}
