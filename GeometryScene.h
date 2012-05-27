#ifndef GEOMETRYSCENE_H_
#define GEOMETRYSCENE_H_

#include "TreeItem.h"
#include "GeometryNode.h"
#include "textureFileItem.h"
#include "Inventor/nodes/SoSeparator.h"
//#include <bicpl.h>
#include <stdio.h>    //FILE, fopen(), fclose()
#include <stdlib.h>
#include <sys/stat.h> //stat, stat()
#include <string>     //string
#include <fstream>    //fstream 
#include <iostream>   //cout
// #include "hdf5.h"		//C API
#include "H5Cpp.h"		//C++ API
#include "H5File.h"
#include <QVector>
// #include <QFile>
// #include <QTextStream>

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
//  	bool loadLabelLU(QFile &file);
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
	
// 	typedef struct  {
//       size_t len; /* Length of VL data (in base type units) */      
//       QVector <SoMaterial *> p;    /* Pointer to VL data */        
// 	} hvl_tSoMat;

	struct h5_output_type {
		int num_edge;
		int num_vertex;
		hvl_t *centreX; hvl_t *centreY; hvl_t *centreZ;
		hvl_t *tangentX; hvl_t *tangentY; hvl_t *tangentZ;
		hvl_t *radius;
		hvl_t *heights;
		int *label;
		//int *id;
		int *edges;
		float *vertices;
// 		h5_output_type() : height({0.0}) {}		//initializing height to default value*/
	};
	
	//struct h5_output_type * H5_reader (char* dbfile);
	struct h5_output_type H5_reader (char* dbfile);
	
	int num_edge_total;
	
	
// 	struct load_Cylinder_output_type {
// 		int nodeid;
// 		SoMaterial *nodematerial;
// // 		load_Cylinder_output_type : nodematerial(new SoMaterial) {}
// 	};
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
		void getUserLabel();
		void updateRadiusTransparency(int newVal);
		
// 		 // called when an object (not necessarily in this geometryScene) was picked to be labeled
//         void assignLabel(int index, int id, int x, int y, int z);
signals:
         void pointPickedColor(int indx1, int indx2, float r, float g, float b);
private:
	
	void radiusCalc(hvl_t *cylradiusVect);
	ResourceForm *rf;
	// the root separator from the viewer
	SoSeparator *root;
	// the separator which will hold all the geometry bits
	SoSeparator *scene;
	// the actual geometry bits
	GeometryNode *geometry;
	
	QVector<QVariant> data;

	hvl_ti * nodeIDs;
	int nodeId;
	SoMaterial *nodematerial;
	/*hvl_tSoMat *cylmatVect;*/ 	
	QVector <QVector <SoMaterial *> > cylmatVect;
	hvl_t *cylradiusVect;  	//QVector <float> cylradiusVect;
	QVector <int> labelVect;
	QVector <int> edgesVect;
	QVector <int> new_edgesVect;
	QVector <float> new_radius;
	QVector <float> new_heights;
	QVector <float> new_tangx; QVector <float> new_tangy; QVector <float> new_tangz;
	QVector <float> new_cntrx; QVector <float> new_cntry; QVector <float> new_cntrz;
	QVector <float> vertices_centreVect;
	
	QVector <int> connectingVect;
	bool flag_new_edge;
	int num_clicked;
	int edge1_indx;
	int edge2_indx;
	
	float radiusmax;
	float radiusmin;

	QWidget *labelWidget;
	QVBoxLayout *labelLayout;

	//QVector <int> segmentIDVect;
	QVector <int> cylnum_transparency0_5;



};

#endif /*GEOMETRYSCENE_H_*/
