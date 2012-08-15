#ifndef GEOMETRYSCENE_H_
#define GEOMETRYSCENE_H_

#include "TreeItem.h"
#include "GeometryNode.h"
#include "textureFileItem.h"
#include "Inventor/nodes/SoSeparator.h"
//#include <bicpl.h>
#include <stdio.h>    
#include <stdlib.h>
#include <sys/stat.h> 
#include <string>     
#include <fstream>    
#include <iostream>   
// #include "hdf5.h"		/* C API */
#include "H5Cpp.h"		/* C++ API */
#include "H5File.h"
#include <QVector>


#include "ui_labelProps.h"
#include <QtGui>

class GeometryScene : public TreeItem {
//     Q_OBJECT
public:
	GeometryScene(const QVector<QVariant> &data,
			ResourceForm *rf, TreeItem *parent = 0);
	virtual ~GeometryScene();
	Ui::labelProps uilabel;
	/*QWidget **/ void createLabelForm();

	void saveh5FileFunc(QFile &filename);
	bool loadGeometry(QFile &file, const QVector<QVariant> &data,
			ResourceForm *rf, TreeItem *parent = 0);
	bool loadVertstats(QFile &file);
	bool updateLabelLUColor();
	int whichEdgeIndx (int edge_ind1,int edge_ind2,int x, int y,int z);

	void wasSelected();
	QString h5_filename;
	QString label_num2Name (int label);
     int label_Name2num (QString labelName);		
	bool edgeExists(int vertx0,int vertx1,QVector <int> Vect);
	
	typedef struct  {
      size_t len; /* Length of VL data (in base type units) */      
      float *p;    /* Pointer to VL data */        
	} hvl_t;

	typedef struct  {
      size_t len; /* Length of VL data (in base type units) */      
      int *p;    /* Pointer to VL data */        
	} hvl_ti;


	struct h5_output_type {
		int num_edge;
		int num_vertex;
		hvl_t *centreX; hvl_t *centreY; hvl_t *centreZ;
		hvl_t *tangentX; hvl_t *tangentY; hvl_t *tangentZ;
		hvl_t *radius;
		hvl_t *heights;
		int *label;
		int *edges;
		float *vertices;
		//h5_output_type() : height({0.0}) {}		/*initializing height to default value */
	};
	
	//struct h5_output_type * H5_reader (char* dbfile);
	struct h5_output_type H5_reader (char* dbfile);
	
	int num_edge_total;
	
	

//signals:
        // signal emitted if the picked point in the scene belongs to this scenegraph
        //void localPointPicked(int index);
public slots:
        // called when an object (not necessarily in this geometryScene) was picked
        void pickReceived(int index, int id, float x, float y, float z,SoType objtype);
		void noPointReceived();
		void saveLabel();
		void saveasLabel();
		void createNewEdge();
		void addNewEdge();
		void deleteThisEdge();
		void getUserLabel();
		void updateRadiusTransparency(int newVal);
		
// signals:
//          //void pointPickedColor(int indx1, int indx2, float r, float g, float b);
// 	    void createEdgeSignal();
private:
	
	void radiusCalc(QVector <QVector <float> > cylradiusVect);
	ResourceForm *rf;
	/* the root separator from the viewer */
	SoSeparator *root;
	/* the separator which will hold all the geometry bits */
	SoSeparator *scene;
	/* the actual geometry bits */
	GeometryNode *geometry;
	
	QVector<QVariant> data;

	QVector< QVector <int> > nodeIDs;
	QVector< QVector <int> > childIDs;
	int nodeId;
	int e_num;
	QVector <QVector <SoMaterial *> > cylmatVect;
	QVector <QVector <float> > cylradiusVect;	//hvl_t *cylradiusVect;  	
	
	QVector <int> labelVect;
	QVector <int> edgesVect;
	QVector <int> new_edgesVect;
	QVector <float> new_radius;
	QVector <float> new_heights;
	QVector <float> new_cntrx; QVector <float> new_cntry; QVector <float> new_cntrz;
	QVector <float> new_tangx; QVector <float> new_tangy; QVector <float> new_tangz;
	QVector <float> vertices_centreVect;
	
	QVector <int> del_edgesVect;

	QVector <int> connectingVect;
	bool flag_new_edge;
	int num_clicked;
	int edge1_indx;
	int edge2_indx;
	
	float radiusmax;
	float radiusmin;

	QWidget *labelWidget;
	QVBoxLayout *labelLayout;

	QVector <int> cylnum_transparency0_5;

	QVector <SoMaterial *> tmp_cylmatVect;
	QVector <float> tmp_cylradiusVect;
	QVector <int> tmp_nodeid;
	QVector <int> tmp_childid;


};

#endif /*GEOMETRYSCENE_H_*/
