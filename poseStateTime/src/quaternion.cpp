
#include "quaternion.h"

Cquaternion::Cquaternion()
{
	qq(0) = 1;
	qq(1) = 0;
	qq(2) = 0;
	qq(3) = 0;
}

Cquaternion::Cquaternion(const double q0, const double q1, const double q2, const double q3)
{
	qq(0) = q0;
	qq(1) = q1;
	qq(2) = q2;
	qq(3) = q3;
}

Cquaternion::Cquaternion(const double q1, const double q2, const double q3)
{
	this->set(q1,q2,q3);
}

Cquaternion::Cquaternion(const dlib::matrix<double,4,1> qin)
{
	qq = qin;
}

Cquaternion::Cquaternion(const dlib::matrix<double,3,1> qin)
{
	this->set(qin);
}

Cquaternion::~Cquaternion()
{
	//nothing to do
}

void Cquaternion::set(const double q0, const double q1, const double q2, const double q3)
{
      qq(0) = q0;
      qq(1) = q1;
      qq(2) = q2;
      qq(3) = q3;
}

void Cquaternion::set(const double q1, const double q2, const double q3)
{
      double quadSum;
	qq(1) = q1;
	qq(2) = q2;
	qq(3) = q3;
      quadSum = q1*q1-q2*q2-q3*q3;
      if (quadSum < 1) qq(0) = sqrt(1-quadSum);	
      else
      {
          qq(0) = 0; 
          this->normalize();
          //std::cout << "Warning! Cquaternion::set(). sqrt(<0)!!; q1*q1+q2*q2+q3*q3 = " << q1*q1+q2*q2+q3*q3 << std::endl;
      }
}

void Cquaternion::set(const dlib::matrix<double,4,1> qin)
{
      set(qin(0),qin(1),qin(2),qin(3));
}

void Cquaternion::set(const dlib::matrix<double,3,1> qin)
{
      set(qin(0),qin(1),qin(2));
}

void Cquaternion::set(const dlib::matrix<double,3,3> &rMat)
{
      double aux1, aux2;
      
      aux1 = 1 + rMat(0,0) + rMat(1,1) + rMat(2,2);
      aux2 = 1 + rMat(0,0) - rMat(1,1) - rMat(2,2);
      
      //to do : check if with this two cases all rottaions are covered ...
      if ( aux1 > aux2 )
      {
            qq(0) = 0.5*sqrt(aux1);
            qq(1) = (0.25/qq(0))*(rMat(2,1)-rMat(1,2));
            qq(2) = (0.25/qq(0))*(rMat(0,2)-rMat(2,0));
            qq(3) = (0.25/qq(0))*(rMat(1,0)-rMat(0,1));
      }
      else
      {
            qq(1) = 0.5*sqrt(aux2);
            qq(2) = (0.25/qq(1))*(rMat(0,1)+rMat(1,0));
            qq(3) = (0.25/qq(1))*(rMat(0,2)+rMat(2,0));
            qq(4) = (0.25/qq(1))*(rMat(2,1)-rMat(1,2));
      }
      
      this->normalize();
}

void Cquaternion::getMatrix(dlib::matrix<double,3,3> & rM) const
{
      rM = qq(0)*qq(0)+qq(1)*qq(1)-qq(2)*qq(2)-qq(3)*qq(3),  2*qq(1)*qq(2)-2*qq(0)*qq(3),  2*qq(1)*qq(3)+2*qq(0)*qq(2),
            2*qq(1)*qq(2)+2*qq(0)*qq(3),  qq(0)*qq(0)-qq(1)*qq(1)+qq(2)*qq(2)-qq(3)*qq(3),  2*qq(2)*qq(3)-2*qq(0)*qq(1),
            2*qq(1)*qq(3)-2*qq(0)*qq(2),  2*qq(2)*qq(3)+2*qq(0)*qq(1),  qq(0)*qq(0)-qq(1)*qq(1)-qq(2)*qq(2)+qq(3)*qq(3);      
}

dlib::matrix<double,3,3> Cquaternion::getMatrix() const
{
      dlib::matrix<double,3,3> rM;
      
      getMatrix(rM);
      return rM;
}

void Cquaternion::getAxisAngle(dlib::matrix<double,3,1> &axis, double & angle)
{
      double aux;
      this->normalize();
      aux = sqrt(qq(1)*qq(1)+qq(2)*qq(2)+qq(3)*qq(3));
      axis = qq(1),qq(2),qq(3);
      axis = axis/aux;
      angle = 2*acos(qq(0));
}

void Cquaternion::conjugate()
{
	qq(1) = -qq(1);
	qq(2) = -qq(2);
	qq(3) = -qq(3);
}

void Cquaternion::normalize()
{
	double module;
	
	module = sqrt(qq(0)*qq(0)+qq(1)*qq(1)+qq(2)*qq(2)+qq(3)*qq(3));
	qq = qq/module;
}

void Cquaternion::product(const Cquaternion & qin, Cquaternion & qRes) const
{
         qRes.qq(0) = qq(0)*qin(0)-qq(1)*qin(1)-qq(2)*qin(2)-qq(3)*qin(3);
         qRes.qq(1) = qq(0)*qin(1)+qq(1)*qin(0)+qq(2)*qin(3)-qq(3)*qin(2);
         qRes.qq(2) = qq(0)*qin(2)-qq(1)*qin(3)+qq(2)*qin(0)+qq(3)*qin(1);
         qRes.qq(3) = qq(0)*qin(3)+qq(1)*qin(2)-qq(2)*qin(1)+qq(3)*qin(0);
}

Cquaternion Cquaternion::product(const Cquaternion & qin) const
{
	Cquaternion qRes;
	
	product(qin,qRes);
	return qRes;
}

void Cquaternion::print() const
{
	std::cout << dlib::trans(qq) << std::endl;
}

double Cquaternion::operator()(unsigned int ii) const
{
	return this->qq(ii);
}

void Cquaternion::operator=(const Cquaternion & qin)
{
	this->qq(0) = qin(0);
	this->qq(1) = qin(1);
	this->qq(2) = qin(2);
	this->qq(3) = qin(3);	
      this->normalize();
}

Cquaternion Cquaternion::operator*(const Cquaternion & qin)
{
      Cquaternion qRes;
      
      product(qin,qRes);
      return qRes;      
}
