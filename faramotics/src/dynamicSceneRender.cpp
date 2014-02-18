#include "dynamicSceneRender.h"

CdynamicSceneRender::CdynamicSceneRender(unsigned int ww, unsigned int hh, float hAp, float vAp, float nearZ, float farZ) : CsceneRender(ww,hh,hAp,vAp,nearZ,farZ)
{
      //generate lists
      frameList = glGenLists(1);
      scanHitsList = glGenLists(1);
}

CdynamicSceneRender::~CdynamicSceneRender()
{

}

void CdynamicSceneRender::render()
{
   	lookAtValues lav;
	
	glutSetWindow(winId);
// 	cout << "render(): " << __LINE__ << ": " << winId << endl;
	//if (!isVisible) this->hide();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear Screen And Depth Buffer with previous values
	glMatrixMode(GL_MODELVIEW); //necessary to perform viewing and model transformations
	glLoadIdentity(); //resets the model/view matrix
	viewPoint.getLookAt(lav); //gets look at values from a position expressed as (x,y,z,h,p,r)
	gluLookAt(lav.ex,lav.ey,lav.ez,lav.ax,lav.ay,lav.az,lav.ux,lav.uy,lav.uz);
	glCallList(modelList);
	glCallList(scanHitsList);
	glCallList(frameList);
// 	cout << "render(): " << __LINE__ << ": " << modelList << "," << scanHitsList << "," << frameList << endl;
	glFinish();
}

void CdynamicSceneRender::drawScan(Cpose3d devicePose, vector<float> scan, double aperture, double firstAngle)
//void CdynamicSceneRender::drawScan()
{
	//GLuint dynamicObjectsList;
	unsigned int ii;
	Cpose3d scanPoint;
	double delta;
	
	delta = aperture/(double)scan.size();
            
      //set window and list
	glutSetWindow(winId);	
	glNewList(scanHitsList, GL_COMPILE);
	
// 	glColor3f(0.,1.,0.);
	glColor3f(1.,0.,0.);
// 	glMatrixMode(GL_MODELVIEW); //necessary to perform viewing and model transformations	

	for (ii=0; ii<scan.size(); ii++)
	{
		//cout << scan.at(ii) << " " ;
		//compute the point wrt model
		scanPoint.setPose(devicePose);
		scanPoint.rt.turnHeading(firstAngle-(double)ii*delta);//head the ray
		//scanPoint.turnZaxis(aperture/2.0-(double)ii*delta);//head the ray
		scanPoint.moveForward(scan.at(ii));//move following the ray
		//scanPoint.printPosition();
		
		//draws the scan point
		//glLoadIdentity(); //resets the model/view matrix
		glTranslatef(scanPoint.pt(0),scanPoint.pt(1),scanPoint.pt(2));// moves model origin
		gluSphere(gluNewQuadric(),0.1, 5,5);
		//gluNewQuadric();
		//gluCylinder(gluNewQuadric(),0.005,0.005,0.005,4,2);
		glTranslatef(-scanPoint.pt(0),-scanPoint.pt(1),-scanPoint.pt(2));// moves model origin
	}
	//cout << endl;

/*
	//center ray in red
	glColor3f(1.,0.,0.);
	scanPoint.setFullPose(devicePose);
	scanPoint.moveForward(scan.at(scan.size()/2));//move following the ray
 	glTranslatef(scanPoint.getX(),scanPoint.getY(),scanPoint.getZ());// moves model origin
 	gluSphere(gluNewQuadric(),0.2, 10,10);
	glTranslatef(-scanPoint.getX(),-scanPoint.getY(),-scanPoint.getZ());// moves model origin
	
	//center+1 ray in grey
	glColor3f(0.5,0.5,0.5);
	scanPoint.setFullPose(devicePose);
	scanPoint.turnZaxis(firstAngle-(double)(scan.size()/2 + 1)*delta);//head the ray
	scanPoint.moveForward(scan.at(scan.size()/2 + 1));//move following the ray
 	glTranslatef(scanPoint.getX(),scanPoint.getY(),scanPoint.getZ());// moves model origin
 	gluSphere(gluNewQuadric(),0.2, 10,10);
*/
	
	glEndList();
	glFinish();	//finish all openGL work
}

void CdynamicSceneRender::drawPoseAxis(Cpose3d & axis)
{
	//GLuint dynamicObjectsList;
	double vv[6];

      //gets pose as XYZ, fwd and left vectors
      axis.getFwdLft(vv);      
      
      //set current window
	glutSetWindow(winId);	
      
      //set list
	glNewList(frameList, GL_COMPILE);
	
	//x axis
	glColor3f(1.,0.,0.);
	glTranslatef(axis.pt(0),axis.pt(1),axis.pt(2));// moves model origin
	glRotated(90.0,vv[3],vv[4],vv[5]);
	gluCylinder(gluNewQuadric(),0.02,0.02,0.5,10,10);
	glRotated(90.0,vv[3],vv[4],vv[5]);
	
	//y axis
	glColor3f(0.,1.,0.);
	glRotated(-90.0,vv[0],vv[1],vv[2]);
	gluCylinder(gluNewQuadric(),0.02,0.02,0.5,10,10);	
	glRotated(-90.0,vv[0],vv[1],vv[2]);
	
	//z axis
	glColor3f(0.,0.,1.);
	gluCylinder(gluNewQuadric(),0.02,0.02,0.5,10,10);	

      //Ends list
	glEndList();
	glFinish();	//finish all openGL work
}
