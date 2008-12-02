#ifndef BRAINQUARTER_H_
#define BRAINQUARTER_H_

/* BrainQuarter
 * A simple subclass of the QuarterWidget at this moment in time.
 * Will eventually contain the functions to load different scene graphs.
 */


#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>

#include <Inventor/events/SoMouseButtonEvent.h>

#include <Quarter/QuarterWidget.h>
#include <iostream>

using namespace SIM::Coin3D::Quarter;

class BrainQuarter : public QuarterWidget {
public:
	// constructor
	BrainQuarter(QWidget * parent = 0, const QGLWidget * sharewidget = 0);
	// destructor
	virtual ~BrainQuarter();
	// creates a yellow cone - i.e. just for testing purposes.
	void createSampleSceneGraph();
	SoSeparator* getRootSeparator();
	// don't let the window get too small
	virtual QSize minimumSizeHint(void) const;
private:
	// bits for the sample scene graph
	SoSeparator *root;
        SoBaseColor *col;
        SoPerspectiveCamera *pcam;
        SoEventCallback *ecb;
};

#endif /*BRAINQUARTER_H_*/
