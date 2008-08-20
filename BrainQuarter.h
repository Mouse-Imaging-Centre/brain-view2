#ifndef BRAINQUARTER_H_
#define BRAINQUARTER_H_

#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoSeparator.h>

#include <Quarter/QuarterWidget.h>

using namespace SIM::Coin3D::Quarter;

class BrainQuarter : public QuarterWidget {
public:
	BrainQuarter(QWidget * parent = 0, const QGLWidget * sharewidget = 0);
	virtual ~BrainQuarter();
	void createSampleSceneGraph();
	virtual QSize minimumSizeHint(void) const;
private:

	SoSeparator *root;
	SoBaseColor *col;
};

#endif /*BRAINQUARTER_H_*/
