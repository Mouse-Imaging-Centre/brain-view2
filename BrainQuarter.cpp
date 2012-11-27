#include "BrainQuarter.h"
#include "Inventor/actions/SoGLRenderAction.h"
#include <Inventor/SoRenderManager.h>
#include "Inventor/nodes/SoDirectionalLight.h"
#include "Inventor/SoRenderManager.h"

#include "Inventor/details/SoFaceDetail.h"
#include "Inventor/details/SoPointDetail.h"
#include <Inventor/details/SoLineDetail.h>
#include <Inventor/details/SoCylinderDetail.h>


#include <QtDebug>
#include <iostream>
#include <sstream>
#include <string>
#include <QtGui>

/* Callback function for when the mouse is clicked somewhere on the scene.
   It applies a ray-pick action, gets the point found, then calls a method
   from the viewer which in turn will do the appropriate things with the
   picked point
*/
static void event_cb(void *ud, SoEventCallback *n) {

	const SoMouseButtonEvent *mbe = (SoMouseButtonEvent *)n->getEvent();
	if (mbe->getButton() == SoMouseButtonEvent::BUTTON1 &&
        mbe->getState() == SoButtonEvent::DOWN) {
       BrainQuarter *viewer = (BrainQuarter *)ud;
		SoRayPickAction rp(viewer->getSoRenderManager()->getViewportRegion());
	  // set an 8 pixed wide region for the search
        rp.setPoint(mbe->getPosition());
		//rp.setPoint(cursorPosition);
		//rp.setRadius(8.0);
		rp.setPickAll(TRUE);	//all the objects the ray intersects with should be picked. If not, only the intersection point of the object closest to the camera will be picked.
		SoNode *newroot = viewer->getSceneGraph();
       rp.apply(viewer->getSceneGraph());
// 		rp.apply(viewer->getOverlaySceneGraph());
        SoPickedPoint *point = rp.getPickedPoint(0);
		SoPickedPoint *point2 = rp.getPickedPoint(1);
        //n->setHandled();
        viewer->getPickedPoint(point, point2);

    }
}

BrainQuarter::BrainQuarter(QWidget *parent) : QuarterWidget(parent) {
    root = new SoSeparator;
    root->ref();
    pcam = new SoPerspectiveCamera;
    root->addChild(pcam);

	// light
 	//root->addChild(new SoDirectionalLight);

    ecb = new SoEventCallback;
    ecb->addEventCallback(SoMouseButtonEvent::getClassTypeId(), event_cb, this);
    root->addChild(ecb);
	

    setNavigationModeFile();
    setSceneGraph(root);
	pickedtag= (float*) malloc(sizeof(float) * 3); //(float*) calloc(3, sizeof(float));
    setTransparencyType(SORTED_OBJECT_SORTED_TRIANGLE_BLEND);
	//setBackgroundColor 	( QColor(255.0,255.0,255.0));  // void SoRenderManager::setBackgroundColor 	( 	const SbColor4f &  	color 	 ) 	
	//createSampleSceneGraph();

	faceTagOpt = 1; //Default to tag polygon in middle point
// 	qDebug() << "Debug. <<BrainQuarter::BrainQuarter()";
}

void BrainQuarter::getPickedPoint(SoPickedPoint *point , SoPickedPoint *point2) {
	//qDebug() << "Debug. >>BrainQuarter::getPickedPoint()";
    // only do something if a the mouse cursor was over a valid piece of geometry
    if (point != NULL && point->isOnGeometry()) {

       // std::cout << "\nOn geometry ..." << std::endl;
        SbVec3f v = point->getPoint(); //the world space of the picked point.
		SbVec4f t = point->getTextureCoords();	//the image space texture coordinates. 
		int matindx = point->getMaterialIndex(); //the material index.
//        std::cout << "\nOn geometry ... Found: " << v[0] << " " << v[1] << " "<< v[2] << std::endl;
//         std::cout << "Texture: " << t[0] << " " << t[1] << " " << t[2] << " " << t[3] << std::endl;
// 		std::cout << "Material Index " << matindx << std::endl;
        SoPath *path = point->getPath();	//the path to the picked object. //
		SoNode * tail = static_cast<SoFullPath*>(path)->getTail(); //SoPath::getTail() will return the first node in the path.  If you need the actual tail node (with hidden children), you need to cast the path to SoFullPath

//         std::cout << "ID: " << path->getTail()->getNodeId() << std::endl;
		SbVec3f objIntersect = point->getObjectPoint();//the object space point, in the object space specified by node. If node equals NULL, the object space of the node where the point was actually picked will be used (this is what one would usually be interested in)
		const SoDetail *objdetail = point->getDetail();  	
		SoType objtype = objdetail->getTypeId();

		if (objtype == SoFaceDetail::getClassTypeId()) {
		///// if object is polygon:
			//std::cout << "\nPicked point on Polygon obejct!" << faceTagOpt <<std::endl;
			SoFaceDetail *fd = new SoFaceDetail();
			//SoFaceDetail *fd = (SoFaceDetail *) point->getDetail();  //fd is faceDetail
			fd = (SoFaceDetail *) point->getDetail();
// 			//std::cout << "Picker - num points: " << fd->getNumPoints() << std::endl; //Number of vertices making up the polygon. 
			//if object is polygon Picker - num points: 4
 			//if (fd->getNumPoints() ==4) {
			const SoPointDetail *pd = fd->getPoint(0); //getPoint(const int idx) Returns a pointer into the array of vertices, starting at the idx'th  vertice of the polygon
			/////const SoPointDetail* SoFaceDetail::getPoint(int) const: Assertion `idx >= 0 && idx < this->numpoints' 
// 			std::cout << "Index: " << pd->getCoordinateIndex() << std::endl; //Returns index into coordinate set for the point's 3D coordinates. 
			
			SbVec3f v2 = point2->getPoint(); //the world space of the picked point.
			if (faceTagOpt ==0){
			pickedtag[0]=v[0]; 
			pickedtag[1]=v[1];
			pickedtag[2]=v[2];
			}		
			else if (faceTagOpt ==1 ){
			pickedtag[0]=(v[0]+v2[0])/2; 
			pickedtag[1]=(v[1]+v2[1])/2;
			pickedtag[2]=(v[2]+v2[2])/2;
			}
			else if (faceTagOpt == 2) {
			pickedtag[0]=v2[0]; 
			pickedtag[1]=v2[1];
			pickedtag[2]=v2[2];
			}
			
// 			std::cout << "Found point1: " << v[0] << " " << v[1] << " "<< v[2] << std::endl;
// 			std::cout << "Found point2: " << v2[0] << " " << v2[1] << " "<< v2[2] << std::endl;
//			std::cout << "Center tag: " << pickedtag[0] << " " << pickedtag[1] << " "<< pickedtag[2] << std::endl<< std::endl;
			
// 			const SoPointDetail *pd2 = fd->getPoint(2); //getPoint(const int idx) Returns a pointer into the array of vertices, starting at the idx'th  vertice of the polygon
// 			std::cout << "Index pd2: " << pd2->getCoordinateIndex() << std::endl; //Returns index into coordinate set for the point's 3D coordinates. 

			// emit a signal containing the coordinate picked and ID of the bit of geometry containing
			// this point. This ID will be used by the slots attached to this signal to decide whether
			// to do anything or not.
			emit pointPicked(pd->getCoordinateIndex(), path->getTail()->getNodeId(),tail->getNodeId(), pickedtag[0], pickedtag[1], pickedtag[2], objtype);
		}
		

		else if (objtype == SoLineDetail::getClassTypeId()) {
		//if obejct is line:
			//std::cout << "\nPicked point on Line obejct!" <<std::endl;
			SoLineDetail *ld = new SoLineDetail();
			ld = (SoLineDetail *) point->getDetail();
			//std::cout << "Picker - num points: " << ld->getNumPoints() << std::endl;
			const SoPointDetail *pd0 = ld->getPoint0(); //Returns SoPointDetail describing the line start point. 
 			const SoPointDetail *pd1 = ld->getPoint1(); //Returns SoPointDetail describing the line end point.
			std::cout << "(Index p0-p1) graph edge =[" << pd0->getCoordinateIndex() <<"," << pd1->getCoordinateIndex() <<"]"<< std::endl;
// 			std::cout << "Index p1: " << pd1->getCoordinateIndex() << std::endl;
// 			std::cout << "material Index p0: " << pd0->getMaterialIndex() << std::endl;
// 			std::cout << "material Index p1: " << pd1->getMaterialIndex() << std::endl;

			pickedtag[0]=v[0]; 
			pickedtag[1]=v[1];
			pickedtag[2]=v[2];
// 			std::cout << "Found point1: " << v[0] << " " << v[1] << " "<< v[2] << std::endl;
// 			std::cout << "Found point2: " << v2[0] << " " << v2[1] << " "<< v2[2] << std::endl;
//			std::cout << "Center tag: " << pickedtag[0] << " " << pickedtag[1] << " "<< pickedtag[2] << std::endl<< std::endl;
			// emit a signal containing the coordinate picked and ID of the bit of geometry containing
			// this point. This ID will be used by the slots attached to this signal to decide whether
			// to do anything or not.
			emit pointPicked(pd0->getCoordinateIndex(), path->getTail()->getNodeId(),tail->getNodeId(), v[0], v[1], v[2], objtype); //Returns index into coordinate set for the point's 3D coordinates. 
		}
		
		else if (objtype == SoCylinderDetail::getClassTypeId()) {
			//std::cout << "\nPicked point on Cylinder obejct!" <<std::endl;
			SoCylinderDetail *cd = new SoCylinderDetail();
			cd = (SoCylinderDetail *) point->getDetail();
			//int cylpart = cd->getPart;
			emit pointPicked(path->getTail()->getNodeId(), path->getTail()->getNodeId(),tail->getNodeId(), v[0], v[1], v[2], objtype);
		}
		//elseif not LINES or POLYGONS or Cylinder: it has not been read and executed in GeometryNode. so no need to worry about them here!
	}
	
	else {
		//std::cout << "\nNot on geometry ..." << std::endl;
		emit pointNotPicked();
	}
// 	qDebug() << "Debug. <<BrainQuarter::getPickedPoint()";
}

BrainQuarter::~BrainQuarter() {
	root->unref();
}


//BrainQuarter::getViewportRegion() {
 //   return pcam->getViewportBounds(

SoSeparator* BrainQuarter::getRootSeparator() {
// 	qDebug() << "Debug. >>BrainQuarter::getRootSeparator()";
	return root;
// 	qDebug() << "Debug. <<BrainQuarter::getRootSeparator()";
}

// create the sample scene - a yellow cone
void BrainQuarter::createSampleSceneGraph() {
	//qDebug() << "Debug. >>BrainQuarter::createSampleSceneGraph()";
    root = new SoSeparator;
    root->ref();

    col = new SoBaseColor;
    col->rgb = SbColor(1, 1, 0);
    root->addChild(col);

    root->addChild(new SoCone);
    setSceneGraph(root);
	//qDebug() << "Debug. <<BrainQuarter::createSampleSceneGraph()";
}

QSize BrainQuarter::minimumSizeHint(void) const
{
	//qDebug() << "Debug. >>BrainQuarter::minimumSizeHint()";
	return QSize(640, 480);
	//qDebug() << "Debug. <<BrainQuarter::minimumSizeHint()";
}
