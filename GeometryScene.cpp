#include <QtDebug>
#include <QFileDialog>
#include <QFile>
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

	//qDebug() << "Debug. >>GeometryScene::GeometryScene()";
	setData(0, "Geometry Scene");

	scene = new SoSeparator();
				// light
 	//scene->addChild(new SoDirectionalLight);
	
	geometry = new GeometryNode(scene, data, rf, this);
	childItems.insert(childCount(), geometry);
	this->rf = rf;
	this->data = data;
	flag_new_edge = FALSE;
	num_clicked = 0;
	connectingVect.resize(0);
	//qDebug() << "Debug. <<GeometryScene::GeometryScene()";
}

//delete the edge from the lists and the viewer
void GeometryScene::deleteThisEdge(){
	//qDebug() << "\nGeometryScene::deleteThisEdge() "<<  num_edge_total;
	
		//qDebug() << "NodeId " << nodeId <<"="<< nodeIDs[e_num][0]<< " Vessel segment number " << e_num << " is edge ("<< edgesVect[2*e_num] << "," << edgesVect[2*e_num+1] << ") with label# " << labelVect[e_num] << " : " << label_num2Name(labelVect[e_num])<< " will be deleted!";                              

/*		for (int i=0; i<num_edge_total; i++){
			std::cout <<"\n"<< i <<  ": " << std::flush;
			for (int j=0; j<childIDs[i].size(); j++){
				std::cout<< j << "("<< childIDs[i][j] << ")" << nodeIDs[i][j] <<"="<< root->getChild(childIDs[i][j])->getNodeId() << "  " << std::flush;
			}
		}*/
		
		
	rf->uilabel.DeleteEdge->setEnabled ( FALSE );
	rf->uilabel.label->setEnabled ( FALSE );
	rf->uilabel.LabelVessel->setEnabled ( FALSE );
	//r u sure u want to delete this edge?
	QString response = QString("Are you sure you want to delete selected vessel segment?");
	int answer = QMessageBox::question(0, "Delete vessel?", response, QMessageBox::Yes | QMessageBox::No);
	if (answer == QMessageBox::Yes){
		//delete edge
		//std::cout << "root children : " << root->getNumChildren() << std::endl;
		int childid = childIDs[e_num][0];
		for (int i=0; i<childIDs[e_num].size(); i++){
			root->removeChild(childid);
		}
	for (int i=e_num+1; i<childIDs.size(); i++){
		for (int j=0; j< childIDs[i].size(); j++){
			childIDs[i][j]-=childIDs[e_num].size();
		}
	}
	
// 	nodeIDs.remove(e_num);	
// 	childIDs.remove(e_num);
// 	labelVect.remove(e_num);
// 	edgesVect.remove(2*e_num+1);
// 	edgesVect.remove(2*e_num);
// 	cylmatVect.remove(e_num);
// 	cylradiusVect.remove(e_num);
// 	for (int i=0; i<cylnum_transparency0_5.size(); i++){
// 		if (cylnum_transparency0_5[i] == e_num){
// 			cylnum_transparency0_5.remove(i);
// 		}
// 		if (cylnum_transparency0_5[i] > e_num){
// 			cylnum_transparency0_5[i]--;
// 		}
// 	}


 		
// /*		SoChildList * childrenlist = root->getChildren();
// 		for (int i=0; i<root->getNumChildren(); i++){
// 			if (childrenlist[i]->getNodeId() == nodeId){
// 				std::cout << "found child : " << root->findChild(childrenlist[i]) << std::endl;
// 			}
// 		}*/
		
// 		geometry->updateCylTransparency(cylmatVect[e_num].size(), cylmatVect[e_num], .8);
		//std::cout << "root children : " << root->getNumChildren() << std::endl;
		//rf->somethingChanged();
	
	num_edge_total--;
	del_edgesVect.append(e_num);
	qDebug() << /*"NodeId " << nodeId <<"="<< nodeIDs[e_num][0]<< " Vessel segment number " << e_num << " is*/ "edge ("<< edgesVect[2*e_num] << "," << edgesVect[2*e_num+1] << ") with label #" << labelVect[e_num] << " : " << label_num2Name(labelVect[e_num])<< " is deleted!\n";
	}
/*		for (int i=0; i<num_edge_total; i++){
			std::cout <<"\n"<< i <<  ": " << std::flush;
			for (int j=0; j<childIDs[i].size(); j++){
				std::cout<< j << "("<< childIDs[i][j] << ")" << nodeIDs[i][j] <<"="<< root->getChild(childIDs[i][j])->getNodeId() << "  " << std::flush;
			}
		}*/
	
	//qDebug() << "\nGeometryScene::deleteThisEdge() "<<  num_edge_total;	
}

//start making the new edge by preparing the vector and wait for 2 edges to be clicked on
void GeometryScene::createNewEdge(){
	//qDebug() << "\nGeometryScene::createEdge() " ;
	rf->uilabel.Saveh5->setEnabled ( FALSE );
	rf->uilabel.Saveash5->setEnabled ( FALSE );
	rf->uilabel.DeleteEdge->setEnabled ( FALSE );
	rf->uilabel.AddEdge->setEnabled ( FALSE );
	rf->uilabel.label->setEnabled ( FALSE );
	rf->uilabel.LabelVessel->setEnabled ( FALSE );
	flag_new_edge = TRUE;
	num_clicked = 0;
	connectingVect.resize(0);
	
}

bool GeometryScene::edgeExists(int vertx0,int vertx1,QVector <int> Vect){
	bool return_val = FALSE;
	for (int i=0; i < Vect.size()-1; i++){
		if (Vect[i]==vertx0 && Vect[i+1]==vertx1){
			return_val=TRUE;
			break;			
		}
	}
	
	return return_val;
}
	
//once the 2 edges to be connected are clicked, add the new edge to lists and the viewer
void GeometryScene::addNewEdge(){
	//qDebug() << "\nGeometryScene::addEdge() " ;
	///{add to edgesVect,and add corresponding to label and nodeId (when showing it) and calc cyl_stuff and show! and activate Saveh5, in Saveh5:save edgesVect too}	
	rf->uilabel.Saveh5->setEnabled ( TRUE );
	rf->uilabel.Saveash5->setEnabled ( TRUE );
	rf->uilabel.AddEdge->setEnabled ( TRUE );
	rf->uilabel.DeleteEdge->setEnabled ( FALSE );
	rf->uilabel.label->setEnabled ( FALSE );
	rf->uilabel.LabelVessel->setEnabled ( FALSE );
	if (connectingVect.size()!=2){
		cout <<"ERROR: the number of vertices in the new edge is " << connectingVect.size() << "\nAborted!" <<endl;
		exit(0);
	}
	
	//sort the edge (v0,v1) so that v0<v1
	if (connectingVect[1]<connectingVect[0]){
		int tmpval=connectingVect[1];
		connectingVect[1]=connectingVect[0];
		connectingVect[0]=tmpval;
	}
	
	
	if (!edgeExists(connectingVect[0],connectingVect[1],edgesVect)){
		cout << "New edge ("<<connectingVect[0] << "," << connectingVect[1]<< ") is added!\n" << endl;
		edgesVect.append(connectingVect[0]);
		edgesVect.append(connectingVect[1]);
		new_edgesVect.append(connectingVect[0]);
		new_edgesVect.append(connectingVect[1]);
		
		labelVect.append(int(0));
		//std::cout << "labelVect.size() "<< labelVect.size() << std::endl;
		num_edge_total++;
		
		float a1 = vertices_centreVect[3*connectingVect[0]]; float b1 = vertices_centreVect[3*connectingVect[0]+1]; float c1 = vertices_centreVect[3*connectingVect[0]+2];
		float a2 = vertices_centreVect[3*connectingVect[1]]; float b2 = vertices_centreVect[3*connectingVect[1]+1]; float c2 = vertices_centreVect[3*connectingVect[1]+2];
		//cout << "edge (" << connectingVect[0] << ","<< connectingVect[1] <<") has vertices' centres at [" << a1 << " " << b1 << " " << c1 << "] and [" << a2 << " " << b2 << " " << c2 << "]"<< endl; 
		
		float centreX = (a1+a2)/2; float centreY = (b1+b2)/2;  float centreZ = (c1+c2)/2;
		float heights = sqrt((a2-a1)*(a2-a1)+(b2-b1)*(b2-b1)+(c2-c1)*(c2-c1));
		float tangentX = (a2-a1)/heights; float tangentY = (b2-b1)/heights;  float tangentZ = (c2-c1)/heights;
		float newedge_rad=0;
		for (int c=0; c<cylradiusVect[edge1_indx].size(); c++){
			newedge_rad += cylradiusVect[edge1_indx][c];
		}
		for (int c=0; c<cylradiusVect[edge2_indx].size(); c++){
			newedge_rad += cylradiusVect[edge2_indx][c];
		}
		newedge_rad = newedge_rad/(cylradiusVect[edge1_indx].size() + cylradiusVect[edge2_indx].size());
		
		// /*hvl_ti*  new_*/nodeIDs = (hvl_ti2*) realloc (nodeIDs, (int)(num_edge_total)*sizeof(hvl_ti2));
// 		nodeIDs[num_edge_total-1].len = 1;
// 		nodeIDs[num_edge_total-1].p = (int*) calloc((unsigned long)(nodeIDs[num_edge_total-1].len), sizeof(int) );
// 		nodeIDs[num_edge_total-1].q = (int*) calloc((unsigned long)(nodeIDs[num_edge_total-1].len), sizeof(int) );
		
		
		//QVector <SoMaterial *> tmp_cylmatVect;
		// /*hvl_t* new_*/cylradiusVect = (hvl_t*) realloc(cylradiusVect,(int)(num_edge_total)*sizeof(hvl_t) );
		//cylradiusVect[num_edge_total-1].len = 1;
		//cylradiusVect[num_edge_total-1].p = (float*) calloc((unsigned long)(cylradiusVect[num_edge_total-1].len), sizeof(float) );
				

		scene = new SoSeparator();		//create one node for each cylinder and show them into Scene Graph
		
		geometry = new GeometryNode(scene, data, rf, this);
		childItems.insert(childCount(), geometry);
		//this->rf = rf;

		root->addChild(scene);
		GeometryNode::load_Cylinder_output_type output_loadCylinder;
		QFile new_edge_name("additional_edge");
		output_loadCylinder = geometry->loadCylinder(new_edge_name,centreX,centreY,centreZ,newedge_rad,tangentX,tangentY,tangentZ,heights,0);
		tmp_cylmatVect.append(output_loadCylinder.nodematerial);
		tmp_nodeid.append(output_loadCylinder.nodeid);
		tmp_childid.append(root->getNumChildren()-1);	//so that number of children-1 becomes the children indeces
		cylmatVect.append(tmp_cylmatVect);
		tmp_cylradiusVect.append(output_loadCylinder.noderadius);
		cylradiusVect.append(tmp_cylradiusVect);
// 		cylradiusVect[num_edge_total-1].p[0] = output_loadCylinder.noderadius;
// 		nodeIDs[num_edge_total-1].p[0]= output_loadCylinder.nodeid;
// 		nodeIDs[num_edge_total-1].q[0]= root->getNumChildren()-1;
		nodeIDs.append(tmp_nodeid);
		childIDs.append(tmp_childid);
		
		new_radius.append(newedge_rad);
		new_heights.append(heights);
		new_cntrx.append(centreX); new_cntry.append(centreY); new_cntrz.append(centreZ);
		new_tangx.append(tangentX); new_tangy.append(tangentY); new_tangz.append(tangentZ);
		tmp_cylmatVect.resize(0); tmp_cylradiusVect.resize(0);  tmp_nodeid.resize(0); tmp_childid.resize(0);
		
	}
	else{
		cout << "New edge ("<<connectingVect[0] << "," << connectingVect[1]<< ") already exists!" << endl;
	}
	
	num_clicked = 0;
	connectingVect.resize(0);
	//qDebug() << "\nGeometryScene::addEdge() " ;

}

bool GeometryScene::loadGeometry(QFile &file, const QVector<QVariant> &data,
		ResourceForm *rf, TreeItem *parent){
	//qDebug() << "Debug. >>GeometryScene::loadGeometry()";
	root = viewer->getRootSeparator();
	
	QFileInfo fileInfo(file);
    QString ext = fileInfo.suffix();


    if (ext == "h5") {
		//std::cout << "Loading Line object as a cylinder: " <<std::endl;
		////read the h5 file and pass the radius and tangent height and center of vetex v
		struct h5_output_type h5_output_data = H5_reader ((char*)file.fileName().toLatin1().data());
		//std::cout << "Successfully read the h5 file " << file.fileName().toLatin1().data() << "!" <<std::endl;
		num_edge_total = h5_output_data.num_edge;
		for (int i =0; i < num_edge_total; i++)
		{
			labelVect.append(h5_output_data.label[i]);
			edgesVect.append(h5_output_data.edges[2*i]);
			edgesVect.append(h5_output_data.edges[2*i+1]);
		}
		for (int i =0; i < h5_output_data.num_vertex; i++)
		{
			vertices_centreVect.append(h5_output_data.vertices[3*i]);
			vertices_centreVect.append(h5_output_data.vertices[3*i+1]);
			vertices_centreVect.append(h5_output_data.vertices[3*i+2]);
		}
		//std::cout << "Size of edges " << num_edge_total <<std::endl;
// 		//save unique cylinder NodeId in order of cylinder numbers from GeometryNode::loadCylinder in here
		// /*hvl_ti **/nodeIDs = (hvl_ti2*) malloc((int)(num_edge_total)*sizeof(hvl_ti2) );	
		//cylmatVect = (hvl_tSoMat*) malloc((int)(num_edge_total)*sizeof(hvl_tSoMat) );	
		//cylradiusVect = (hvl_t*) malloc((int)(num_edge_total)*sizeof(hvl_t) );
		
		GeometryNode::load_Cylinder_output_type output_loadCylinder;
		
		for( int e = 0; e < num_edge_total; e++ ) {
// 			nodeIDs[e].len = h5_output_data.centreX[e].len;
// 			nodeIDs[e].p = (int*) malloc((unsigned long)(nodeIDs[e].len)*sizeof(int) );
// 			nodeIDs[e].q = (int*) malloc((unsigned long)(nodeIDs[e].len)*sizeof(int) );
			//cylmatVect[e].len = h5_output_data.centreX[e].len;
			//(cylmatVect[e].p).resize(cylmatVect[e].len);
			//cylradiusVect[e].len = h5_output_data.centreX[e].len;
			//cylradiusVect[e].p = (float*) calloc((unsigned long)(cylradiusVect[e].len),sizeof(float) );
			
			for (int c =0; c < h5_output_data.centreX[e].len ; c++){
				scene = new SoSeparator();		//create one node for each cylinder and show them into Scene Graph
				//scene->addChild(new SoDirectionalLight);	// light
	
				geometry = new GeometryNode(scene, data, rf, this);
				childItems.insert(childCount(), geometry);
				//this->rf = rf;
				root->addChild(scene);
				output_loadCylinder = geometry->loadCylinder(file,h5_output_data.centreX[e].p[c],h5_output_data.centreY[e].p[c],h5_output_data.centreZ[e].p[c],h5_output_data.radius[e].p[c],h5_output_data.tangentX[e].p[c],h5_output_data.tangentY[e].p[c],h5_output_data.tangentZ[e].p[c],1.1*h5_output_data.heights[e].p[c],h5_output_data.label[e]);
				tmp_nodeid.append(output_loadCylinder.nodeid);
				tmp_childid.append(root->getNumChildren()-1);
// 				nodeIDs[e].p[c]= output_loadCylinder.nodeid;
// 				nodeIDs[e].q[c]= root->getNumChildren()-1;
	 			//std::cout << "parent num children : " << root->getNumChildren() <<std::endl;
				//cylmatVect[e].p[c]= output_loadCylinder.nodematerial;
				tmp_cylmatVect.append(output_loadCylinder.nodematerial);
				tmp_cylradiusVect.append(output_loadCylinder.noderadius);
				//cylradiusVect[e].p[c]= output_loadCylinder.noderadius; 
			}
			cylmatVect.append(tmp_cylmatVect);
			cylradiusVect.append(tmp_cylradiusVect);
			nodeIDs.append(tmp_nodeid);
			childIDs.append(tmp_childid);
/*			std::cout << "parent num children : "<< std::flush;
			for (int i =0; i< tmp_childid.size(); i++){
				std::cout<< tmp_childid[i]<< std::flush; }
			std::cout <<""<<std::endl;*/
			tmp_cylmatVect.resize(0); tmp_cylradiusVect.resize(0); tmp_nodeid.resize(0); tmp_childid.resize(0);
		}
		free (h5_output_data.centreX);	free (h5_output_data.centreY);	free (h5_output_data.centreZ);
		free (h5_output_data.tangentX);	free (h5_output_data.tangentY);	free (h5_output_data.tangentZ);
		free (h5_output_data.radius);		free (h5_output_data.heights);	free (h5_output_data.label);
		free (h5_output_data.edges);		free (h5_output_data.vertices);
		radiusCalc(cylradiusVect);
		std::cout << "min and max radius: "<<radiusmin << ", " << radiusmax <<std::endl;
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
			//qDebug() << "Debug: << geometry->loadObj #" <<i <<" status = " << status;
		}
	}

	//qDebug() << "Debug. <<GeometryScene::loadGeometry()";
	return true;
}

void GeometryScene::createLabelForm() {
	this->rf = rf;

	connect(rf->uilabel.radiusSlider, SIGNAL(valueChanged(int)),
			this, SLOT(updateRadiusTransparency(int)));
	connect(rf->uilabel.LabelVessel, SIGNAL(activated( int )),
			this, SLOT(getUserLabel()));		
	connect(rf->uilabel.Saveh5, SIGNAL(clicked()),
			this, SLOT(saveLabel()));
	connect(rf->uilabel.Saveash5, SIGNAL(clicked()),
			this, SLOT(saveasLabel()));
	connect(rf->uilabel.DeleteEdge, SIGNAL(clicked()),
		   this, SLOT(deleteThisEdge()));
	connect(rf->uilabel.AddEdge, SIGNAL(clicked()),
		   this, SLOT(createNewEdge()));
/*	connect(this, SIGNAL(createEdgeSignal()),
		   this, SLOT(addNewEdge()));*/
			
	rf->uilabel.DeleteEdge->setEnabled ( FALSE );
 	rf->uilabel.AddEdge->setEnabled ( TRUE );
	rf->uilabel.label->setEnabled ( FALSE );
	rf->uilabel.LabelVessel->setEnabled ( FALSE );
	rf->uilabel.Saveh5->setEnabled ( TRUE );
	rf->uilabel.Saveash5->setEnabled ( TRUE );
	rf->uilabel.radiusSlider->setEnabled ( TRUE );
	rf->uilabel.radius->setEnabled ( TRUE );
} 	





void GeometryScene::saveasLabel(){
	//qDebug() << "\nGeometryScene::saveasLabel() " ;
	QString slname = QFileDialog::getSaveFileName(0,
                tr("Save h5 to File"), QString(), tr("Files (*.h5)"));
	if (! slname.isNull()) {
		QFile slfile(slname);
		QFileInfo fileInfo(slfile);
		QString abspath = fileInfo.absoluteFilePath();
		//qDebug() << "construct a new file " + slfile.fileName() ;
	
		
		QFile currfile(h5_filename);
		QFileInfo currfileInfo(currfile);
		QString currabspath = currfileInfo.absoluteFilePath();
		//std:cout<< "old " << currabspath.toLatin1().data() <<" new " << abspath.toLatin1().data() << " compare " << currabspath.compare( abspath)<< std::endl;
   
		if ((int)currabspath.compare(abspath) != 0){	//if file specified in saveas is not the same as the file originally opened
			saveh5FileFunc(slfile);
		}
		else {
			saveLabel();
		}
	}
	
	//qDebug() << "\nGeometryScene::saveasLabel() " ;
}

void GeometryScene::saveh5FileFunc(QFile &newfile){
	//qDebug() << "\nGeometryScene::saveh5FileFunc() " ;
	if (QFile::exists(newfile.fileName())){
		bool status2 = QFile::remove (newfile.fileName());
		//qDebug() << "\nGeometryScene::saveh5FileFunc() remove existing file " + newfile.fileName()+ " " +status2 ;
	}
// copy h5_filename to filename and replace the name of h5_filename, then call saveLabel
	/*QFile currentfile;
	currentfile.setFileName(h5_filename);
	*/QFile newfile2;
	newfile2.setFileName(newfile.fileName());
	if(!QFile::copy ( h5_filename,newfile2.fileName() ))
//	if(!QFile::copy(filename.fileName(), h5_filename))
    //if(!file.copy(newfile))
    {
        qDebug() << "\n File copy failed .. " + h5_filename + " to " + newfile2.fileName() + " Error : " + newfile2.errorString();
    }
     else{ 
        //qDebug() << "\n File copied .. " + h5_filename + " to " + newfile2.fileName();
		h5_filename = newfile2.fileName();
		saveLabel();
	}
	//qDebug() << "\nGeometryScene::saveh5FileFunc() " ;

}


void GeometryScene::saveLabel(){
	//qDebug() << "\nGeometryScene::saveLabel() " ;
	QFile tmpfile( "tmp.h5");	///there is a random unresolved error where sometimes this function fails with Segmentation Fault,so we save the current changes in a tmp file for now until this bug is fixed, so that your most recent changes won't get lost
	if (QFile::exists(tmpfile.fileName())){QFile::remove(tmpfile.fileName());}
	QFile::copy(h5_filename,tmpfile.fileName() );

	//qDebug() <<del_edgesVect.size() << " "<<del_edgesVect ;
	qSort (del_edgesVect);
	//qDebug() <<del_edgesVect.size() << " "<<del_edgesVect ;	
	for (int i=del_edgesVect.size()-1; i>-1;i--){
		int tmp_e_num=del_edgesVect[i];
		//qDebug() << "e_num deleted:" << tmp_e_num;
		nodeIDs.remove(tmp_e_num);	
		childIDs.remove(tmp_e_num);
		labelVect.remove(tmp_e_num);
		edgesVect.remove(2*tmp_e_num+1);
		edgesVect.remove(2*tmp_e_num);
		cylmatVect.remove(tmp_e_num);
		cylradiusVect.remove(tmp_e_num);
		//qDebug() << "e_num deleted2:" << tmp_e_num;
		for (int i=0; i<cylnum_transparency0_5.size(); i++){
			if (cylnum_transparency0_5[i] == tmp_e_num){
				cylnum_transparency0_5.remove(i);
			}
		}
	}
	for (int i=del_edgesVect.size()-1; i>-1;i--){
		int tmp_e_num=del_edgesVect[i];
		for (int i=0; i<cylnum_transparency0_5.size(); i++){
				if (cylnum_transparency0_5[i] > tmp_e_num){
					cylnum_transparency0_5[i]--;
			}
		}
	}

	//qDebug() << "done deleting" ;
	
	if (labelVect.size()!= (edgesVect.size()/2)){
		std::cout <<"ERROR: Numbe of new edges added to the label and edges are not equal!\nAborted!!"<<endl;
		return;
	}
	if ( (new_radius.size()!=(new_edgesVect.size()/2)) || (new_radius.size()!=new_heights.size()) || (new_radius.size()!=new_cntrx.size()) || (new_radius.size()!=new_cntry.size()) || (new_radius.size()!=new_cntrz.size()) || (new_radius.size()!=new_tangx.size()) || (new_radius.size()!=new_tangy.size()) || (new_radius.size()!=new_tangz.size())){
		std::cout <<"ERROR: Numbe of new edges added to the new_radius and edges are not equal!\nAborted!!"<<endl;
		return;
	}
	//cout << new_edgesVect.size()/2 << " " <<new_radius.size() << " " <<new_heights.size()<< " " <<new_cntrx.size()<< " " <<new_tangx.size()<<" new edges have been created" << endl;
	//cout << edgesVect.size()/2 << " total number of edges="<< num_edge_total << endl;
	
	///since the dataset is not chunked extend function will not work for it. C++ API does not have the dataset delete function => we use C API
	int *Label_buf = (int*) malloc( sizeof(int) * labelVect.size() );
	for (int i=0; i<labelVect.size(); i++){
		Label_buf[i]=labelVect[i];
	}

	hid_t       file_id, group_id, dataset, dataspace, hdf_plist;  
	herr_t      status;
	int rank = 2;
	hsize_t  dims[2];	dims[0] = labelVect.size(); 	dims[1] = 1;
	
	file_id = H5Fopen(h5_filename.toLatin1().data(), H5F_ACC_RDWR, H5P_DEFAULT);

	///write the labels to the graph
	group_id = H5Gopen (file_id, "/vessel_graph/edge_properties", H5P_DEFAULT);
	status = H5Ldelete (group_id, "cyl_label", H5P_DEFAULT);
	dataspace = H5Screate_simple(rank, dims, NULL);
	dataset = H5Dcreate2(group_id, "cyl_label", H5T_NATIVE_INT, dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	status = H5Dwrite(dataset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, Label_buf);
	status = H5Dclose(dataset);
	status = H5Gclose (group_id);	
	status = H5Fclose (file_id);
 	//cout << "wrote label dataset!" << endl;
	free(Label_buf);
	
	
	
	if (new_edgesVect.size()>0 or del_edgesVect.size()>0){
		int num_new_e = new_edgesVect.size()/2;
		file_id = H5Fopen(h5_filename.toLatin1().data(), H5F_ACC_RDWR, H5P_DEFAULT);

		int status_n;
		
		///write the edges to the graph
		int *edge_buf = (int*) malloc( sizeof(int) * edgesVect.size() );
		for (int i=0; i<edgesVect.size(); i++){
			edge_buf[i]=edgesVect[i];
		}
		dims[0] = labelVect.size(); 	dims[1] = 2;
		group_id = H5Gopen (file_id, "/vessel_graph", H5P_DEFAULT);
		status = H5Ldelete (group_id, "edges", H5P_DEFAULT);
		dataspace = H5Screate_simple(rank, dims, NULL);
		dataset = H5Dcreate2(group_id, "edges", H5T_NATIVE_INT, dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
		status = H5Dwrite(dataset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, edge_buf);
		status = H5Dclose(dataset);
		status = H5Gclose (group_id);	
		status = H5Fclose (file_id);
		free(edge_buf);
 		//cout << "wrote edge dataset!" << endl;
		
		/*//write new_edges
		int *new_edge_buf = (int*) malloc( sizeof(int) * new_edgesVect.size() );
		for (int i=0; i<new_edgesVect.size(); i++){
			new_edge_buf[i]=new_edgesVect[i];
		}
			
		dims[0] = new_edgesVect.size()/2; 	dims[1] = 2;
		status = H5Lexists(group_id,"new_edges", H5P_DEFAULT);
		if (!status){		///new_edges doesn't exist => create and write
			dataspace = H5Screate_simple(rank, dims, NULL);
			dataset = H5Dcreate2(group_id, "new_edges", H5T_NATIVE_INT, dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
			cout << "new_edges dataset didn't exist" << endl;
			status = H5Dwrite(dataset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, new_edge_buf);
			status= H5Dclose(dataset);
			status = H5Gclose (group_id);	
			cout << "created and wrote new_edges dataset!" << status << endl;
		}
		else {	///new_edges exist => read to buffer, resize buffer to write new edges, delete new_edges and write the new one!
			dataset = H5Dopen2(group_id, "new_edges", H5P_DEFAULT);
			dataspace = H5Dget_space(dataset);    // dataspace handle 
			rank  = H5Sget_simple_extent_ndims(dataspace);
			status_n  = H5Sget_simple_extent_dims(dataspace, dims, NULL);
			cout << "new_edges dataset rank " << rank << " dims[0][1] " << dims[0] << "," << dims[1] << endl;
			int *new_edge_buf_i = (int*) malloc( sizeof(int) * dims[0]*dims[1] );
			status = H5Dread(dataset, H5T_NATIVE_INT, dataspace, dataspace, H5P_DEFAULT, new_edge_buf_i);
			cout << "new_edges [0][1]" << new_edge_buf_i[0] << " " << new_edge_buf_i[1] << endl;
			new_edge_buf_i = (int*) realloc (new_edge_buf_i, sizeof(int)*((unsigned long)(dims[0]*dims[1])+new_edgesVect.size()));
			for (int i=0; i<new_edgesVect.size() ; i++){
				new_edge_buf_i[i+(dims[0]*dims[1])] = new_edge_buf[i];
			}
			status= H5Dclose(dataset);
			dims[0] += new_edgesVect.size()/2;
			status = H5Ldelete (group_id, "new_edges", H5P_DEFAULT);
			dataspace = H5Screate_simple(rank, dims, NULL);
			dataset = H5Dcreate2(group_id, "new_edges", H5T_NATIVE_INT, dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
			status = H5Dwrite(dataset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, new_edge_buf_i);
			status = H5Dclose(dataset);
			status = H5Gclose (group_id);	
			cout << "wrote new_edges dataset " << status << endl;
			free(new_edge_buf_i);
		}
		free(new_edge_buf);
		cout << "freed new_edge_buf" << endl;
		*/

		///write intermediaries
// 		rank = 1;
// 		hsize_t  dims_cyl[1];
// 		hid_t    datatype;
// 		file_id = H5Fopen(h5_filename.toLatin1().data(), H5F_ACC_RDWR, H5P_DEFAULT);
// 		group_id = H5Gopen (file_id, "/vessel_graph/edge_properties", H5P_DEFAULT);
// 		
// 		hid_t       dataset_intm,dataspace_intm,datatype_intm;
// 		dataset_intm = H5Dopen2(group_id, "intermediaries", H5P_DEFAULT);
// 		dataspace_intm = H5Dget_space(dataset_intm);    // dataspace handle 
// 		rank  = H5Sget_simple_extent_ndims(dataspace_intm);
// 		status_n  = H5Sget_simple_extent_dims(dataspace_intm, dims_cyl, NULL);
// 		hvl_ti* new_buf_i_intm = (hvl_ti*) malloc( sizeof(hvl_ti) * (unsigned long)(dims_cyl[0]) );
// 		cout << "intermediaries dataset rank " << rank << " dims_cyl[0] " << dims_cyl[0] << " intermediaries.size " << num_new_e << endl;
// 		datatype_intm  = H5Dget_type(dataset_intm);     // datatype handle 
// 		status = H5Dread(dataset_intm, datatype_intm, dataspace_intm, dataspace_intm, H5P_DEFAULT, new_buf_i_intm);
// 		cout << "intermediaries [0][1]" << new_buf_i_intm[0].p[0] << " " << new_buf_i_intm[1].p[0] << endl;
// 		new_buf_i_intm = (hvl_ti*) realloc (new_buf_i_intm, sizeof(hvl_ti) * (unsigned long)(dims_cyl[0]+num_new_e) );
// 		for (int i=0; i<num_new_e; i++){
// 			new_buf_i_intm[i+dims_cyl[0]].len =1;
// 			new_buf_i_intm[i+dims_cyl[0]].p = (int *) malloc( sizeof(int) );
// 			new_buf_i_intm[i+dims_cyl[0]].p[0]= -1;			/// ??? what to do with the intermediaries of new edges to be empty => should add new_interm to be empty or in graph2graph if its -1 then put []
// 		}
// 		dims_cyl[0] += num_new_e;
// 		cout << "dims_cyl[0]" << dims_cyl[0] << endl;
// 		if (del_edgesVect.size()>0){
// 			hsize_t del_dims_cyl[1]; del_dims_cyl[0] = num_edge_total;	//if there is del_edgesVect
// 			cout << "final dims_cyl[0]" << del_dims_cyl[0] << endl;
// 			for (int i=0; i<del_edgesVect.size(); i++){
// 				cout << "i=" << i << " edge #"<< del_edgesVect[i]<<endl;
// 				new_buf_i_intm[del_edgesVect[i]].len=0;
// 			}
// 			hvl_ti* new_buf_o_intm = (hvl_ti*) malloc(sizeof(hvl_ti) * (unsigned long)(del_dims_cyl[0]));
// 			int j =0;
// 			for (int i=0; i< dims_cyl[0]; i++){
// 				if (new_buf_i_intm[i].len!=0){
// 					new_buf_o_intm[j].len = new_buf_i_intm[i].len;
// 					new_buf_o_intm[j].p = (int *) malloc(sizeof(int)*new_buf_i_intm[i].len);
// 					for (int k=0; k<new_buf_i_intm[i].len; k++){
// 						new_buf_o_intm[j].p[k]=new_buf_i_intm[i].p[k];
// 					}
// 					j++; 
// 				}
// 			}
// 			cout << "done"<<endl;
// 			status = H5Dset_extent(dataset_intm, del_dims_cyl);
// 			cout << status<<endl;
// 			status = H5Dwrite(dataset_intm, datatype_intm, H5S_ALL, H5S_ALL, H5P_DEFAULT, new_buf_o_intm);
// 			cout << status << endl;
// 			free(new_buf_o_intm);
// 			cout << "Freed new_buf_o_intm"<< endl;
// 		}
// 		//status = H5Dextend (dataset_cx, dims_cyl);
// 		/*//status = H5Ldelete (group_id, "cyl_centreX", H5P_DEFAULT);
// 		//dataspace_cx = H5Screate_simple(rank, dims_cyl, NULL);
// 		//dataset_cx = H5Dcreate2(group_id, "cyl_centreX", datatype_cx, dataspace_cx, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);*/
// 		else {
// 			status = H5Dextend (dataset_intm, dims_cyl);
// 			status = H5Dwrite(dataset_intm, datatype_intm, H5S_ALL, H5S_ALL, H5P_DEFAULT, new_buf_i_intm);
// 		}
// 		status = H5Dclose(dataset_intm);
// 		cout << "wrote intermediaries dataset! "<<status << endl;
//  		free(new_buf_i_intm);
// 		cout << "freed new_buf_i_intm" << endl;
// 		status = H5Gclose (group_id);	
// 		status = H5Fclose (file_id);
// 		
		
		
		///write cyl_properties
		char* cyl_prop = "cyl_centreX";
		writeCylProp2H5(cyl_prop,num_new_e, new_cntrx, del_edgesVect);
		
		cyl_prop = "cyl_centreY";
		writeCylProp2H5(cyl_prop,num_new_e, new_cntry, del_edgesVect);
		
		cyl_prop = "cyl_centreZ";
		writeCylProp2H5(cyl_prop,num_new_e, new_cntrz, del_edgesVect);
		
		cyl_prop = "cyl_radius";
		writeCylProp2H5(cyl_prop,num_new_e, new_radius, del_edgesVect);
		
		cyl_prop = "cyl_height";
		writeCylProp2H5(cyl_prop,num_new_e, new_heights, del_edgesVect);
		
		cyl_prop = "cyl_tangentX";
		writeCylProp2H5(cyl_prop,num_new_e, new_tangx, del_edgesVect);
			
		cyl_prop = "cyl_tangentY";
		writeCylProp2H5(cyl_prop,num_new_e, new_tangy, del_edgesVect);
			
		cyl_prop = "cyl_tangentZ";
		writeCylProp2H5(cyl_prop,num_new_e, new_tangz, del_edgesVect);
			
		///empty the vectors of new edge properties after they were written into the file
		new_radius.resize(0);
		new_heights.resize(0);
		new_cntrx.resize(0); new_cntry.resize(0); new_cntrz.resize(0);
		new_tangx.resize(0); new_tangy.resize(0); new_tangz.resize(0);
		new_edgesVect.resize(0);
		del_edgesVect.resize(0);	
	}

	
/*	///with C++ API
	H5::H5File Lh5file = H5::H5File();
	Lh5file.openFile(h5_filename.toLatin1().data(), H5F_ACC_RDWR );
	
	H5::Group edge_properties_group = Lh5file.openGroup("/vessel_graph/edge_properties");
	std::cout<< "opening label dataset to write the user label" <<std::endl;
	H5::DataSet dataset_label = edge_properties_group.openDataSet("label");
	H5::DataSpace dataspace = dataset_label.getSpace();	//Get dataspace of the dataset.
	hsize_t dims_out[1];// Get the dimension size of each dimension in the dataspace and display them.
	int rank = dataspace.getSimpleExtentDims( dims_out, NULL);
//  	std::cout << "/vessel_graph/edge_properties/label dims_out[0],[1]: " <<dims_out[0] << "," << dims_out[1]<< " , ndims = " << ndims <<" , rank = " << rank <<endl;
	
	if (labelVect.size()> dims_out[0])
	{	
		std::cout <<"Old number of edges were " << dims_out[0] << " with rank " << rank << std::flush;
		dims_out[0] = labelVect.size();
		std::cout <<" New edges have been added to the graph! "<< dims_out[0] <<endl;
	}
	
	
// 	hsize_t      maxdims[2] = {H5::H5S_UNLIMITED, H5::H5S_UNLIMITED};
	H5::DataSpace fspace( rank, dims_out);	// , maxdims //Create dataspace for the dataset in the file.

	H5::DSetCreatPropList cparms;
	hsize_t      chunk_dims[2] ={1, 1};
	cparms.setChunk( rank, chunk_dims );
	int fill_val = 0;
	cparms.setFillValue( H5::PredType::NATIVE_INT, &fill_val);
	cout << "cparms" << endl;
	H5::DataSet* new_dataset_l = new H5::DataSet(edge_properties_group.createDataSet("new_label2", H5::PredType::NATIVE_INT, fspace,cparms));	//Create dataset and write it into the file.
	cout << "New dataset created!" << endl;
	dims_out[0]+=1;
	new_dataset_l->extend( dims_out );
	cout << "New dataset extended!" << endl;
// 	dataset_label.extend( dims_out );
// 	cout << "old dataset extended!" << endl;


	
	int *Label_buf = (int*) malloc( sizeof(int) * labelVect.size() );
	for (int i=0; i<labelVect.size(); i++){
		Label_buf[i]=labelVect[i];
	}
	
	new_dataset_l->write(Label_buf,H5::PredType::NATIVE_INT, fspace, fspace);
	//dataset_label.write(Label_buf,H5::PredType::NATIVE_INT, fspace, fspace);
	
	new_dataset_l->close();
	dataset_label.close();
	edge_properties_group.close();
	
	
	///write the new edges to the graph
	H5::Group e_group = Lh5file.openGroup("/vessel_graph");
	H5::DataSet dataset_edge = e_group.openDataSet("edges");
	H5::DataSpace dataspace_e = dataset_edge.getSpace();	//Get dataspace of the dataset.
	hsize_t dims_out_e[2];// Get the dimension size of each dimension in the dataspace and display them.
	int rank_e = dataspace_e.getSimpleExtentDims( dims_out_e, NULL);
//  	std::cout << "/vessel_graph/edge_properties/label dims_out[0],[1]: " <<dims_out[0] << "," << dims_out[1]<< " , ndims = " << ndims <<" , rank = " << rank <<endl;
	
	if ((edgesVect.size()/2)> dims_out_e[0])
	{	
		std::cout <<"Old number of edges were " << dims_out_e[0] << " with rank " << rank_e << std::flush;
		dims_out_e[0] = edgesVect.size()/2;
		std::cout <<" New edges have been added to the graph! " << dims_out_e[0] <<endl;
	}
	
	H5::DataSpace fspace_e( rank_e, dims_out_e );
	
	int *edge_buf = (int*) malloc( sizeof(int) * edgesVect.size()*dims_out_e[1] );
	for (int i=0; i<edgesVect.size(); i++){
		//cout << i << " " << flush;
		edge_buf[i]=edgesVect[i];
		//edge_buf[2*i+1]=edgesVect[2*i+1];
	}
	
	dataset_edge.write(edge_buf,H5::PredType::NATIVE_INT, fspace_e, fspace_e);

	dataset_edge.close();
	e_group.close();

	
	Lh5file.close();
	*/
	
 	qDebug() << "Debug. >>GeometryScene::saveLabel()...Successfully wrote the labeled H5 file" << h5_filename.toLatin1().data()<<"!\n";
}

/** function to write cylinder cx,cy,cz,h,r,tx,ty,tz properties as part of the save function 
* global variables used: h5_filename,num_edge_total 
*/		
void GeometryScene::writeCylProp2H5(char * prop_name,int num_new_edge, QVector <float> new_edges_prop_val, QVector <int> deleted_edgesVect){		
		hid_t       file_id, group_id, dataset, dataspace, datatype, hdf_plist;  
		herr_t      status;
		int rank = 1;
		hsize_t  dims[1];
		int status_n;
		
		file_id = H5Fopen(h5_filename.toLatin1().data(), H5F_ACC_RDWR, H5P_DEFAULT);
		group_id = H5Gopen (file_id, "/vessel_graph/edge_properties", H5P_DEFAULT);
		
		dataset = H5Dopen2(group_id, prop_name, H5P_DEFAULT);
		dataspace = H5Dget_space(dataset);    // dataspace handle 
		rank  = H5Sget_simple_extent_ndims(dataspace);
		status_n  = H5Sget_simple_extent_dims(dataspace, dims, NULL);
		hvl_tf* buf_i = (hvl_tf*) malloc( sizeof(hvl_tf) * (unsigned long)(dims[0]) );
		//cout << "cyl_centreX dataset rank " << rank << " dims[0] " << dims[0] << " num_new_edge " << num_new_edge << endl;
		datatype  = H5Dget_type(dataset);     // datatype handle 
		status = H5Dread(dataset, datatype, dataspace, dataspace, H5P_DEFAULT, buf_i);
		//cout << status << prop_name<<"[0][1]" << buf_i[0].p[0] << " " << buf_i[1].p[0] << endl;
		if (num_new_edge>0)
		{
			buf_i = (hvl_tf*) realloc (buf_i, sizeof(hvl_tf) * (unsigned long)(dims[0]+num_new_edge) );
			for (int i=0; i<num_new_edge; i++){
				buf_i[i+dims[0]].len =1;
				buf_i[i+dims[0]].p = (float *) malloc( sizeof(float) );
				buf_i[i+dims[0]].p[0]= new_edges_prop_val[i];
			}
			dims[0] += num_new_edge;
			//cout << "dims[0]" << dims[0] << endl;
		}
		if (deleted_edgesVect.size()>0){
			hsize_t del_dims[1]; del_dims[0] = num_edge_total;	//if there is deleted_edgesVect
		//	cout << "final dims[0]" << del_dims[0] << endl;
			for (int i=0; i<deleted_edgesVect.size(); i++){
				buf_i[deleted_edgesVect[i]].len=0;
				//cout << "i=" << i << " edge #"<< deleted_edgesVect[i]<<endl;
			}
			hvl_tf* buf_o = (hvl_tf*) malloc(sizeof(hvl_tf) * (unsigned long)(del_dims[0]));
			int j =0;
			for (int i=0; i< dims[0]; i++){
				if (buf_i[i].len!=0){
					buf_o[j].len = buf_i[i].len;
					buf_o[j].p = (float *) malloc(sizeof(float)*buf_i[i].len);
					for (int k=0; k<buf_i[i].len; k++){
						buf_o[j].p[k]=buf_i[i].p[k];
					}
					j++;
					//cout << j << " " << flush;
				}
			}
			//cout << j << " done deleting"<<endl;
			status = H5Dset_extent(dataset, del_dims);
			//cout << status<<endl;
			status = H5Dwrite(dataset, datatype, H5S_ALL, H5S_ALL, H5P_DEFAULT, buf_o);
			//cout << status << endl;
			free(buf_o);
			//cout << "Freed buf_o"<< endl;
		}
		else {
			status = H5Dextend (dataset, dims);
			status = H5Dwrite(dataset, datatype, H5S_ALL, H5S_ALL, H5P_DEFAULT, buf_i);
		}
		status = H5Dclose(dataset);
		//cout << "wrote "<< prop_name <<" dataset! "<<status << endl;
 		free(buf_i);
		//cout << "freed buf_i" << endl;

		status = H5Gclose (group_id);	
		status = H5Fclose (file_id);

}

void GeometryScene::getUserLabel(){
	//qDebug() << "GeometryScene::getUserLabel() " ;
 	int userLabelnum = 0;
 	QString userLabel;
//	bool ok;
	//for the selected cylinder I get the NodeID which corresponds to a cylinder number N=> open H5 and read the N-th segment_ID , then save label as new field for all the edges that have that segment_ID
	//we then read this in python to assign labels to actual corrected output of vessel tracking
// 	int e_num;
// 	for (int i=0; i< num_edge_total ; i++){
// 		for (int c=0; c<nodeIDs[i].len ; c++){
// 			if (nodeIDs[i].p[c]== nodeId){
// 				e_num=i;
// 				break;	//e_num will have the number of the picked cylinder object
// 			}
// 		}
// 	}
	//qDebug() << "user entered a label ..." ;	
// 	userLabel = QInputDialog::getInteger(0, "Label selected vessel segment", "Label number [1:255.0]:", labelVect[e_num],0,255.0,1,&ok); //opens a dialog with title Label selected vessel segment, inside it message Label number
	userLabel = rf->uilabel.LabelVessel->currentText();
	qDebug() << "user entered label " << userLabel ;
	
				// 	//if user cancled entering label dont' do following steps
				// 	if (!ok){
				// 		return;
				// 	}
				//	//std::cout << "User entered Label " <<userLabel << " for vessel segment with NodeId " << nodeId << " of selected cylinder number " << e_num << " with label " << labelVect[e_num] <<std::endl;

	userLabelnum = label_Name2num (userLabel);
					
	//check if this vessel segment was labeled before differently? if it wasnt 0 and different from userLabel, ask relable?
 	if (labelVect[e_num]!=userLabelnum && labelVect[e_num]!=0) {		//it has been labeled with a different label from new userLabel 
//	if (labelVect[e_num].compare(userLabel)!=0 && labelVect[e_num].compare (QString("No label\0"))!=0) {		//it has been labeled with a different label from new userLabel 
		QString response = QString("Selected vessel was labelled before.Do you want to relabel?");
		int answer = QMessageBox::question(0, "Relabel vessel?", response, QMessageBox::Yes | QMessageBox::No);
	// 	std::cout << " After MessageBox" << std::endl;
		if (answer == QMessageBox::Yes){
			//relabel
			labelVect[e_num]=userLabelnum;
		}
	}
 	else if (labelVect[e_num]==0 ){	//it wasn't labeled before =>label
		labelVect[e_num]=userLabelnum;
	}
	//std::cout << "labelVect.size() "<< labelVect.size() << " : " << e_num << " "<< labelVect[e_num]<< std::endl;
	
	
	//color the selected vessel segment to color from lookup table based on label, transpar=0.5
 	if (labelVect[e_num]!=0 ){	//it was labeled before => color and transparency
//	if (labelVect[e_num].compare (QString("No label\0"))!=0 ){	//it was labeled before => color and transparency
		//std::cout <<"bing" << std::endl;
		geometry->updateCylinderColour(/*cylmatVect[e_num].len, cylmatVect[e_num].p*/cylmatVect[e_num].size(), cylmatVect[e_num],labelVect[e_num]);
		//std::cout <<"bing" << std::endl;
		if (!cylnum_transparency0_5.contains(e_num)){
			cylnum_transparency0_5.append(e_num);
		}
		//std::cout <<"bing" << std::endl;
	}
	
	//color the selected vessel segment to undo labeling: r=255.0, g=255.0, b=255.0, transpar=0.0
	else{
		geometry->updateCylinderColour(/*cylmatVect[e_num].len, cylmatVect[e_num].p*/cylmatVect[e_num].size(), cylmatVect[e_num], 0,0.0);
		int trans_ind= cylnum_transparency0_5.lastIndexOf(e_num);
		if (trans_ind !=-1){ 
			cylnum_transparency0_5.remove(trans_ind);
		}
	}
	//qDebug() << "GeometryScene::getUserLabel() " ;	
}


void GeometryScene::updateRadiusTransparency(int newVal){
// 	qDebug() << "Debug. <<GeometryScene::updateRadiusTransparency()";
	//find the threshold radius, find all radius in QVect smaller than threshold and call geometrynode::updateCylTransparency
	float radthresh= (newVal*(radiusmax- radiusmin)/100)+radiusmin;
// 	std::cout<< " Threshold radius: " << radthresh << std::endl;
	for (int e=0; e<num_edge_total; e++){
		float mean_rad=0;
		for (int c=0; c<cylradiusVect[e].size(); c++){
			mean_rad += cylradiusVect[e][c];
		}
		mean_rad = mean_rad/cylradiusVect[e].size();
		if (mean_rad<=radthresh) {
			geometry->updateCylTransparency( /*cylmatVect[e].len, cylmatVect[e].p*/cylmatVect[e].size(), cylmatVect[e], 1);
		}
		else if (mean_rad>radthresh) {
			if (cylnum_transparency0_5.contains(e)){
				geometry->updateCylTransparency( /*cylmatVect[e].len, cylmatVect[e].p*/cylmatVect[e].size(), cylmatVect[e], 0.5);
			}
			else{
				geometry->updateCylTransparency( /*cylmatVect[e].len, cylmatVect[e].p*/cylmatVect[e].size(), cylmatVect[e], 0.0);
			}
		}
	}
// 	qDebug() << "Debug. >>GeometryScene::updateRadiusTransparency()";
}

GeometryScene::h5_output_type GeometryScene::H5_reader (char* dbfile){
	H5::H5File h5file = H5::H5File();
	h5file.openFile(dbfile, H5F_ACC_RDONLY); //read only access. H5F_ACC_RDWR :read and write access 
	//cout << "File: " << h5file.getFileName() << "\nSize:" << h5file.getFileSize() << "\nObject Count:" << h5file.getObjCount() << endl;
	H5::Group edge_properties_group = h5file.openGroup("/vessel_graph/edge_properties");
	//cout << "edge_properties group has " << edge_properties_group.getNumObjs() << " object in it. Its ID is " << edge_properties_group.getId() << endl;
	struct h5_output_type h5_data;

	/*/// using C++ API
	cout << "reading segment ID (type variable length array of int)..." <<endl;	
	H5::DataSet dataset_segmentID = edge_properties_group.openDataSet("cyl_segment_ID");
	H5::DataSpace dataspace_ID = dataset_segmentID.getSpace();	//Get filespace for rank and dimension of the dataset.
	//int rank = dataspace_ID.getSimpleExtentNdims();//Get the number of dimensions in the file dataspace.
	hsize_t dims_ID[1];// dataset dimensions
	int rank_ID = dataspace_ID.getSimpleExtentDims( dims_ID, NULL);//Get the number of dimensions and the dimension sizes of the file dataspace
	hssize_t num_element_ID = dataspace_ID.getSimpleExtentNpoints();	//Get number of elements in a dataspace
	cout << "dims_ID[0],[1] = " <<dims_ID[0]<< " , rank_ID = " << rank_ID << " , num_element_ID = " << num_element_ID <<endl;
	H5::DataSpace memspace_ID( rank_ID, dims_ID );
	H5::DataType dtype = dataset_segmentID.getDataType();
	//hsize_t buff_size_ID = dataset_segmentID.getVlenBufSize(dtype,dataspace_ID );
	hvl_ti * segmentID_data = (hvl_ti*) malloc((unsigned long)(dims_ID[0])*sizeof(hvl_ti));
	dataset_segmentID.read(segmentID_data ,dtype ,memspace_ID, dataspace_ID); 	
 	///// 	dataset_segmentID.read(segmentID_data,H5::H5T_VARIABLE,memspace_ID, dataspace_ID);
	cout << segmentID_data[0].len << " , " << segmentID_data[1].len << " , " << segmentID_data[2].len << " , " << segmentID_data[3].len << " , " << endl;
	cout << segmentID_data[0].p[0] << " , " << segmentID_data[0].p[1] << " , " << segmentID_data[1].p[0] << " , " << segmentID_data[2].p[1] << " , "<< segmentID_data[6].p[0] << endl;
 	dataset_segmentID.close();
	*/
	/*/// using C API	
	edge_properties_group.close(); 
	h5file.close();
	hid_t       file_id, dataset_id;  
	herr_t      status;
	// hsize_t size;
	hid_t tid1 = H5Tvlen_create (H5T_NATIVE_INT);
	file_id = H5Fopen(dbfile, H5F_ACC_RDWR, H5P_DEFAULT);
	dataset_id = H5Dopen(file_id, "/vessel_graph/edge_properties/cyl_segment_ID", H5P_DEFAULT);
	hvl_t * rdataid = (hvl_t*) malloc((unsigned long)(dims[0])*sizeof(hvl_t));
		status =H5Dread(dataset_id, tid1, H5S_ALL, H5S_ALL, H5P_DEFAULT, rdataid);
		cout << status << endl;
		cout << rdataid[0].len << " , " << rdataid[1].len << " , " << rdataid[2].len << " , " << rdataid[3].len << " , " << endl;
		cout << rdataid[0].p[0] << " , " << rdataid[0].p[1] << " , " << rdataid[1].p[0] << " , " << rdataid[2].p[1] << " , "<< rdataid[6].p[0] << endl;
	status = H5Dclose(dataset_id);
	status = H5Fclose(file_id);
	h5file.openFile(dbfile, H5F_ACC_RDONLY); //read only access. H5F_ACC_RDWR :read and write access 
	edge_properties_group = h5file.openGroup("/vessel_graph/edge_properties");
	*/		

	
	
	//cout << "\n\n\nreading edge centres X,Y,Z (type variable length array of float)..." <<endl;	
	H5::DataSet dataset_centreX =edge_properties_group.openDataSet("cyl_centreX");
	H5::DataSet dataset_centreY =edge_properties_group.openDataSet("cyl_centreY");
	H5::DataSet dataset_centreZ =edge_properties_group.openDataSet("cyl_centreZ");
	H5::DataSpace dataspace_c = dataset_centreX.getSpace();	//Get dataspace of the dataset.
	hsize_t dims_c[1];// Get the dimension size of each dimension in the dataspace and display them.
	int rank_c = dataspace_c.getSimpleExtentDims( dims_c, NULL);
	H5::DataSpace memspace_c ( rank_c, dims_c );
	H5::DataType ctype = dataset_centreX.getDataType();
	h5_data.centreX /*hvl_t * centreX_struc*/ = (hvl_tf*) malloc((unsigned long)(dims_c[0])*sizeof(hvl_tf));
	h5_data.centreY /*hvl_t * centreY_struc*/ = (hvl_tf*) malloc((unsigned long)(dims_c[0])*sizeof(hvl_tf));
	h5_data.centreZ /*hvl_t * centreZ_struc*/ = (hvl_tf*) malloc((unsigned long)(dims_c[0])*sizeof(hvl_tf));
	dataset_centreX.read(h5_data.centreX ,ctype ,memspace_c, dataspace_c); 	
	dataset_centreY.read(h5_data.centreY ,ctype ,memspace_c, dataspace_c); 	
	dataset_centreZ.read(h5_data.centreZ ,ctype ,memspace_c, dataspace_c); 	
	//cout << h5_data.centreX[0].len << " , " << h5_data.centreX[1].len << " , " << h5_data.centreX[2].len << " , " << h5_data.centreX[3].len << " , " << endl;
	//cout << h5_data.centreX[0].p[0] << " , " << h5_data.centreX[0].p[1] << " , " << h5_data.centreX[1].p[0] << " , " << h5_data.centreX[2].p[1] << " , "<< h5_data.centreX[6].p[0] << endl;
 	dataset_centreX.close();
 	dataset_centreY.close();
 	dataset_centreZ.close();
	
	
	
	//cout << "reading edge tangents X,Y,Z (type variable length array of float)..." <<endl;	
	H5::DataSet dataset_tangentX =edge_properties_group.openDataSet("cyl_tangentX");
	H5::DataSet dataset_tangentY =edge_properties_group.openDataSet("cyl_tangentY");
	H5::DataSet dataset_tangentZ =edge_properties_group.openDataSet("cyl_tangentZ");
	H5::DataSpace dataspace_t = dataset_tangentX.getSpace();	//Get dataspace of the dataset.
	hsize_t dims_t[1];// Get the dimension size of each dimension in the dataspace and display them.
	int rank_t = dataspace_t.getSimpleExtentDims( dims_t, NULL);
	//H5::DataSpace memspace_t ( rank_t, dims_t );
	if ((unsigned long)(dims_c[0])!= dims_t[0]) {
		cout <<"ERROR: the number of the edges centres are not equal to the number of tangents!" <<endl;
		cout << "Aborted"<<endl;
		exit(0);
	}
	H5::DataType ttype = dataset_tangentX.getDataType();
	h5_data.tangentX /*hvl_t * tangentX_struc*/ = (hvl_tf*) malloc((unsigned long)(dims_t[0])*sizeof(hvl_tf));
	h5_data.tangentY /*hvl_t * tangentY_struc*/ = (hvl_tf*) malloc((unsigned long)(dims_t[0])*sizeof(hvl_tf));
	h5_data.tangentZ /*hvl_t * tangentZ_struc*/ = (hvl_tf*) malloc((unsigned long)(dims_t[0])*sizeof(hvl_tf));
	dataset_tangentX.read(h5_data.tangentX ,ttype ,dataspace_t, dataspace_t); 	
	dataset_tangentY.read(h5_data.tangentY ,ttype ,dataspace_t, dataspace_t); 	
	dataset_tangentZ.read(h5_data.tangentZ ,ttype ,dataspace_t, dataspace_t); 	
	//cout << h5_data.tangentX[0].len << " , " << h5_data.tangentX[1].len << " , " << h5_data.tangentX[2].len << " , " << h5_data.tangentX[3].len << " , " << endl;
	//cout << h5_data.tangentX[0].p[0] << " , " << h5_data.tangentX[0].p[1] << " , " << h5_data.tangentX[1].p[0] << " , " << h5_data.tangentX[2].p[1] << " , "<< h5_data.tangentX[6].p[0] << endl;
 	dataset_tangentX.close();
 	dataset_tangentY.close();
 	dataset_tangentZ.close();
	
	
	
	//cout << "reading edge heights (type variable length array of float)..." <<endl;	
	H5::DataSet dataset_heights =edge_properties_group.openDataSet("cyl_height");	
	H5::DataSpace dataspace_h = dataset_heights.getSpace();	//Get dataspace of the dataset.
	hsize_t dims_h[1];// Get the dimension size of each dimension in the dataspace and display them.
	int rank_h = dataspace_h.getSimpleExtentDims( dims_h, NULL);
	if ((unsigned long)(dims_c[0])!= dims_h[0]) {
		cout <<"ERROR: the number of the edges centres are not equal to the number of heights!" <<endl;
		cout << "Aborted"<<endl;
		exit(0);
	}
	H5::DataType htype = dataset_heights.getDataType();
	h5_data.heights /*hvl_t * height_struc*/ = (hvl_tf*) malloc((unsigned long)(dims_h[0])*sizeof(hvl_tf));
	dataset_heights.read(h5_data.heights ,htype ,dataspace_h, dataspace_h); 	
	//cout << h5_data.heights[0].len << " , " << h5_data.heights[1].len << " , " << h5_data.heights[2].len << " , " << h5_data.heights[3].len << " , " << endl;
	//cout << h5_data.heights[0].p[0] << " , " << h5_data.heights[0].p[1] << " , " << h5_data.heights[1].p[0] << " , " << h5_data.heights[2].p[1] << " , "<< h5_data.heights[6].p[0] << endl;
 	dataset_heights.close();
	
	
	
	//cout << "reading edge radius (type variable length array of float)..." <<endl;	
	H5::DataSet dataset_radius =edge_properties_group.openDataSet("cyl_radius");
	H5::DataSpace dataspace_r = dataset_radius.getSpace();	//Get dataspace of the dataset.
	hsize_t dims_r[1];// Get the dimension size of each dimension in the dataspace and display them.
	int rank_r = dataspace_r.getSimpleExtentDims( dims_r, NULL);
	if ((unsigned long)(dims_c[0])!= dims_r[0]) {
		cout <<"ERROR: the number of the edges centres are not equal to the number of radius!" <<endl;
		cout << "Aborted"<<endl;
		exit(0);
	}
	H5::DataType rtype = dataset_radius.getDataType();
	h5_data.radius /*hvl_t * radius_struc*/ = (hvl_tf*) malloc((unsigned long)(dims_r[0])*sizeof(hvl_tf));
	dataset_radius.read(h5_data.radius ,rtype ,dataspace_r, dataspace_r); 	
	//cout << h5_data.radius[0].len << " , " << h5_data.radius[1].len << " , " << h5_data.radius[2].len << " , " << h5_data.radius[3].len << " , " << endl;
	//cout << h5_data.radius[0].p[0] << " , " << h5_data.radius[0].p[1] << " , " << h5_data.radius[1].p[0] << " , " << h5_data.radius[2].p[1] << " , "<< h5_data.radius[6].p[0] << endl;
 	dataset_radius.close();
	
	
	
	//cout << "reading edge label (integer)..." <<endl;	
	H5::DataSet dataset_label =edge_properties_group.openDataSet("cyl_label");
	H5::DataSpace dataspace_l = dataset_label.getSpace();	//Get filespace for rank and dimension of the dataset.
	//int rank_l = dataspace_l.getSimpleExtentNdims();//Get the number of dimensions in the file dataspace.
	hsize_t dims_l[2];// dataset dimensions
	int rank_l = dataspace_l.getSimpleExtentDims( dims_l, NULL);//Get the dimension sizes of the file dataspace; Retrieves dataspace dimension size and Returns Number of dimensions the same value as returned by DataSpace::getSimpleExtentNdims()	
	if ((unsigned long)(dims_c[0])!= dims_l[0]) {
		cout <<"ERROR: the number of the edges centres are not equal to the number of labels!" <<endl;
		cout << "Aborted"<<endl;
		exit(0);
	}
	//hssize_t num_element = dataspace_l.getSimpleExtentNpoints();	//Get number of elements in a dataspace
	//cout << "dims[0],[1] = " <<dims_l[0] /*<< "," << dims_l[1]*/<< " , rank_l = " << rank_l << " , num_element_l = " << num_element <<endl;
	H5::DataSpace memspace_l( rank_l, dims_l );
	h5_data.label /*int * cyl_label*/ = (int*) calloc((unsigned long)(dims_l[0]), sizeof(int));
	dataset_label.read(h5_data.label,H5::PredType::NATIVE_INT,memspace_l, dataspace_l);	//read from file(dataspace) to memoryspace
 	//std::cout << "cyl_label[0] is: "<< h5_data.label[0] << " , " << h5_data.label[1] << " , " << h5_data.label[2] << " , " << h5_data.label[3]<<std::endl;
	dataset_label.close();
	
	
		
	/*cout << "reading edge ID (integer)..." <<endl;	
	H5::DataSet dataset_id =edge_properties_group.openDataSet("edge_ID");
	H5::DataSpace dataspace_id = dataset_id.getSpace();	//Get filespace for rank and dimension of the dataset.
	hsize_t dims_id[1];// dataset dimensions
	int rank_id = dataspace_id.getSimpleExtentDims( dims_id, NULL);//Get the dimension sizes of the file dataspace; Retrieves dataspace dimension size and Returns Number of dimensions the same value as returned by DataSpace::getSimpleExtentNdims()	
	if ((unsigned long)(dims_c[0])!= dims_id[0]) {
		cout <<"ERROR: the number of the edges centres are not equal to the number of edge IDs!" <<endl;
		cout << "Aborted"<<endl;
		exit(0);
	}
	h5_data.id = (int*) calloc((unsigned long)(dims_id[0]), sizeof(int));
	dataset_id.read(h5_data.id,H5::PredType::NATIVE_INT,dataspace_id, dataspace_id);	//read from file(dataspace) to memoryspace
 	std::cout << "edge_id[0] is: "<< h5_data.id[0] << " , " << h5_data.id[1] << " , " << h5_data.id[2] << " , " << h5_data.id[3]<<std::endl;
	dataset_id.close();
	*/
	edge_properties_group.close(); 
	
	//cout << "reading edges (integer,integer)..." <<endl;	
	H5::Group edge_group = h5file.openGroup("/vessel_graph");
	H5::DataSet dataset_e =edge_group.openDataSet("edges");
	H5::DataSpace dataspace_e = dataset_e.getSpace();	//Get filespace for rank and dimension of the dataset.
	hsize_t dims_e[2];// dataset dimensions
	int rank_e = dataspace_e.getSimpleExtentDims( dims_e, NULL);//Get the dimension sizes of the file dataspace; Retrieves dataspace dimension size and Returns Number of dimensions the same value as returned by DataSpace::getSimpleExtentNdims()	
	if ((unsigned long)(dims_c[0])!= dims_e[0]) {
		cout <<"ERROR: the number of the edges centres are not equal to the number of edges!" <<endl;
		cout << "Aborted"<<endl;
		exit(0);
	}
	//cout << "dims[0],[1] = " <<dims_e[0] << "," << dims_e[1]<< " , rank_e = " << rank_e <<endl;
	h5_data.edges = (int*) calloc((unsigned long)(dims_e[0]*dims_e[1]), sizeof(int));
	dataset_e.read(h5_data.edges,H5::PredType::NATIVE_INT,dataspace_e, dataspace_e);	//read from file(dataspace) to memoryspace
 	//std::cout << "edge_e[0] is: "<< h5_data.edges[0] << " , " << h5_data.edges[1] << " , " << h5_data.edges[2] << " , " << h5_data.edges[3]<<std::endl;
	dataset_e.close();
	edge_group.close(); 
	
	
	//cout << "reading vertices centre (float,float,float)..." <<endl;	
	H5::Group vertex_group = h5file.openGroup("/vessel_graph/vertex_properties");
	H5::DataSet dataset_v = vertex_group.openDataSet("centre");
	H5::DataSpace dataspace_v = dataset_v.getSpace();	//Get filespace for rank and dimension of the dataset.
	hsize_t dims_v[2];// dataset dimensions
	int rank_v = dataspace_v.getSimpleExtentDims( dims_v, NULL);//Get the dimension sizes of the file dataspace; Retrieves dataspace dimension size and Returns Number of dimensions the same value as returned by DataSpace::getSimpleExtentNdims()	
	//cout << "dims[0],[1] = " <<dims_v[0] << "," << dims_v[1] << " , rank_v = " << rank_v <<endl;
	h5_data.vertices = (float*) calloc((unsigned long)(dims_v[0]*dims_v[1]), sizeof(float));
	dataset_v.read(h5_data.vertices,H5::PredType::NATIVE_FLOAT,dataspace_v, dataspace_v);	//read from file(dataspace) to memoryspace
 	//std::cout << "vertice_centre[0] is: "<< h5_data.vertices[0] << " , " << h5_data.vertices[1] << " , " << h5_data.vertices[2] << " , " << h5_data.vertices[3]<<std::endl;
	dataset_v.close();
	vertex_group.close(); 

	h5file.close();
	
	h5_data.num_edge = dims_c[0];
	h5_data.num_vertex = dims_v[0];

    //cout << "Debug. << h5reader closing " << dbfile << endl;
	return h5_data;
}

void GeometryScene::noPointReceived(){
// 	rf->uitag.CreateTagPoint->setEnabled ( FALSE );
// 	rf->uitag.AaddTagPoint->setEnabled ( FALSE );
	rf->uilabel.label->setEnabled ( FALSE );
	rf->uilabel.LabelVessel->setEnabled ( FALSE );
	rf->uilabel.DeleteEdge->setEnabled ( FALSE );
	//rf->uilabel.AddEdge->setEnabled ( FALSE );
}

int GeometryScene::whichEdgeIndx (int edge_ind1,int edge_ind2,int x, int y,int z){
	int a1 = vertices_centreVect[3*edge_ind1]; int b1 = vertices_centreVect[3*edge_ind1+1]; int c1 = vertices_centreVect[3*edge_ind1+2];
	float distance1 = sqrt((x-a1)*(x-a1)+(y-b1)*(y-b1)+(z-c1)*(z-c1));
	int a2 = vertices_centreVect[3*edge_ind2]; int b2 = vertices_centreVect[3*edge_ind2+1]; int c2 = vertices_centreVect[3*edge_ind2+2];
	float distance2 = sqrt((x-a2)*(x-a2)+(y-b2)*(y-b2)+(z-c2)*(z-c2));
	if (distance1<distance2){
		return edge_ind1;
	}
	else {
		return edge_ind2;
	}
}

void GeometryScene::pickReceived(int index, int id,int real_id, float x, float y, float z,SoType objtype) {
  	//std::cout << "Debug. >>GeometryScene::pickReceived(): x,y,z " << x << " " << y << " " << z << " index " << index << " nodeId(id) " << id << " real_id " << real_id << "num_edge_total "  << num_edge_total << std::endl;
	nodeId=id;
	//if clicked on polygon enable Tag buttons, else if enabled, disable it:
	if (objtype == SoFaceDetail::getClassTypeId() or objtype == SoLineDetail::getClassTypeId()){
		rf->uitag.CreateTagPoint->setEnabled ( TRUE );
		rf->uitag.AddTagPoint->setEnabled ( FALSE );
		rf->uitag.TagSize->setEnabled ( TRUE );
		rf->uitag.TagColour->setEnabled ( TRUE );
		rf->uilabel.label->setEnabled ( FALSE );		
		rf->uilabel.LabelVessel->setEnabled ( FALSE );		
		rf->uilabel.DeleteEdge->setEnabled ( FALSE );		
		//rf->uilabel.AddEdge->setEnabled ( FALSE );
	}
		
	else if (objtype == SoCylinderDetail::getClassTypeId()) {
		rf->uitag.CreateTagPoint->setEnabled ( FALSE );
		rf->uitag.AddTagPoint->setEnabled ( FALSE );
		rf->uitag.TagSize->setEnabled ( FALSE );
		rf->uitag.TagColour->setEnabled ( FALSE );
		//rf->uilabel.LabelVessel->setEnabled ( TRUE );
		//int e_num;
		for (int i=0; i< num_edge_total+del_edgesVect.size() ; i++){
			//std::cout << i << ":"<< nodeIDs[i].size() << std::endl;
			for (int c=0; c<nodeIDs[i].size() ; c++){
				if (nodeIDs[i][c]== nodeId){
					e_num=i;
					break;	//e_num will have the number of the picked cylinder object
				}
			}
		}
		qDebug() << /*"NodeId " << nodeId <<"="<< nodeIDs[e_num][0]<< " Vessel segment number " << e_num << " is */"edge ("<< edgesVect[2*e_num] << "," << edgesVect[2*e_num+1] << ") has label #" << labelVect[e_num] << " : " << label_num2Name(labelVect[e_num]);                              
		if (flag_new_edge){	//we are waiting for 2 edges to be clicked for connecting
			rf->uilabel.label->setEnabled ( FALSE );		
			rf->uilabel.LabelVessel->setEnabled ( FALSE );		
			rf->uilabel.DeleteEdge->setEnabled ( FALSE );		
			
			num_clicked++;
			
			if (num_clicked==1){
				edge1_indx = e_num;
				int edge_indx = whichEdgeIndx (edgesVect[2*e_num],edgesVect[2*e_num+1],x,y,z);	// pick one of the following based on location clicked and edge centers:
				connectingVect.append(edge_indx); 	//connectingVect.append(edgesVect[2*e_num]); or //connectingVect.append(edgesVect[2*e_num+1]);
			}
			if (num_clicked==2){
				if (edge1_indx==e_num){		//clicked on same edge 2wice => no new edge => wait for new edge to be clicked
					num_clicked--;
				}
				else{
					edge2_indx = e_num;
					int edge_indx = whichEdgeIndx (edgesVect[2*e_num],edgesVect[2*e_num+1],x,y,z);	// pick one of the following based on location clicked and edge centers:
					connectingVect.append(edge_indx); 	//connectingVect.append(edgesVect[2*e_num]); or //connectingVect.append(edgesVect[2*e_num+1]);
				}
			}
			if (connectingVect.size()==2){
				flag_new_edge = FALSE;
				addNewEdge();
				//emit createEdgeSignal();
			}
			
		}
		else{//we have clicked on an edge in h5 and not in the middle of creating a new edge	
			rf->uilabel.label->setEnabled ( TRUE );		
			rf->uilabel.LabelVessel->setEnabled ( TRUE );		
			rf->uilabel.DeleteEdge->setEnabled ( TRUE );		
			rf->uilabel.AddEdge->setEnabled ( TRUE );		
		}
	}
	else {		
		rf->uitag.CreateTagPoint->setEnabled ( FALSE );
		rf->uitag.AddTagPoint->setEnabled ( FALSE );
		rf->uitag.TagSize->setEnabled ( FALSE );
		rf->uitag.TagColour->setEnabled ( FALSE );
		rf->uilabel.label->setEnabled ( FALSE );		
		rf->uilabel.LabelVessel->setEnabled ( FALSE );		
		rf->uilabel.DeleteEdge->setEnabled ( FALSE );		
		//rf->uilabel.AddEdge->setEnabled ( FALSE );	
	}
	

	// check to see whether picked point belongs to this scene.
    if (id == geometry->getID()) {
		//std::cout <<"picked point does belong to this scene" <<std::endl;
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

//qDebug() << "Debug. >>GeometryScene::pickReceived()";
	
}


void GeometryScene::wasSelected() {
	//qDebug() << "Debug. >>GeometryScene::wasSelected()";
	rf->setLastGeometry(this);
	//qDebug() << "Debug. <<GeometryScene::wasSelected()";
}


bool GeometryScene::loadVertstats(QFile &file) {
	//qDebug() << "Debug. >>GeometryScene::loadVertstats()";
	textureFileItem *vert = new textureFileItem(scene, itemData, form, this);
	//qDebug() << "Debug. >>GeometryScene::loadVertstats() vert";
	vert->loadFile(file);
	//qDebug() << "Debug. >>GeometryScene::loadVertstats() loadFile";
	childItems.insert(childCount(), vert);
	//qDebug() << "Debug. >>GeometryScene::loadVertstats() childItems";
	// make sure that the texture file receives the signal if local point was selected in
	// a ray-picking action.
	connect(this, SIGNAL(localPointPicked(int)), vert, SLOT(pickedPointReceived(int)));

	//qDebug() << "Debug. <<GeometryScene::loadVertstats()";
	return true;
}


bool GeometryScene::updateLabelLUColor() {
	//qDebug() << "Debug. >>GeometryScene::updateLabelLUColor()";
	for( int e = 0; e < labelVect.size(); e++ ) {
		geometry->updateCylinderColour(/*cylmatVect[e].len, cylmatVect[e].p*/cylmatVect[e].size(), cylmatVect[e],labelVect[e],0.0);
	}
	//qDebug() << "Debug. <<GeometryScene::updateLabelLUColor()";
	return true;
}




void GeometryScene::radiusCalc(QVector<QVector <float> > cylradiusVect){
	radiusmin=cylradiusVect[0][0];
	radiusmax=cylradiusVect[0][0];
	for (int e=0; e<num_edge_total; e++){
		for (int c=0; c<cylradiusVect[e].size(); c++){
			if (radiusmin>	cylradiusVect[e][c]){ radiusmin= cylradiusVect[e][c];}
			if (radiusmax<	cylradiusVect[e][c]){ radiusmax= cylradiusVect[e][c];}
		}
	}
	//std::cout << "min and max radius: "<<radiusmin << ", " << radiusmax <<std::endl;
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
	
	return labelnum;
}














