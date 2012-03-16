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
#include <Inventor/SoPickedPoint.h>

#include <Inventor/events/SoMouseButtonEvent.h>

#include <Quarter/QuarterWidget.h>
#include <iostream>
#include <sstream>
#include <string>
#include <QtDebug>
#include <QtGui>

using namespace SIM::Coin3D::Quarter;

class BrainQuarter : public QuarterWidget {
    Q_OBJECT
public:
	// constructor
        BrainQuarter(QWidget * parent = 0);
	// destructor
	virtual ~BrainQuarter();
	// creates a yellow cone - i.e. just for testing purposes.
	void createSampleSceneGraph();
	SoSeparator* getRootSeparator();
	float * pickedtag;
	// don't let the window get too small
        virtual QSize minimumSizeHint(void) const;
        void getPickedPoint(SoPickedPoint *point,SoPickedPoint *point2);
signals:
        void pointPicked(int index, int id, int x, int y, int z,SoType objtype);
		void pointNotPicked();

private:
	// bits for the sample scene graph
	SoSeparator *root;
        SoBaseColor *col;
        SoPerspectiveCamera *pcam;
        SoEventCallback *ecb;
};

#endif /*BRAINQUARTER_H_*/
