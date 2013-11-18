#include "angle.h"

Cangle::Cangle():angle(0)
{
}

Cangle::Cangle(double value, const bool rd)
{
	setAngle(value,rd);
}

Cangle::~ Cangle()
{
}

void Cangle::setAngle(const double value, const bool rd)
{
	if ( rd == inRADIANS ) angle=value;
	else angle=value*M_PI/180;
	mPi2Pi();
}

double Cangle::getAngle(const bool rd) const
{
	if ( rd == inRADIANS ) return angle;
	else return (angle*180.0/M_PI);
}

void Cangle::wMeanAngle(const double a1, const double w1, const double a2, const double w2, const bool rd)
{
	double ss=0,cc=0;
	
	if ( rd == inRADIANS ){
		ss=(w1*sin(a1)+w2*sin(a2))/(w1+w2);
		cc=(w1*cos(a1)+w2*cos(a2))/(w1+w2);}
	else {
		ss=(w1*sin(a1*M_PI/180.0)+w2*sin(a2*M_PI/180.0))/(w1+w2);
		cc=(w1*cos(a1*M_PI/180.0)+w2*cos(a2*M_PI/180.0))/(w1+w2);}

	angle=atan2(ss,cc);//already in (-pi,pi]
	//std::cout << "w1=" << w1 << "; w2=" << w2 << "; ss=" << ss << "; cc=" << cc << "; angle=" << angle << std::endl;
}

bool Cangle::operator==(const Cangle beta)
{
	if (this->angle == beta.getAngle()){return true;}
	else {return false;}
}

void Cangle::operator=(const double beta)
{
      this->setAngle(beta);
}

void Cangle::operator=(const Cangle beta)
{
	this->setAngle(beta.getAngle());
}

void Cangle::operator+=(const Cangle beta)
{
	this->setAngle(this->getAngle()+beta.getAngle());
}

void Cangle::operator-=(const Cangle beta)
{
	this->setAngle(this->getAngle()-beta.getAngle());
}

Cangle Cangle::operator+(const Cangle beta) const
{
	Cangle tmp;
	tmp.setAngle(this->getAngle()+beta.getAngle());
	return tmp;
}

Cangle Cangle::operator-(const Cangle beta) const 
{
	Cangle tmp;
	tmp.setAngle(this->getAngle()-beta.getAngle());
	return tmp;
}

void Cangle::mPi2Pi()
{
  	angle = angle-2*M_PI*round((angle)/(2*M_PI));
	if (angle==-M_PI){angle=M_PI;} //special case to keep the interval as (-pi,pi]
}	
