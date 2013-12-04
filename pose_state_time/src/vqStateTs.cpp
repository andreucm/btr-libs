#include "vqStateTs.h"

CvqStateTs::CvqStateTs() : CvqState()
{
        
}

CvqStateTs::CvqStateTs(const dlib::matrix<double,3,1> & vel, const Cquaternion & quat, const CtimeStamp & time) : CvqState(vel,quat)
{
        this->ts = time;
}

CvqStateTs::CvqStateTs(const double vx, const double vy, const double vz, const double q1, const double q2, const double q3, const double time) : CvqState(vx,vy,vz,q1,q2,q3)
{
        this->ts = time;
}

CvqStateTs::~CvqStateTs()
{
        
}

void CvqStateTs::operator=(const CvqStateTs & vqt)
{
        this->ts = vqt.ts;
        this->vv = vqt.vv;
        this->qt.qq = vqt.qt.qq;
}

void CvqStateTs::print() const
{
        std::cout << "ts: "; ts.print(); std::cout << std::endl;
        CvqState::print();
}
