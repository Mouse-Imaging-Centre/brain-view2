#include "BrainQuarter.h"
#include "Inventor/actions/SoGLRenderAction.h"
#include "Inventor/SoPickedPoint.h"
#include "Quarter/SoRenderManager.h"
#include "Inventor/details/SoFaceDetail.h"
#include "Inventor/details/SoPointDetail.h"

static void event_cb(void *ud, SoEventCallback *n) {
    std::cout << "Woooohooo!" << std::endl;
    const SoMouseButtonEvent *mbe = (SoMouseButtonEvent *)n->getEvent();
    if (mbe->getButton() == SoMouseButtonEvent::BUTTON1 &&
        mbe->getState() == SoButtonEvent::DOWN) {
        BrainQuarter *viewer = (BrainQuarter *)ud;
        SoRayPickAction rp(viewer->getSoRenderManager()->getViewportRegion());
        rp.setPoint(mbe->getPosition());
        rp.apply(viewer->getSceneGraph());
        SoPickedPoint *point = rp.getPickedPoint();
        //n->setHandled();
        if (point != NULL && point->isOnGeometry()) {
            std::cout << "On geometry ..." << std::endl;
            SbVec3f v = point->getPoint();
            SbVec4f t = point->getTextureCoords();
            std::cout << "Found: " << v[0] << " " << v[1] << " "<< v[2] << std::endl;
            std::cout << "Texture: " << t[0] << " " << t[1] << " " << t[2] << " " << t[3] << std::endl;

            SoFaceDetail *fd = (SoFaceDetail *) point->getDetail();
            const SoPointDetail *pd = fd->getPoint(0);
            std::cout << "Index: " << pd->getCoordinateIndex() << std::endl;

            SoPath *path = point->getPath();
            std::cout << "ID: " << path->getTail()->getNodeId() << std::endl;

        }
    }
}

BrainQuarter::BrainQuarter(QWidget *parent, const QGLWidget *sharewidget) : QuarterWidget(parent, sharewidget) {
    root = new SoSeparator;
    root->ref();
    pcam = new SoPerspectiveCamera;
    root->addChild(pcam);

    ecb = new SoEventCallback;
    ecb->addEventCallback(SoMouseButtonEvent::getClassTypeId(), event_cb, this);
    root->addChild(ecb);

    setSceneGraph(root); 
    setTransparencyType(SoGLRenderAction:: SORTED_OBJECT_SORTED_TRIANGLE_BLEND);
	//createSampleSceneGraph();
}

BrainQuarter::~BrainQuarter() {
}

//BrainQuarter::getViewportRegion() {
 //   return pcam->getViewportBounds(

SoSeparator* BrainQuarter::getRootSeparator() {
	return root;
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
