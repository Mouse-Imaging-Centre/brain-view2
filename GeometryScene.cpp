#include <QtDebug>
#include "GeometryScene.h"
#include "GeometryNode.h"
#include <stdio.h>    //FILE, fopen(), fclose()
#include <stdlib.h>
#include <sys/stat.h> //stat, stat()
#include <string>     //string
#include <fstream>    //fstream 
#include <iostream>   //cout
// #include "hdf5.h"		//C API
#include "H5Cpp.h"		//C++ API
#include "H5File.h"
// #include "H5Gloc.h"
#include "math.h"
#include <QVector>

#include "Inventor/details/SoFaceDetail.h"
#include "Inventor/details/SoPointDetail.h"
#include <Inventor/details/SoLineDetail.h>
#include <Inventor/details/SoCylinderDetail.h>

#include "Inventor/nodes/SoDirectionalLight.h"

// using namespace H5;

// forward declaration
SoSeparator* bic_graphics_file_to_iv( const char* filename );


GeometryScene::GeometryScene(const QVector<QVariant> &data,
		ResourceForm *rf, TreeItem *parent)
	: TreeItem(data, rf, parent){

	qDebug() << "Debug. >>GeometryScene::GeometryScene()";
	setData(0, "Geometry Scene");

	scene = new SoSeparator();
				// light
 	//scene->addChild(new SoDirectionalLight);
	
	geometry = new GeometryNode(scene, data, rf, this);
	childItems.insert(childCount(), geometry);
	this->rf = rf;
	qDebug() << "Debug. <<GeometryScene::GeometryScene()";
}



bool GeometryScene::loadGeometry(QFile &file, const QVector<QVariant> &data,
		ResourceForm *rf, TreeItem *parent){
	qDebug() << "Debug. >>GeometryScene::loadGeometry()";
	root = viewer->getRootSeparator();
	
	QFileInfo fileInfo(file);
    QString ext = fileInfo.suffix();


    if (ext == "h5") {
		std::cout << "Loading Line object as a cylinder: " <<std::endl;
		////read the h5 file and pass the radius and tangent height and center of vetex v
		struct h5_output_type *h5_output_data = H5_reader ((char*)file.fileName().toLatin1().data());
		std::cout << "Successfully read the h5 file! " <<std::endl;
	
		//save unique cylinder NodeId in order of cylinder numbers from GeometryNode::loadCylinder in here
		nodeIDs = (int*) malloc( sizeof(int) * (int)(h5_output_data->dimension) );	
		GeometryNode::load_Cylinder_output_type output_loadCylinder;
		
		float vrtxX; float vrtxY; float vrtxZ; float vrtx_radius; float tangentX; float tangentY; float tangentZ; float vrtx_height; /*QString lab;*/ int lab;
		int cyl_num=0;




// 		std::string testString (h5_output_data->label, 25*h5_output_data->dimension); 
// 		const char* testChar = testString.c_str();
// 		std::cout << "testChar[0] is: "<< testChar[0] << ", " << testChar[1] << ", " << testChar[2] << ", " << testChar[3]<< ", " << testChar[4]<< ", " << testChar[5]<< ", " << testChar[25*11456-10]<< std::endl;


// 		char* temp_labelStr = (char *) malloc(25* sizeof(char) );
// 		int tempcntr=0;
// 		int v = -1;
// 		for (int cntr=0; cntr < 25*h5_output_data->dimension; cntr++){
// 			if ( h5_output_data->label[cntr]!= '\0'){
// 				temp_labelStr[tempcntr]= h5_output_data->label[cntr];
// 				tempcntr++;
// 			}
// 			else {
// 				temp_labelStr[tempcntr]='\0';
// 				v++;
// 				//std::cout<< v << " " << std::flush;
// 				if (v < h5_output_data->dimension){
// 					labelVect.append(QString(temp_labelStr));
// 					tempcntr=0;
// 				}
// 			}
// 		}
// 		qDebug() << "labelVect: " << labelVect[0] << ", " << labelVect[1]<< ", " << labelVect[2] << ", " << labelVect[3];
		
		

		for( int v = 0; v < h5_output_data->dimension; v++ ) {
			//labelVect.append(h5_output_data->label[v].c_str());
			labelVect.append(h5_output_data->label[v]);
			segmentIDVect.append(h5_output_data->segment_ID[v]);
			
			scene = new SoSeparator();		//create one node for each cylinder and show them into Scene Graph
							// light
 	//scene->addChild(new SoDirectionalLight);
	
			geometry = new GeometryNode(scene, data, rf, this);
			childItems.insert(childCount(), geometry);
			this->rf = rf;
			root->addChild(scene);
			vrtxX=h5_output_data->centres[3*v];	//0.0;		//v-th vertex tangentX
			vrtxY=h5_output_data->centres[3*v+1];		//0.0;		//v-th vertex tangentY
			vrtxZ=h5_output_data->centres[3*v+2];		//1.0;		//v-th vertex tangentZ
			vrtx_radius=h5_output_data->radius[v];		//0.1;		//v-th vertex radius
			tangentX=h5_output_data->tangents[3*v];	//0.0;		//v-th vertex tangentX
			tangentY=h5_output_data->tangents[3*v+1];		//0.0;		//v-th vertex tangentY
			tangentZ=h5_output_data->tangents[3*v+2];		//1.0;		//v-th vertex tangentZ
			vrtx_height=1.1*h5_output_data->heights[v]; //to make cylinder longer to gets connected to neighbour cylinder for better visualization    
			lab = h5_output_data->label[v];
			//lab = labelVect[v];
			//qDebug() << v << " : " << lab;
			output_loadCylinder = geometry->loadCylinder(file,vrtxX,vrtxY,vrtxZ,vrtx_radius,tangentX,tangentY,tangentZ,vrtx_height,cyl_num,lab);
// 			std::cout << "parent num children : " << root->getNumChildren() <<std::endl;
			nodeIDs[v]= output_loadCylinder.nodeid;
			cylmatVect.append( output_loadCylinder.nodematerial);
			cylradiusVect.append( output_loadCylinder.noderadius); 
			cyl_num++;
		}
		free (h5_output_data);
		radiusCalc(cylradiusVect);
		std::cout << "size, min and max radius: "<< radsize<< " , "<<radiusmin << ", " << radiusmax <<std::endl;
		////create the labeling and radius widget
		createLabelForm();
		h5_filename=file.fileName(); //QString type
	}
	
	if (ext == "obj") {
		File_formats format;
		int num_objects;
		object_struct** object_list;
		// FIXME: fix bicpl for constness on these damn strings!!!
		if ( input_graphics_file( (char*)file.fileName().toLatin1().data(), &format, &num_objects, &object_list ) != OK ) {
			return false;
		}
		if (num_objects > 1) {
			std::cout << "number of objects is: " << num_objects <<std::endl;
		}
    
		for ( int i = 0; i < num_objects; ++i ) {
			scene = new SoSeparator();
			geometry = new GeometryNode(scene, data, rf, this);
			childItems.insert(childCount(), geometry);
			this->rf = rf;
			root->addChild(scene);
			bool status = geometry->loadObj(file,i);
			Q_ASSERT_X(status == true, "GeometryScene::loadGeometry", "loadObj returned False");
			qDebug() << "Debug: << geometry->loadObj #" <<i <<" status = " << status;
		}
	}

	qDebug() << "Debug. <<GeometryScene::loadGeometry()";
	return true;
}

void GeometryScene::createLabelForm() {
	this->rf = rf;

	connect(rf->uilabel.radiusSlider, SIGNAL(valueChanged(int)),
			this, SLOT(updateRadiusTransparency(int)));
// 	connect(rf->uilabel.LabelVessels, SIGNAL(clicked()),
// 			this, SLOT(getUserLabel()));
	connect(rf->uilabel.LabelVessel, SIGNAL(activated( int )),
			this, SLOT(getUserLabel()));		
	connect(rf->uilabel.SaveLabeledash5, SIGNAL(clicked()),
			this, SLOT(saveLabel()));
			
	rf->uilabel.radiusSlider->setEnabled ( TRUE );
	rf->uilabel.radius->setEnabled ( TRUE );
}

void GeometryScene::saveLabel(){
	qDebug() << "\nGeometryScene::saveLabel() " ;
	H5::H5File Lh5file = H5::H5File();
    Lh5file.openFile(h5_filename.toLatin1().data(), H5F_ACC_RDWR );
	H5::Group edge_properties_group = Lh5file.openGroup("/vessel_graph/edge_properties");
	
	std::cout<< "opening label dataset to write the user label" <<std::endl;
	H5::DataSet dataset_label =edge_properties_group.openDataSet("label");
	H5::DataSpace dataspace = dataset_label.getSpace();	//Get dataspace of the dataset.
	int rank = dataspace.getSimpleExtentNdims();//Get the number of dimensions in the dataspace.
	hsize_t dims_out[2];// Get the dimension size of each dimension in the dataspace and display them.
	int ndims = dataspace.getSimpleExtentDims( dims_out, NULL);
//  	std::cout << "/vessel_graph/edge_properties/label dims_out[0],[1]: " <<dims_out[0] << "," << dims_out[1]<< " , ndims = " << ndims <<" , rank = " << rank <<endl;
	H5::DataSpace fspace( rank, dims_out );
	
	if (labelVect.size()!= dims_out[0])
	{
		std::cout <<"ERROR: the number of the edges labels are not equal to the number of labels in the vector!" <<endl;
		std::cout << "Aborted"<<endl;
		return;
	}
	
	int *Label_buf = (int*) malloc( sizeof(int) * labelVect.size() );
// 	char *Label_buf = (char*) malloc(25* sizeof(char) * labelVect.size() );
// 	char *temp_buf = (char*) malloc(25* sizeof(char) );
// 	int cnt=0;
	for (int i=0; i<labelVect.size(); i++){
		Label_buf[i]=labelVect[i];
	}
	
	dataset_label.write(Label_buf,H5::PredType::NATIVE_INT, fspace, fspace);
	
// 		char* temp_buf=labelVect[i].toLatin1().data();
// 		int j=0;
// 		while (temp_buf[j]!='\0'){
// 			Label_buf[cnt]=temp_buf[j];
// 			cnt++;
// 			j++;
// 		}
// 		Label_buf[cnt]='\0';
// 		cnt++;
// 	}
// //	std::cout << "Label_buf[0] is: "<< Label_buf[0] << ", " << Label_buf[1] << ", " << Label_buf[2] << ", " << Label_buf[3]<< ", " << Label_buf[4]<< ", " << Label_buf[5]<< ", " << Label_buf[25*11456-10]<< std::endl;
// 
// // 	H5::StrType *Label_buf (0,H5::H5T_VARIABLE /*H5::PredType::VARIABLE*/); /*= (QString*) malloc( sizeof(QString) * labelVect.size() );*/
// // 	for (int i=0; i<labelVect.size(); i++){
// // 		Label_buf[i]=labelVect[i];		//.toLatin1().data()  convert QString to char *
// // 	}
// // 	
// 


// 		//DataSpace sid1(SPACE1_RANK, dims1);
// 		///* Create a datatype to refer to */
// 		//H5::StrType tid1(0, H5T_VARIABLE);
// 		//DataSet dataset = file.createDataSet(DSET_VLSTR_NAME, tid1, sid1);
// 		///* Write dataset to disk */
// 		//dataset.write((void*)wdata, tid1);
// 	
// 		//hid_t datatype = H5Tcopy (H5T_C_S1);
// 		//H5Tset_size (datatype, H5T_VARIABLE);
// 		///	dataset_label.write((void*)Label_buf,datatype/*tid1*/);

// 	/* Get datatype for dataset */
//     H5::DataType dtype = dataset_label.getDataType();
// // 	std::cout << "got dataset_label's datatype... " << std::endl;
// 	
// 	std::cout << "writing dataset_label " << std::endl;	
// ////	dataset_label.read(h5_data->label ,dtype ,memspace_l, dataspace_l); /*memtype*//*H5::PredType::NATIVE_CHAR*//*H5T_C_S1*/	
//  	dataset_label.write(Label_buf,/*H5::PredType::NATIVE_CHAR*/dtype , fspace, fspace);		//H5::H5T_VARIABLE  /*H5::PredType::VARIABLE*/ //H5::PredType::NATIVE_INT = H5T_NATIVE_INT


	dataset_label.close();
	edge_properties_group.close();
	Lh5file.close();
 	qDebug() << "Debug. >>GeometryScene::saveLabel()...Successfully wrote the labeled H5 file!\n";
}

void GeometryScene::getUserLabel(){
	qDebug() << "GeometryScene::getUserLabel() " ;
 	int userLabelnum = 0;
 	QString userLabel;
//	bool ok;
	//for the selected cylinder I get the NodeID which corresponds to a cylinder number N=> open H5 and read the N-th segment_ID , then save label as new field for all the edges that have that segment_ID
	//we then read this in python to assign labels to actual corrected output of vessel tracking
	int cyl_num;
	for (int i=0; i<labelVect.size() ; i++){
		if (nodeIDs[i]== nodeId){
			cyl_num=i;
			break;	//cyl_num will have the number of the picked cylinder object
		}
	}
	qDebug() << "user entered a label ..." ;	
// 	userLabel = QInputDialog::getInteger(0, "Label selected vessel segment", "Label number [1:255.0]:", labelVect[cyl_num],0,255.0,1,&ok); //opens a dialog with title Label selected vessel segment, inside it message Label number
	userLabel = rf->uilabel.LabelVessel->currentText();
	qDebug() << "user entered label " << userLabel ;
	
				// 	//if user cancled entering label dont' do following steps
				// 	if (!ok){
				// 		return;
				// 	}
				//	//std::cout << "User entered Label " <<userLabel << " for vessel segment with NodeId " << nodeId << " of selected cylinder number " << cyl_num << " with label " << labelVect[cyl_num] <<std::endl;

	userLabelnum = label_Name2num (userLabel);
				
	//check if this vessel segment was labeled before differently? if it wasnt 0 and different from userLabel, ask relable?
 	if (labelVect[cyl_num]!=userLabelnum && labelVect[cyl_num]!=0) {		//it has been labeled with a different label from new userLabel 
//	if (labelVect[cyl_num].compare(userLabel)!=0 && labelVect[cyl_num].compare (QString("No label\0"))!=0) {		//it has been labeled with a different label from new userLabel 
		QString response = QString("Selected vessel was labelled before.Do you want to relabel?");
		int answer = QMessageBox::question(0, "Relabel vessel?", response, QMessageBox::Yes | QMessageBox::No);
	// 	std::cout << " After MessageBox" << std::endl;
		if (answer == QMessageBox::Yes){
			//relabel
			for (int i=0; i<segmentIDVect.size(); i++){
				if (segmentIDVect[i]==segmentIDVect[cyl_num]){
					labelVect[i]=userLabelnum;
				}
			}
		}
	}
 	else if (labelVect[cyl_num]==0 ){	//it wasn't labeled before =>label
//	else if (labelVect[cyl_num].compare (QString("No label\0"))==0 ){	//it wasn't labeled before =>label
		for (int i=0; i<segmentIDVect.size(); i++){
			if (segmentIDVect[i]==segmentIDVect[cyl_num]){
				labelVect[i]=userLabelnum;
			}
		}	
	}
	
	//color the selected vessel segment to color from lookup table based on label, transpar=0.5
 	if (labelVect[cyl_num]!=0 ){	//it was labeled before => color and transparency
//	if (labelVect[cyl_num].compare (QString("No label\0"))!=0 ){	//it was labeled before => color and transparency
		for (int i=0; i<segmentIDVect.size(); i++){
			if (segmentIDVect[i]==segmentIDVect[cyl_num]){
				geometry->updateCylinderColour(i, cylmatVect,labelVect[i]);
				cylnum_transparency0_5.append(i);
			}
		}
	}
	//color the selected vessel segment to undo labeling: r=255.0, g=255.0, b=255.0, transpar=0.0
	else{
		for (int i=0; i<segmentIDVect.size(); i++){
			if (segmentIDVect[i]==segmentIDVect[cyl_num]){
				geometry->updateCylinderColour(i, cylmatVect, 0,0.0);
				int trans_ind= cylnum_transparency0_5.lastIndexOf(i);
				if (trans_ind !=-1){ cylnum_transparency0_5.remove(trans_ind);}
			}
		}
	}
	qDebug() << "GeometryScene::getUserLabel() " ;	
}


void GeometryScene::updateRadiusTransparency(int newVal){
// 	qDebug() << "Debug. <<GeometryScene::updateRadiusTransparency()";
	//find the threshold radius, find all radius in QVect smaller than threshold and call geometrynode::updateCylTransparency
	float radthresh= (newVal*(radiusmax- radiusmin)/100)+radiusmin;
// 	std::cout<< " Threshold radius: " << radthresh << std::endl;
	for (int i=0; i<radsize ; i++){
		if (cylradiusVect[i]<=radthresh) {
			geometry->updateCylTransparency( i, cylmatVect, 1);
		}
		else if (cylradiusVect[i]>radthresh) {
			if (cylnum_transparency0_5.contains(i)){
				geometry->updateCylTransparency( i, cylmatVect, 0.5);
			}
			else{
				geometry->updateCylTransparency( i, cylmatVect, 0.0);
			}
		}
	}
// 	qDebug() << "Debug. >>GeometryScene::updateRadiusTransparency()";
}

GeometryScene::h5_output_type * GeometryScene::H5_reader (char* dbfile){
    H5::H5File h5file = H5::H5File();
    h5file.openFile(dbfile, H5F_ACC_RDONLY); //read only access. H5F_ACC_RDWR :read and write access 
// 	cout << "File: " << h5file.getFileName() << "\nSize:" << h5file.getFileSize() << "\nObject Count:" << h5file.getObjCount() << endl;
// 	cout << "Opening DataSet." << endl;

//     cout << "Opening Groups..." << endl;//Open the specified file and the specified dataset in the file.
	H5::Group edge_properties_group = h5file.openGroup("/vessel_graph/edge_properties");
// 	cout << "edge_properties_group group has " << edge_properties_group.getNumObjs() << " object in it. Its ID is " << edge_properties_group.getId() << endl;

// 	cout << "Opening DataSets..." << endl;
// 	cout << "reading segment ID..." <<endl;	
	H5::DataSet dataset_segmentID =edge_properties_group.openDataSet("segment_ID");
	
// 	cout << "reading edge centres..." <<endl;	
	H5::DataSet dataset_centres =edge_properties_group.openDataSet("centre");
	
// 	cout << "reading edge heights..." <<endl;	
	H5::DataSet dataset_heights =edge_properties_group.openDataSet("height");	
	
// 	cout << "reading edge radius..." <<endl;	
	H5::DataSet dataset_radius =edge_properties_group.openDataSet("radius");
	
// 	cout << "reading edge label..." <<endl;	
	H5::DataSet dataset_label =edge_properties_group.openDataSet("label");
	
// 	cout << "reading edge tangents..." <<endl;	
	H5::DataSet dataset_tangents =edge_properties_group.openDataSet("tangent");
	
	H5::DataSpace dataspace_ID = dataset_segmentID.getSpace();	//Get dataspace of the dataset.
	int rank_ID = dataspace_ID.getSimpleExtentNdims();//Get the number of dimensions in the dataspace.
    hsize_t dims_out_ID[2];// Get the dimension size of each dimension in the dataspace and display them.
	int ndims_ID = dataspace_ID.getSimpleExtentDims( dims_out_ID, NULL);
// 	cout << "dims_out_ID[0],[1]" <<dims_out_ID[0] << "," << dims_out_ID[1]<< " , ndims_ID = " << ndims_ID <<" , rank_ID = " << rank_ID <<endl;
	H5::DataSpace memspace_ID( rank_ID, dims_out_ID );
	
	H5::DataSpace dataspace = dataset_centres.getSpace();	//Get dataspace of the dataset.
	int rank = dataspace.getSimpleExtentNdims();//Get the number of dimensions in the dataspace.
    hsize_t dims_out[2];// Get the dimension size of each dimension in the dataspace and display them.
	int ndims = dataspace.getSimpleExtentDims( dims_out, NULL);
// 	cout << "dims_out[0],[1]" <<dims_out[0] << "," << dims_out[1]<< " , ndims = " << ndims <<" , rank centre = " << rank <<endl;
	H5::DataSpace memspace( rank, dims_out );
	
	H5::DataSpace dataspace_h = dataset_heights.getSpace();	//Get dataspace of the dataset.
	int rank_h = dataspace_h.getSimpleExtentNdims();//Get the number of dimensions in the dataspace.
    hsize_t dims_out_h[2];// Get the dimension size of each dimension in the dataspace and display them.
	int ndims_h = dataspace_h.getSimpleExtentDims( dims_out_h, NULL);
// 	cout << "dims_out_h[0],[1]" <<dims_out_h[0] << "," << dims_out_h[1]<< " , ndims_h = " << ndims_h<<" , rank_h = " << rank_h <<endl;
	H5::DataSpace memspace_h( rank_h, dims_out_h );

	H5::DataSpace dataspace_r = dataset_radius.getSpace();	//Get dataspace of the dataset.
	int rank_r = dataspace_r.getSimpleExtentNdims();//Get the number of dimensions in the dataspace.
    hsize_t dims_out_r[2];// Get the dimension size of each dimension in the dataspace and display them.
	int ndims_r = dataspace_r.getSimpleExtentDims( dims_out_r, NULL);
//  	cout << "dims_out_r[0],[1]" <<dims_out_r[0] << "," << dims_out_r[1]<< " , ndims_r = " << ndims_r<<" , rank_r = " << rank_r <<endl;
	H5::DataSpace memspace_r( rank_r, dims_out_r );
	
	H5::DataSpace dataspace_l = dataset_label.getSpace();	//Get dataspace of the dataset.
	int rank_l = dataspace_l.getSimpleExtentNdims();//Get the number of dimensions in the dataspace.
    hsize_t dims_out_l[2];// Get the dimension size of each dimension in the dataspace and display them.
	int ndims_l = dataspace_l.getSimpleExtentDims( dims_out_l, NULL);
//  	cout << "dims_out_l[0],[1]" <<dims_out_l[0] << "," << dims_out_l[1]<< " , ndims_l = " << ndims_l<<" , rank_l = " << rank_l <<endl;
	H5::DataSpace memspace_l( rank_l, dims_out_l );
	
	H5::DataSpace dataspace_t = dataset_tangents.getSpace();	//Get dataspace of the dataset.
	int rank_t = dataspace_t.getSimpleExtentNdims();//Get the number of dimensions in the dataspace.
    hsize_t dims_out_t[2];// Get the dimension size of each dimension in the dataspace and display them.
	int ndims_t = dataspace_t.getSimpleExtentDims( dims_out_t, NULL);
// 	cout << "dims_out_t[0],[1]" <<dims_out_t[0] << "," << dims_out_t[1]<< " , ndims_t = " << ndims_t<<" , rank_t = " << rank_t <<endl;
	H5::DataSpace memspace_t( rank_t, dims_out_t );
	
	if ((unsigned long)(dims_out[1])!= 3) {
		cout <<"ERROR: the size of the edges centres is not 3!" <<endl;
		cout << "Aborted"<<endl;
		exit(0);
	}
	
	if ((unsigned long)(dims_out[0])!= dims_out_ID[0]) {
		cout <<"ERROR: the number of the edges centres are not equal to the number of segment_IDs!" <<endl;
		cout << "Aborted"<<endl;
		exit(0);
	}
	
	if ((unsigned long)(dims_out[0])!= dims_out_h[0]) {
		cout <<"ERROR: the number of the edges centres are not equal to the number of heights!" <<endl;
		cout << "Aborted"<<endl;
		exit(0);
	}
	
	if ((unsigned long)(dims_out[0])!= dims_out_r[0]) {
		cout <<"ERROR: the number of the edges centres are not equal to the number of radius!" <<endl;
		cout << "Aborted"<<endl;
		exit(0);
	}
	
	if ((unsigned long)(dims_out[0])!= dims_out_l[0]) {
		cout <<"ERROR: the number of the edges centres are not equal to the number of labels!" <<endl;
		cout << "Aborted"<<endl;
		exit(0);
	}
	
	if ((unsigned long)(dims_out[0])!= dims_out_t[0]) {
		cout <<"ERROR: the number of the edges centres are not equal to the number of tangents!" <<endl;
		cout << "Aborted"<<endl;
		exit(0);
	}
	
	if ((unsigned long)(dims_out_t[1])!= 3) {
		cout <<"ERROR: the size of the edges tangents is not 3!" <<endl;
		cout << "Aborted"<<endl;
		exit(0);
	}
	
	
		
	////create the output struct
	struct h5_output_type *h5_data = (struct h5_output_type*) malloc( sizeof(*h5_data) );
	h5_data->dimension = (unsigned long)(dims_out[0]);
// 	std::cout <<"dimension of h5_data = " << (unsigned long)(dims_out[0]) << std::endl;
	h5_data->segment_ID = (int*) malloc( sizeof(int) * (int)(dims_out[0]) );
	h5_data->centres = (float *) malloc(sizeof(float) * (unsigned long)(dims_out[0]) *3);	//int **array1 = (int **)malloc(nrows * sizeof(int *));
	h5_data->heights = (float*) calloc((unsigned long)(dims_out[0]), sizeof(float));
	h5_data->radius = (float*) calloc((unsigned long)(dims_out[0]), sizeof(float));
	h5_data->label = (int*) calloc((unsigned long)(dims_out[0]), sizeof(int));

// // 	std::cout <<"allocate h5_data->label " <<std::endl;
// 	h5_data->label = (char *) malloc(25* sizeof(char )*(unsigned long)(dims_out[0]));	//25 is the size of the longest vessel label string with null terminator
// // 	h5_data->label.resize((unsigned long)(dims_out[0]));
// // 	std::cout <<"allocation of h5_data->label done " <<std::endl; 

	h5_data->tangents = (float*) malloc(sizeof(float) * (unsigned long)(dims_out[0]) *3);
	
	
// 	std::cout << "Reading data..." << endl;
	dataset_segmentID.read(h5_data->segment_ID,H5::PredType::NATIVE_INT,memspace_ID, dataspace_ID);
//  	std::cout << "h5_data->segment_ID[0] is: "<< h5_data->segment_ID[0] <<std::endl;
	
	dataset_centres.read(h5_data->centres,H5::PredType::NATIVE_FLOAT,memspace, dataspace);	//PredType::NATIVE_FLOAT  //PredType::NATIVE_INT
	dataset_heights.read(h5_data->heights ,H5::PredType::NATIVE_FLOAT,memspace_h, dataspace_h);
	dataset_radius.read(h5_data->radius ,H5::PredType::NATIVE_FLOAT,memspace_r, dataspace_r);	//H5T_NATIVE_FLOAT//PredType::NATIVE_FLOAT
	dataset_label.read(h5_data->label ,H5::PredType::NATIVE_INT,memspace_l, dataspace_l);	//H5T_NATIVE_FLOAT//PredType::NATIVE_FLOAT
// 	std::cout << "h5_data->radius[0] is: "<< h5_data->radius[0] << ", " << h5_data->radius[1] << ", "<< h5_data->radius[2] << ", "<< h5_data->radius[20]<< ", "<< h5_data->radius[ 11456-10]<< ", "<< h5_data->radius[ 11456-9]<<std::endl;



// // Create new string datatype for attribute
//     //H5::StrType strdatatype(H5::PredType::C_S1, 256); // of length 256 characters
// 	/* Create a datatype to refer to */
// // 	H5::StrType tid1(0, H5::H5T_VARIABLE);
// // 	std::cout << "done creating type for string label" <<std::endl;
// // 	
// 
// 
// 
// 
// 	// Create the datatype as follows
// 		// 	hid_t type_id;
// 		// 	type_id = H5Tvlen_create(H5T_NATIVE_CHAR); 	
// 		hid_t datatype = H5Tcopy (H5T_C_S1);
// 		H5Tset_size (datatype, H5T_VARIABLE);
// 
// 
// 	/* Get datatype for dataset */
//     H5::DataType dtype = dataset_label.getDataType();
// // 	std::cout << "got dataset_label's datatype : " << std::endl;
// 	
// // 	std::cout << "reading dataset_label " << std::endl;	
// 	dataset_label.read(h5_data->label ,/*datatype*/dtype ,memspace_l, dataspace_l); /*memtype*//*H5::PredType::NATIVE_CHAR*//*H5T_C_S1*/	
// // 	std::cout << "h5_data->label[0] is: "<< h5_data->label[0] << "," << h5_data->label[1] << "," << h5_data->label[2] << "," << h5_data->label[3]<< "," << h5_data->label[4]<< "," << h5_data->label[5]<<  "," << h5_data->label[6] << "," << h5_data->label[7] << "," << h5_data->label[8]<< "," << h5_data->label[9]<< "," << h5_data->label[10]<< std::endl;

	dataset_tangents.read(h5_data->tangents ,H5::PredType::NATIVE_FLOAT,memspace_t, dataspace_t);

	dataset_tangents.close();
	dataset_radius.close();
	dataset_label.close();
	dataset_heights.close();
	dataset_centres.close();
	dataset_segmentID.close();
	edge_properties_group.close(); 
	h5file.close();
    cout << "Debug. << h5reader closing " << dbfile << endl;
	return h5_data;
}

void GeometryScene::noPointReceived(){
// 	rf->uitag.CreateTagPoint->setEnabled ( FALSE );
// 	rf->uitag.AddTagPoint->setEnabled ( FALSE );
// 	rf->uilabel.LabelVessels->setEnabled ( FALSE );
	rf->uilabel.LabelVessel->setEnabled ( FALSE );	
}

void GeometryScene::pickReceived(int index, int id, int x, int y, int z,SoType objtype) {
// 	qDebug() << "Debug. >>GeometryScene::pickReceived()";
	nodeId=index;
	//if clicked on polygon enable Tag buttons, else if enabled, disable it:
	if (objtype == SoFaceDetail::getClassTypeId() or objtype == SoLineDetail::getClassTypeId()){
		rf->uitag.CreateTagPoint->setEnabled ( TRUE );
		rf->uitag.AddTagPoint->setEnabled ( FALSE );
		rf->uitag.TagSize->setEnabled ( TRUE );
// 		rf->uilabel.LabelVessels->setEnabled ( FALSE );
		rf->uilabel.LabelVessel->setEnabled ( FALSE );		
	}
	else if (objtype == SoCylinderDetail::getClassTypeId()) {
		rf->uitag.CreateTagPoint->setEnabled ( FALSE );
		rf->uitag.AddTagPoint->setEnabled ( FALSE );
		rf->uitag.TagSize->setEnabled ( FALSE );
// 		rf->uilabel.LabelVessels->setEnabled ( TRUE );
		rf->uilabel.LabelVessel->setEnabled ( TRUE );
		int cyl_num;
		for (int i=0; i<labelVect.size() ; i++){
			if (nodeIDs[i]== nodeId){
				cyl_num=i;
				break;	//cyl_num will have the number of the picked cylinder object
			}
		}
		qDebug() << "Vessel segment with NodeId " << nodeId << " of selected cylinder number " << cyl_num << " has label # " << labelVect[cyl_num] << " : " << label_num2Name(labelVect[cyl_num]);                              
	}
	else {		
		rf->uitag.CreateTagPoint->setEnabled ( FALSE );
		rf->uitag.AddTagPoint->setEnabled ( FALSE );
		rf->uitag.TagSize->setEnabled ( FALSE );
		rf->uilabel.LabelVessel->setEnabled ( FALSE );
	}
	
   // check to see whether picked point belongs to this scene.
    if (id == geometry->getID()) {
		std::cout <<"picked point does belong to this scene" <<std::endl;
		// picked point does belong to this scene
        // set the info column in the GUI
        QString info = "Point: ";
        info.append(QString::number(index));
        this->setData(1, info);
        // emit a signal that will be picked up eventually by all the
        // texture columns, causing them to update their GUI info values.
        emit localPointPicked(index);
        // make sure that the GUI will actually be updated
        rf->somethingChanged();
    }
    else {
        // point does not belong to the scene - unset text
        this->setData(1, "");
    }
// 	qDebug() << "Debug. <<GeometryScene::pickReceived()";
}


void GeometryScene::wasSelected() {
	qDebug() << "Debug. >>GeometryScene::wasSelected()";
	rf->setLastGeometry(this);
	qDebug() << "Debug. <<GeometryScene::wasSelected()";
}


bool GeometryScene::loadVertstats(QFile &file) {
	qDebug() << "Debug. >>GeometryScene::loadVertstats()";
	textureFileItem *vert = new textureFileItem(scene, itemData, form, this);
	qDebug() << "Debug. >>GeometryScene::loadVertstats() vert";
	vert->loadFile(file);
	qDebug() << "Debug. >>GeometryScene::loadVertstats() loadFile";
	childItems.insert(childCount(), vert);
	qDebug() << "Debug. >>GeometryScene::loadVertstats() childItems";
	// make sure that the texture file receives the signal if local point was selected in
	// a ray-picking action.
	connect(this, SIGNAL(localPointPicked(int)), vert, SLOT(pickedPointReceived(int)));

	qDebug() << "Debug. <<GeometryScene::loadVertstats()";
	return true;
}


bool GeometryScene::updateLabelLUColor() {
	qDebug() << "Debug. >>GeometryScene::updateLabelLUColor()";
	for( int v = 0; v < labelVect.size(); v++ ) {
		geometry->updateCylinderColour(v, cylmatVect,labelVect[v],0.0);
	}


	qDebug() << "Debug. <<GeometryScene::updateLabelLUColor()";
	return true;
}

// bool GeometryScene::loadLabelLU(QFile &file) {
// 	qDebug() << "Debug. >>GeometryScene::loadLabelLU()";
// // // 	qDebug() << "Debug. <<ResourceForm::insertLabelLUfile()";
// 	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
// 		qDebug() << "The file " << file.fileName() << " can't be opened in readonly mode";
// 		return false;
// 	}
// 
// // 	if( !file.exists() )
// // 	{
// // 		qDebug() << "The file " << file.fileName() << "does not exist.";
// // 		return false;
// // 	}
// 	while (!file.atEnd()) {
// 		//QByteArray line = file.readLine();
// 		QString line = file.readLine();
// 			//qDebug() << line;
// 		if (line.at(0)!= QChar('#')){
// 			QStringList list = line.split(";");
// 			qDebug() << list[4];			
// 			labelnums.append(list[0].toInt());
// 			labelnames.append(list[1]);
// 			labelred.append(list[2].toFloat());
// 			labelgreen.append(list[3].toFloat());
// 			labelblue.append(list[4].toFloat());
// 
// 		}
// 	}
// 	 //qDebug() << labelnums;
// 	// qDebug() << labelnames;
// 	// qDebug() << labelblue;
// 
// 	//file.close();
// 	//cout << "\n\n\n"<< labelnames.size() << "\n\n\n";
// // 	for (int i=0; i<labelnames.size(); i++){
// // // 		qDebug << labelnames.at(i);
// // 		rf->uilabel.LabelVessel->addItem(labelnames.at(i));
// // 	}
// 
// 	qDebug() << "Debug. <<GeometryScene::loadLabelLU()";
// 	return true;
// }


void GeometryScene::radiusCalc(QVector <float> cylradiusVect){
	radsize= cylradiusVect.size ();
	radiusmin=cylradiusVect[0];
	radiusmax=cylradiusVect[0];
	for (int i=0; i<radsize; i++){
		if (radiusmin>	cylradiusVect[i]){ radiusmin= cylradiusVect[i];}
		if (radiusmax<	cylradiusVect[i]){ radiusmax= cylradiusVect[i];}
	}
	std::cout << "size, min and max radius: "<< radsize<< " , "<<radiusmin << ", " << radiusmax <<std::endl;
}
GeometryScene::~GeometryScene() {
}



QString GeometryScene::label_num2Name (int label){
		///convert int label into label Qstring
	
	QString labelName;
	
	int lindx = rf->labelnums.indexOf(label);
	if (lindx > -1)
		labelName = rf->labelnames.at(lindx);
	else
		qDebug() << "GeometryScene::label_num2Name " << label << " doesn't exist in config file";
	
// 		if ( label == 0){ //"None":
// 			labelName = "No label\0";}	//white
// 			
// 		else if ( label == 35){ //"ACA":
// 			labelName = "Anterior Cerebral Artery\0"; }		//red
// 		else if ( label == 191){ //"MCA right":
// 			labelName = "R. Middle Cerebral Artry\0"; }	//yellow
// 		else if ( label == 190){ //"MCA left":
// 			labelName = "L. Middle Cerebral Artry\0"; }
// 		else if ( label == 2){ //"Internal Carotid arte;ry right":	//blue
//                         labelName = "R. Internal Carotid Artery\0"; }
// 		else if ( label == 43){ //"Internal Carotid artery left":	
// 			labelName = "L. Internal Carotid Artery\0"; }
// 		else if ( label == 200){ //"Posterior Comm. artery right":	//purple
// 			labelName = "R. Posterior Comm. Artry\0"; }
// 		else if ( label == 9){ //"Posterior Comm. artery left":
// 			labelName = "L. Posterior Comm. Artry\0"; }
// 		else if ( label == 8){ //"PCA right":						//pink
// 			labelName = "R. Posterior Cereb. Artry\0"; }
// 		else if ( label == 5){ //"PCA left":
// 			labelName = "L. Posterior Cereb. Artry\0"; }
// 		else if ( label == 68){ //"SCA right":						//orange
// 			labelName = "R. Superior Cereb. Artery\0"; }
// 		else if ( label == 227){ //"SCA left":
// 			labelName = "L. Superior Cereb. Artery\0"; }
// 		else if ( label == 46){ //"AICA right":						//dark green
// 			labelName = "R. Ant. Inf. Cereb. Artry\0"; }
// 		else if ( label == 12){ //"AICA left":
// 			labelName = "L. Ant. Inf. Cereb. Artry\0"; }
// 		else if ( label == 196){ //"Basilar artery":					//light blue
// 			labelName = "Basilar Artery\0"; }
// 		else if ( label == 7){ //"Vertebral artery":				//light green
// 			labelName = "Vertebral Artery\0"; }
// 		else if ( label == 49){ //"Internal Auditory artery right":
// 			labelName = "R. Internal Audit Artery\0"; }
// 		else if ( label == 45){ //"Internal Auditory artery left":
// 			labelName = "L. Internal Audit Artery\0"; }
// 
// 			
// 		else if ( label == 11){ //"Superior Saggital Sinus":			//orange
// 			labelName = "Superior Saggital Sinus\0"; }
// 		else if ( label == 6){ //"Great Cerebral Vein of Galen":	//blue
// 			labelName = "Great Cerebral Vein of Galen\0"; }
// 		else if ( label == 30){ //"Transverse Sinus right":			//green
// 			labelName = "R. Transverse Sinus\0"; }
// 		else if ( label == 246){ //"Transverse Sinus left":
// 			labelName = "L. Transverse Sinus\0"; }
// 		else if ( label == 192){ //"Caudal Rhinal Vein right":		//pink purple
// 			labelName = "R. Caudal Rhinal Vein\0"; }
// 		else if ( label == 34){ //"Caudal Rhinal Vein left":
// 			labelName = "L. Caudal Rhinal Vein\0"; }
// 		else if ( label == 20){ //"Rostral Rhinal Vein right":		//red brown
// 			labelName = "R. Rostral Rhinal Vein\0"; }
// 		else if ( label == 21){ //"Rostral Rhinal Vein left":
// 			labelName = "L. Rostral Rhinal Vein\0"; }
// 		else if ( label == 101){ //"Sigmoid Sinus right":				//yellow green
// 			labelName = "R. Sigmoid Sinus\0"; }
// 		else if ( label == 24){ //"Sigmoid Sinus left":
// 			labelName = "L. Sigmoid Sinus\0"; }
// 		else if ( label == 58){ //"Longitudinal Hippocampal Vein right":	//blue grey
// 			labelName = "R. Longitud. Hippo. Vein\0"; }
// 		else if ( label == 57){ //"Longitudinal Hippocampal Vein left":
// 			labelName = "L. Longitud. Hippo. Vein\0"; }
// 		else if ( label == 56){ //"Thalamostriate Vein right":			//dark pink
// 			labelName = "R. Thalamostriate Vein\0"; }
// 		else if ( label == 54){ //"Thalamostriate Vein left":
// 			labelName = "L. Thalamostriate Vein\0"; }		
// 		else if ( label == 1){ //"Medial Collicular Vein right":
// 			labelName = "R. Medial Colicular Vein\0"; }			
// 		else if ( label == 16){ //"Medial Collicular Vein left":
// 			labelName = "L. Medial Colicular Vein\0"; }
// 		else if ( label == 170){ //#unknown sinus/vein:			//dark purple
// 			labelName = "Unknown Sinus / Vein #01\0"; }
// 		else if ( label == 171){ //#"Lateral collicular Vein right":      //dark purple
// 			labelName = "R. Lateral collicular V.\0"; }		
// 		else if ( label == 250){ //#unknown sinus/vein left":			//dark brown
// 			labelName = "L. Unknown Sinus/Vein #2\0"; }
// 		else if ( label == 251){ //##unknown sinues/vein right":      //dark pink-purple
// 			labelName = "R. Unknown Sinus/Vein #2\0"; }		

	return labelName;
}


int GeometryScene::label_Name2num (QString labelName){
	///convert label Qstring to a number to be saved in h5 file	
	
	int 	labelnum = 0;
	
	int lindx = rf->labelnames.indexOf(labelName);
	if (lindx > -1)
		labelnum = rf->labelnums.at(lindx);
	else
		qDebug() << "GeometryScene::label_Name2num " << labelName << " doesn't exist in config file";
	
// 		if (labelName.compare(/*case*/ "No label\0") ==0){//:
// 			labelnum = 0;}	//white
// 			
// 		else if (labelName.compare(/*case*/ "Anterior Cerebral Artery\0") ==0){//:
// 			labelnum = 35;}		//red
// 		else if (labelName.compare(/*case*/ "R. Middle Cerebral Artry\0") ==0){//:
// 			labelnum = 191;}	//yellow
// 		else if (labelName.compare(/*case*/ "L. Middle Cerebral Artry\0") ==0){//:
// 			labelnum = 190;}
// 		else if (labelName.compare(/*case*/ "R. Internal Carotid Artery\0") ==0){//:Internal Carotid Artery	//blue
// 			labelnum = 2;}
// 		else if (labelName.compare(/*case*/ "L. Internal Carotid Artery\0") ==0){//:	
// 			labelnum = 43;}
// 		else if (labelName.compare(/*case*/ "R. Posterior Comm. Artry\0") ==0){//:	//purple
// 			labelnum = 200;}
// 		else if (labelName.compare(/*case*/ "L. Posterior Comm. Artry\0") ==0){//:
// 			labelnum = 9;}
// 		else if (labelName.compare(/*case*/ "R. Posterior Cereb. Artry\0") ==0){//:						//pink
// 			labelnum = 8;}
// 		else if (labelName.compare(/*case*/ "L. Posterior Cereb. Artry\0") ==0){//:
// 			labelnum = 5;}
// 		else if (labelName.compare(/*case*/ "R. Superior Cereb. Artery\0") ==0){//:						//orange
// 			labelnum = 68;}
// 		else if (labelName.compare(/*case*/ "L. Superior Cereb. Artery\0") ==0){//:
// 			labelnum = 227;}
// 		else if (labelName.compare(/*case*/ "R. Ant. Inf. Cereb. Artry\0") ==0){//:						//dark green
// 			labelnum = 46;}
// 		else if (labelName.compare(/*case*/ "L. Ant. Inf. Cereb. Artry\0") ==0){//:L. Anterior Inferior Cerebral Artery
// 			labelnum = 12;}
// 		else if (labelName.compare(/*case*/ "Basilar Artery\0") ==0){//:					//light blue
// 			labelnum = 196;}
// 		else if (labelName.compare(/*case*/ "Vertebral Artery\0") ==0){//:				//light green
// 			labelnum = 7;}
// 		else if (labelName.compare(/*case*/ "R. Internal Audit Artery\0") ==0){//:
// 			labelnum = 49;}
// 		else if (labelName.compare(/*case*/ "L. Internal Audit Artery\0") ==0){//:Internal Auditory Artery
// 			labelnum = 45;}
// 
// 			
// 		else if (labelName.compare(/*case*/ "Superior Saggital Sinus\0") ==0){//:			//orange
// 			labelnum = 11;}
// 		else if (labelName.compare(/*case*/ "Great Cerebral Vein of Galen\0") ==0){//:	//blue
// 			labelnum = 6;}
// 		else if (labelName.compare(/*case*/ "R. Transverse Sinus\0") ==0){//:			//green
// 			labelnum = 30;}
// 		else if (labelName.compare(/*case*/ "L. Transverse Sinus\0") ==0){//:
// 			labelnum = 246;}
// 		else if (labelName.compare(/*case*/ "R. Caudal Rhinal Vein\0") ==0){//:		//pink purple
// 			labelnum = 192;}
// 		else if (labelName.compare(/*case*/ "L. Caudal Rhinal Vein\0") ==0){//:
// 			labelnum = 34;}
// 		else if (labelName.compare(/*case*/ "R. Rostral Rhinal Vein\0") ==0){//:		//red brown
// 			labelnum = 20;}
// 		else if (labelName.compare(/*case*/ "L. Rostral Rhinal Vein\0") ==0){//:
// 			labelnum = 21;}
// 		else if (labelName.compare(/*case*/ "R. Sigmoid Sinus\0") ==0){//:				//yellow green
// 			labelnum = 101;}
// 		else if (labelName.compare(/*case*/ "L. Sigmoid Sinus\0") ==0){//:
// 			labelnum = 24;}
// 		else if (labelName.compare(/*case*/ "R. Longitud. Hippo. Vein\0") ==0){//:	R. Longitudinal Hippocampal Vein// blue grey
// 			labelnum = 58;}
// 		else if (labelName.compare(/*case*/ "L. Longitud. Hippo. Vein\0") ==0){//:
// 			labelnum = 57;}
// 		else if (labelName.compare(/*case*/ "R. Thalamostriate Vein\0") ==0){//:			//dark purple
// 			labelnum = 56; }
// 		else if (labelName.compare(/*case*/ "L. Thalamostriate Vein\0") ==0){//:
// 			labelnum = 54; }
// 		else if (labelName.compare(/*case*/ "R. Medial Colicular Vein\0") ==0){//:Medial Collicular Vein
// 			labelnum = 1;}			
// 		else if (labelName.compare(/*case*/ "L. Medial Colicular Vein\0") ==0){//:
// 			labelnum = 16;}	
// 		else if (labelName.compare(/*case*/ "Unknown Sinus / Vein #01\0") ==0){ //#unknown sinus/vein:			//dark purple
// 			labelnum = 170; }
// 		else if (labelName.compare(/*case*/ "R. Lateral collicular V.\0") ==0){ //#"Lateral collicular Vein right":      //dark purple
// 			labelnum = 171; }		
// 		else if (labelName.compare(/*case*/ "L. Unknown Sinus/Vein #2\0") ==0){ //#unknown sinus/vein left":			//dark brown
// 			labelnum = 250; }
// 		else if (labelName.compare(/*case*/ "R. Unknown Sinus/Vein #2\0") ==0){ //##unknown sinues/vein right":      //dark pink-purple
// 			labelnum = 251; }		
	
	return labelnum;
}














