#include "BrainQuarter.h"

BrainQuarter::BrainQuarter(QWidget *parent, const QGLWidget *sharewidget) : QuarterWidget(parent, sharewidget) {
	createSampleSceneGraph();
}

BrainQuarter::~BrainQuarter() {
}

// create the sample scene - a yellow cone
void BrainQuarter::createSampleSceneGraph() {
    root = new SoSeparator;
    root->ref();

    col = new SoBaseColor;
    col->rgb = SbColor(1, 1, 0);
    root->addChild(col);

    root->addChild(new SoCone);
    setSceneGraph(root);    
}

QSize BrainQuarter::minimumSizeHint(void) const
{
  return QSize(640, 480);
}
