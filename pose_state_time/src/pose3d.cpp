#include "pose3d.h"

Cpose3d::Cpose3d()
{
      //point init to origin. Rotation init already done at Crotation constructor
	pt = 0.,0.,0.;
}

Cpose3d::Cpose3d(const double x0, const double y0, const double z0, const double eaH, const double eaP, const double eaR, const bool rd)
{
      //set point and rotation given by euler angle arguments
	pt = x0,y0,z0;
	this->rt.setEuler(eaH,eaP,eaR,rd);
}

Cpose3d::~Cpose3d()
{
      //nothing to do
}

void Cpose3d::setPose(Cpose3d & pose)
{
      this->pt = pose.pt;
      this->rt = pose.rt;
}

void Cpose3d::setPose(const dlib::matrix<double,6,1> & ptQuat)
{
      dlib::matrix<double,3,1> qijk;
      this->pt = ptQuat(0),ptQuat(1),ptQuat(2);//point part: x,y,z coordinates
      qijk = ptQuat(3),ptQuat(4),ptQuat(5);
      this->rt.setQuaternion( qijk );//quaternion part: imaginary coordinates
}

void Cpose3d::setPose(const dlib::matrix<double,4,4> & hM)
{
	this->rt.setMatrix( dlib::subm(hM,dlib::range(0,2),dlib::range(0,2)) );
	this->pt(0)=hM(0,3); this->pt(1)=hM(1,3); this->pt(2)=hM(2,3);
}

void Cpose3d::getPtQuat(dlib::matrix<double,6,1> & ptQuat)
{
      dlib::matrix<double,3,1> qijk;
      ptQuat(0) = this->pt(0); ptQuat(1) = this->pt(1); ptQuat(2) = this->pt(2);
      //dlib::set_colm(ptQuat,dlib::range(0,2)) = this->pt;
      this->rt.getQuaternion(qijk);
      ptQuat(3) = qijk(0); ptQuat(4) = qijk(1); ptQuat(5) = qijk(2);  
      //dlib::set_colm(ptQuat,dlib::range(3,5)) = qijk(0),qijk(1),qijk(2);
}

void Cpose3d::getHmat(dlib::matrix<double,4,4> & hM)
{
      dlib::matrix<double,3,3> rM;
      this->rt.getMatrix(rM);
      //dlib::subm(hM, dlib::range(0,2), dlib::range(0,2))
      dlib::set_subm(hM, dlib::range(0,2), dlib::range(0,2)) = rM; 
      dlib::set_subm(hM, dlib::range(0,2), dlib::range(3,3)) = this->pt;
      hM(3,0) = 0.; hM(3,1) = 0.; hM(3,2) = 0.; hM(3,3) = 1.;
}

void Cpose3d::getLookAt(lookAtValues & lav)
{
      dlib::matrix<double,3,3> rMat;
      
      //gets this rotation matrix
      this->rt.getMatrix(rMat);
      
      //eye point
      lav.ex = this->pt(0); 
      lav.ey = this->pt(1);
      lav.ez = this->pt(2);
      
      //at point
      lav.ax = lav.ex + rMat(0,0)*100;
      lav.ay = lav.ey + rMat(1,0)*100;
      lav.az = lav.ez + rMat(2,0)*100;
      
      //up vector
      lav.ux = rMat(0,2);
      lav.uy = rMat(1,2);
      lav.uz = rMat(2,2);
}

// void Cpose3d::getPtFwLf(double & vv[9])
// {
//       dlib::matrix<double,3,3> rMat;
//       
//       //gets this rotation matrix
//       this->rt.getMatrix(rMat);
// 
//       //sers point
//       vv[0] = this->pt(0);
//       vv[1] = this->pt(1);
//       vv[2] = this->pt(2);
//       
//       //sets forward vector
//       vv[3] = rMat(0,0);
//       vv[4] = rMat(1,0);
//       vv[5] = rMat(2,0);
//       
//       //sets left vector
//       vv[6] = rMat(0,1);
//       vv[7] = rMat(1,1);
//       vv[8] = rMat(2,1);
// }

void Cpose3d::moveForward(double dd)
{
      dlib::matrix<double,3,3> rMat;
      
      //gets this rotation matrix
      this->rt.getMatrix(rMat);

      //moves point according forward axis (first column)
      this->pt(0) += rMat(0,0)*dd;
      this->pt(1) += rMat(1,0)*dd;
      this->pt(1) += rMat(2,0)*dd;
}

void Cpose3d::trf(dlib::matrix<double,3,1> & aa, const bool targetFrame)
{
      dlib::matrix<double,4,1> aIn,aOut;
      dlib::matrix<double,4,4> HH;

      //gets homogeneous matrix of this pose
      this->getHmat(HH);
      
      //sets homogeneous vectors
	aIn = aa(0), aa(1), aa(2), 1;
	if ( targetFrame == FROM_THIS_TO_REF ) aOut = HH*aIn;
	if ( targetFrame == FROM_REF_TO_THIS ) aOut = dlib::inv(HH)*aIn;
      aa = aOut(0),aOut(1),aOut(2);
}

void Cpose3d::trf(Cpose3d & po, const bool targetFrame)
{
      this->trf(po,po,targetFrame);
}

void Cpose3d::trf(Cpose3d & poIn, Cpose3d & poOut, const bool targetFrame)
{
      dlib::matrix<double,4,4> HH, Hin, Hout;

      //gets homogeneous matrix of this pose and po pose
      this->getHmat(HH);
      poIn.getHmat(Hin);

      //transforms through matrix product
      if ( targetFrame == FROM_THIS_TO_REF ) Hout = HH*Hin;
      if ( targetFrame == FROM_REF_TO_THIS ) Hout = dlib::inv(HH)*Hin;

      //sets resulting matrix
      poOut.setPose(Hout);
}

void Cpose3d::twist(const double deltaH, const double deltaP, const double deltaR)
{
      this->rt.setEuler(this->rt.head()+deltaH, this->rt.pitch()+deltaP, this->rt.roll()+deltaR);
}

void Cpose3d::printPose(bool rd)
{
      dlib::matrix<double,3,3> rMat;
      dlib::matrix<double,4,1> quat;
      
      //gets quat and rMat
      this->rt.getMatrix(rMat);
      this->rt.getQuaternion(quat);
      
      //Prints pose in a single row as: x y z h p r qr qi qj qk m00 m01 m02 m10 ..... m22      
	std::cout   << pt(0) << " " << pt(1) << " " << pt(2) << " "
                  << rt.head(rd) << " " << rt.pitch(rd) << " " << rt.roll(rd) << " "
                  << quat(0) << " " << quat(1) << " " << quat(2) << " " << quat(3) << " "
                  << rMat(0,0) << " " << rMat(0,1) << " " << rMat(0,2) << " " 
                  << rMat(1,0) << " " << rMat(1,1) << " " << rMat(1,2) << " " 
                  << rMat(2,0) << " " << rMat(2,1) << " " << rMat(2,2) << std::endl;
}

void Cpose3d::operator=(Cpose3d & pose)
{
	setPose(pose);
}
