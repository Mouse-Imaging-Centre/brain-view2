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

	bool loadGeometry(QFile &file, const QVector<QVariant> &data,
			ResourceForm *rf, TreeItem *parent = 0);
	bool loadVertstats(QFile &file);
	bool updateLabelLUColor();
//  	bool loadLabelLU(QFile &file);
	void wasSelected();
	QString h5_filename;
	QString label_num2Name (int label);
     int label_Name2num (QString labelName);		
		
	struct h5_output_type {
		unsigned long dimension;		
		int *segment_ID;
// 		unsigned long *edges;
		float *centres;
		float *tangents; 
		float *radius;
		float *heights;
		int *label;
		//char *label;
// 		h5_output_type() : height({0.0}) {}		//initializing height to default value*/
	};
	
	struct h5_output_type * H5_reader (char* dbfile);
	
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
        void pickReceived(int index, int id, int x, int y, int z,SoType objtype);
		void noPointReceived();
		void saveLabel();
		void getUserLabel();
		void updateRadiusTransparency(int newVal);
		
// 		 // called when an object (not necessarily in this geometryScene) was picked to be labeled
//         void assignLabel(int index, int id, int x, int y, int z);
signals:
         void pointPickedColor(int indx1, int indx2, float r, float g, float b);
private:
	
	void radiusCalc(QVector <float> cylradiusVect);
	ResourceForm *rf;
	// the root separator from the viewer
	SoSeparator *root;
	// the separator which will hold all the geometry bits
	SoSeparator *scene;
	// the actual geometry bits
	GeometryNode *geometry;
	

	int * nodeIDs;
	SoMaterial *nodematerial;
	QVector <SoMaterial *> cylmatVect;
	QVector <float> cylradiusVect;
	float radiusmax;
	float radiusmin;
	int radsize;
	QWidget *labelWidget;
	QVBoxLayout *labelLayout;
	int nodeId;
	QVector <int> labelVect;
	//QVector <QString> labelVect;
	QVector <int> segmentIDVect;
	QVector <int> cylnum_transparency0_5;
// 	QVector <int> have_h5;

/*
	//for labale.config file
	QVector <int> labelnums;
	QVector <QString> labelnames;
	QVector <float> labelred;
	QVector <float> labelgreen;
	QVector <float> labelblue;*/
};

#endif /*GEOMETRYSCENE_H_*/
