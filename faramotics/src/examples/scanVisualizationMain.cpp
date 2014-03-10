
#include "../src/sceneRender.h"
#include "../src/dynamicSceneRender.h"
//#include "../src/groundRender.h"
#include "../src/rangeSector.h"
#include "../src/rangeScan2D.h"
//#include "../src/rangeImage.h"


using namespace std;

int main(int argc, char** argv)
{
	CdynamicSceneRender *myRender;
	CrangeScan2D *myScanner; 
	Cpose3d viewPoint;
	Cpose3d devicePose;
	vector<float> myScan;
	string modelFileName;
	string edgesFileName;
	unsigned int ii;
	
	//user params: model and view point
 	modelFileName = "../models/campusNordUPC.obj";
 	edgesFileName = "../models/campusNordUPC_edges.obj";
//	modelFileName = "/home/andreu/iriSw/algorithms/faramotics/trunk/models/fmeBigDoorOpen.obj";
// 	modelFileName = "/home/andreu/iriSw/faramotics/models/fmeBigDoor.obj";
// 	edgesFileName = "";
	devicePose.setPose(2,8,0.2,0,0,0);
// 	devicePose.setFullPose(9,9,0.2,M_PI/2.0,0,0);
	//devicePose.setFullPose(12.9807,7.11034,0.439993,2.07945,-4.2718e-13,3.14159);
	
	//glut initialization
	glutInit(&argc, argv);
	
	//create display and load 3D model
	myRender = new CdynamicSceneRender(1200,700,90*M_PI/180,90*700.0*M_PI/(1200.0*180.0),0.2,100);
      myRender->loadAssimpModel("../models/secretarysDesk.obj");
      //smyRender->loadAssimpModel("../models/LargeMansion.blend");
	//myRender->loadModel(modelFileName,edgesFileName);
      //myRender->loadModel(DEBUG_SCENE);
      //myRender->loadModel(SPHERE);
	
	//create scanner and load 3D model
	//myScanner = new CrangeScan2D(LEUZE_RS4);
	//myScanner = new CrangeScan2D(HOKUYO_UTM30LX_180DEG);
	//myScanner->loadModel(modelFileName);
	//myScanner->loadModel(DEBUG_SCENE);
      //myScanner->loadAssimpModel("../models/secretarysDesk.obj");
      //myScanner->loadModel(SPHERE);

      sleep(1);
	for (ii=0; ii<1000; ii++)
	{
		//if (ii==0) sleep(1);
		//moves the device position
		if (ii<=1000)
		{
                  devicePose.rt.setEuler( devicePose.rt.head()-2*M_PI/180., devicePose.rt.pitch(), devicePose.rt.roll() );
                  //devicePose.pt(0) = devicePose.pt(0) + 0.1;
		}
// 		if ( (ii>40) && (ii<=80) )
// 		{
//                   devicePose.pt(0) = devicePose.pt(0) + 0.1;
//                   devicePose.rt.setEuler(devicePose.rt.head(), devicePose.rt.pitch(), devicePose.rt.roll() + 0.01);
// 		}
// 		if ( (ii>80) && (ii<=120) )
// 		{
//                   devicePose.pt(0) = devicePose.pt(0) + 0.1;
//                   devicePose.rt.setEuler(devicePose.rt.head(), devicePose.rt.pitch(), devicePose.rt.roll() - 0.01);
// 		}
// 		if ( (ii>120) && (ii<=170) )
// 		{
//                   devicePose.rt.setEuler( devicePose.rt.head()+1.8*M_PI/180., devicePose.rt.pitch(), devicePose.rt.roll() );
//                   devicePose.moveForward(0.2);
// 		}
// 		if ( (ii>170) && (ii<=220) )
// 		{
// 			devicePose.rt.setEuler( devicePose.rt.head()-1.8*M_PI/180., devicePose.rt.pitch(), devicePose.rt.roll() );
// 		}
// 		if ( (ii>220) && (ii<=300) ) 
// 		{
// 			devicePose.pt(0) = devicePose.pt(0) + 0.1;
// 		}
// 		if ( (ii>300) && (ii<=500) ) 
// 		{
// 			devicePose.rt.setEuler( devicePose.rt.head()-1.*M_PI/180., devicePose.rt.pitch(), devicePose.rt.roll() );
// 			devicePose.moveForward(0.1);
// 		}
// 		if ( (ii>500) && (ii<=1000) ) 
// 		{
// 			devicePose.rt.setEuler( devicePose.rt.head()+1.*M_PI/180., devicePose.rt.pitch(), devicePose.rt.roll() );
// 			devicePose.moveForward(0.);
// 		}
		
		//compute scan
		//myScan.clear();
		//myScanner->computeScan(devicePose,myScan);

		//draws the scene and the scan
		//myRender->drawScan(devicePose,myScan,190.0*M_PI/180.0,95.0*M_PI/180.0); //leuze 
		myRender->drawPoseAxis(devicePose);
		//myRender->drawScan(devicePose,myScan,M_PI,M_PI/2.0); //hokuyo 180 deg
		
            //locate view point
		//myRender->setViewPoint(devicePose.getX()+6,devicePose.getY(),15,0,M_PI/2.0,0);//overhead view
		//myRender->setViewPoint(devicePose);
 		viewPoint.setPose(devicePose);
 		viewPoint.rt.setEuler( viewPoint.rt.head(), viewPoint.rt.pitch()+20.*M_PI/180., viewPoint.rt.roll() );
 		viewPoint.moveForward(-7);
            
            //renders the scene
 		myRender->setViewPoint(viewPoint);
		myRender->render();

		//sleep to have time to see something
		usleep(50000);
	}
	
	//delete objects
	delete myRender;
	//delete myScanner;

	return 0;
}
