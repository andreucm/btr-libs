#include "vqState.h"

CvqState::CvqState()
{
	vv = dlib::zeros_matrix<double>(3,1);
	qt.qq(0) = 1;
}

CvqState::CvqState(const dlib::matrix<double,3,1> vel, const Cquaternion & quat)
{
	vv = vel;
	qt = quat;
}

CvqState::CvqState(const double vx, const double vy, const double vz, const double q1, const double q2, const double q3)
{
        this->set(vx,vy,vz,q1,q2,q3);
}


CvqState::~CvqState()
{
	//nothing to do
}

void CvqState::set(const double vx, const double vy, const double vz, const double q1, const double q2, const double q3)
{
	vv = vx,vy,vz;
	qt.set(q1,q2,q3);
}

void CvqState::getVector(dlib::matrix<double,VQ_STATE_DIMENSION,1> & vq) const
{
        vq = vv(0),vv(1),vv(2),qt(1),qt(2),qt(3);
}

double CvqState::mahalanobisDistance(const CvqState & vq, const dlib::matrix<double,VQ_STATE_DIMENSION,VQ_STATE_DIMENSION> & covM) const
{
	dlib::matrix<double,6,1> s1;
	dlib::matrix<double,6,1> s2;
	dlib::matrix<double,6,1> dd; 
	
	this->getVector(s1);
	vq.getVector(s2);
	dd = s1-s2;
	return trans(dd)*dlib::inv(covM)*dd;
}

dlib::matrix<double,3,1> CvqState::bodyVelocities() const
{
	dlib::matrix<double,3,3> RR;
	dlib::matrix<double,3,1> bV;
	
	qt.getMatrix(RR);
	bV = dlib::trans(RR)*vv;
	
	return bV;
}

void CvqState::print() const 
{
	std::cout << "vv: " << dlib::trans(vv);
	std::cout << "qq: " << dlib::trans(qt.qq);
}

void CvqState::operator=(const CvqState & vq)
{
	this->vv = vq.vv;
	this->qt = vq.qt;
}

void CvqState::operator=(const dlib::matrix<double,6,1> & vq)
{
	this->vv = vq(0),vq(1),vq(2);
	this->qt.set(vq(3),vq(4),vq(5));	
}