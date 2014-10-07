#include "rotation.h"

Crotation::Crotation()
{
      ea[0] = 0; ea[1] = 0; ea[2] = 0; 
      rM = dlib::identity_matrix(dlib::matrix<double>(3,3));
      qt.qq(0) = 1; qt.qq(1) = 0; qt.qq(2) = 0; qt.qq(3) = 0;
      rStatus = UPDATED;
}

Crotation::Crotation(const double hh, const double pp, const double rr, const bool rd)
{
      this->setEuler(hh,pp,rr,rd);
}

Crotation::Crotation(const dlib::matrix<double,3,3> & mat)
{
      this->setMatrix(mat);
}

Crotation::Crotation(const Cquaternion & quat)
{
      this->setQuaternion(quat);
}

Crotation::~Crotation()
{
      //
}

void Crotation::setEuler(const double hh, const double pp, const double rr, const bool rd)
{
      //sets angles
      setEulerWithoutStatusUpdate(hh, pp, rr, rd);
      
      //update status
      rStatus = OLD_MATRIX | OLD_QUATERNION;
}

void Crotation::setMatrix(const dlib::matrix<double,3,3> & mat)
{
      rM = mat;
      rStatus = OLD_EULER | OLD_QUATERNION;      
}

void Crotation::setQuaternion(const Cquaternion & quat)
{
      qt = quat;
      rStatus = OLD_EULER | OLD_MATRIX;      
}

void Crotation::setQuaternion(const dlib::matrix<double,3,1> & qijk)
{
      qt.set(qijk);
      rStatus = OLD_EULER | OLD_MATRIX;      
}

double Crotation::head(bool rd)
{
      if ( rStatus & OLD_EULER ) updateEuler();
      return ea[0].getAngle(rd);
}

double Crotation::pitch(bool rd)
{
      if ( rStatus & OLD_EULER ) updateEuler();
      return ea[1].getAngle(rd);
}

double Crotation::roll(bool rd)
{
      if ( rStatus & OLD_EULER ) updateEuler();
      return ea[2].getAngle(rd);
}

void Crotation::getMatrix(dlib::matrix<double,3,3> & mat)
{
      if ( rStatus & OLD_MATRIX ) updateMatrix();
      mat = this->rM;
}

void Crotation::getQuaternion(Cquaternion & quat)
{
      if ( rStatus & OLD_QUATERNION ) updateQuaternion();
      quat = this->qt;
}

void Crotation::getQuaternion(dlib::matrix<double,3,1> & quat)
{
      if ( rStatus & OLD_QUATERNION ) updateQuaternion();
      quat = this->qt.qq(1),this->qt.qq(2),this->qt.qq(3);//return only the imaginary part
      //quat = dlib::subm(this->qt.qq,range(1,3),range(0,0));//return only the imaginary part
}

void Crotation::getQuaternion(dlib::matrix<double,4,1> & quat)
{
      if ( rStatus & OLD_QUATERNION ) updateQuaternion();
      quat = this->qt.qq;
}

void Crotation::getAxisAngle(dlib::matrix<double,3,1> &axis, double & angle)
{
      if ( rStatus & OLD_QUATERNION ) updateQuaternion();
      qt.getAxisAngle(axis,angle);
}

void Crotation::rotateUaxis(const double alpha, double ux, double uy, double uz, bool rd)
{
      dlib::matrix<double,3,3> P, Q, I, R;
      dlib::matrix<double,3,1> uu;
      double ar, aux; //angle in radians

      //check if matrix is updated and rad/deg option
      if ( rStatus & OLD_MATRIX ) updateMatrix();
      
      //set angle in radians whatever the input choice
      if ( rd == inRADIANS ) ar = alpha;
      else ar=alpha*M_PI/180.0;//inDEGREES
      
      //updates rM matrix with Rodrigue's formula 
      aux = sqrt(ux*ux+uy*uy+uz*uz);
      uu = ux/aux, uy/aux, uz/aux;
      P = uu*trans(uu);
      //Q = 0, -uz, uy, uz, 0, -ux, -uy, ux, 0;
      Q = 0, -uu(2), uu(1), uu(2), 0, -uu(0), -uu(1), uu(0), 0;
      I = dlib::identity_matrix(dlib::matrix<double>(3,3));
      R=P+(I-P)*cos(ar)+Q*sin(ar);
      rM=R*rM;
      
      //updates new status
      rStatus = OLD_EULER | OLD_QUATERNION;
}

void Crotation::turnHeading(const double alpha, bool rd)
{
      if ( rStatus & OLD_MATRIX ) updateMatrix();
      rotateUaxis(alpha, rM(0,2), rM(1,2), rM(2,2), rd);
}

void Crotation::turnPitch(const double alpha, bool rd)
{
      if ( rStatus & OLD_MATRIX ) updateMatrix();
      rotateUaxis(alpha, rM(0,1), rM(1,1), rM(2,1), rd);    
}

void Crotation::print(const bool rd)
{
      //update things
      if ( rStatus & OLD_EULER ) updateEuler();
      if ( rStatus & OLD_MATRIX ) updateMatrix();
      if ( rStatus & OLD_QUATERNION ) updateQuaternion();
      
      //print them !
      std::cout << "euler(hpr): [" << ea[0].getAngle() << "," <<  ea[1].getAngle() << "," << ea[2].getAngle() << "]" << std::endl; 
      std::cout << "matrix: [" <<  rM(0,0) << "," << rM(0,1) << "," << rM(0,2) 
                              << " | " << rM(1,0) << "," << rM(1,1) << "," << rM(1,2)     
                              << " | " << rM(2,0) << "," << rM(2,1) << "," << rM(2,2) << "]" << std::endl;
      std::cout << "quaternion: [" << qt.qq(0) << "," << qt.qq(1) << "," << qt.qq(2) << "," << qt.qq(3) << "]" << std::endl; 
}

void Crotation::operator=(Crotation & rt)
{
      dlib::matrix<double,3,3> mat;
      rt.getMatrix(mat);
      this->setMatrix(mat);
      this->updateEuler();
      this->updateQuaternion();
}

void Crotation::setEulerWithoutStatusUpdate(const double hh, const double pp, const double rr, const bool rd)
{
      ea[0].setAngle(hh,rd);
      ea[1].setAngle(pp,rd);
      ea[2].setAngle(rr,rd);      
}

void Crotation::updateEuler()
{
      dlib::matrix<double,3,1> lny, lnz, ys;
      double aux; 
      double hh, pp, rr;

      //first check if matrix is updated
      if ( rStatus & OLD_MATRIX ) updateMatrix();
      
      //check for gimbal lock situation (forward axis is pointing downwards or upwards)
      if ( fabs(this->rM(2,0)-1) < EPSILON ) //forward axis pointing upwards
      {
            hh = this->ea[0].getAngle(); //keeps the old heading value
            pp = -M_PI/2;//pitch
            rr = atan2(-this->rM(0,1),this->rM(1,1))-hh;//left axis at xy plane marks roll
      }           
      else if ( fabs(this->rM(2,0)+1) < EPSILON ) //forward axis pointing downwards
      {
            hh = this->ea[0].getAngle(); //keeps the old heading value
            pp = M_PI/2;//pitch
            rr = -atan2(-this->rM(0,1),this->rM(1,1))-hh;//left axis at xy plane marks roll
      }
      else
      {
            hh = atan2( rM(1,0) , rM(0,0) );//heading
            aux = rM(0,0)*rM(0,0)+rM(1,0)*rM(1,0);
            pp = -atan2( rM(2,0) , sqrt(aux) );//pitch
            rr = atan2( rM(2,1)*aux-rM(2,0)*(rM(0,0)*rM(0,1)+rM(1,0)*rM(1,1)) , rM(0,0)*rM(1,1)-rM(1,0)*rM(0,1) ); //roll
      }
      
      //sets angles assuring range correctness
      setEulerWithoutStatusUpdate(hh,pp,rr);            
            
      //Updates status by resetting OLD_EULER bit
      rStatus &= ( 0xFFFF & ~(OLD_EULER) );
}

void Crotation::updateMatrix()
{
      dlib::matrix<double,3,3> mH, mP, mR; //heading, pitch and roll rottaions
      unsigned short rst;
      
      //get rStatus
      rst = rStatus & (OLD_EULER | OLD_QUATERNION);
      
      switch(rst)
      {
            case OLD_EULER: //eulers are old, so we use quaternion to set matrix
                  qt.getMatrix(this->rM);
                  break;
                  
            case OLD_QUATERNION: //quaternion is old, so we use eulers to set matrix
                  mH = cos(ea[0].getAngle()), -sin(ea[0].getAngle()), 0, sin(ea[0].getAngle()), cos(ea[0].getAngle()), 0, 0, 0, 1;
                  mP = cos(ea[1].getAngle()), 0, sin(ea[1].getAngle()), 0, 1, 0, -sin(ea[1].getAngle()), 0, cos(ea[1].getAngle());
                  mR = 1, 0, 0, 0, cos(ea[2].getAngle()), -sin(ea[2].getAngle()), 0, sin(ea[2].getAngle()), cos(ea[2].getAngle());
                  rM=mH*mP*mR; //rM is the composition of these three rotations
                  break;

            default: //eulers and quaternion are old, so matrix was ok ! Nothing to do!
                  break;
      }
                  
      //updates rotation status
      rStatus &= ( 0xFFFF & ~(OLD_MATRIX) );//resets OLD_MATRIX bit
}

void Crotation::updateQuaternion()
{
      //first check if matrix is updated
      if ( rStatus & OLD_MATRIX ) updateMatrix();
      
      //update quaternion from matrix
      this->qt.set(this->rM);
      
      //updates status
      rStatus &= ( 0xFFFF & ~(OLD_QUATERNION) );//resets OLD_MATRIX bit
}
