#include "timeStamp.h"

CtimeStamp::CtimeStamp()
{
	timeStamp = 0;
}

CtimeStamp::~CtimeStamp()
{
	//nothing to do
}

void CtimeStamp::setToNow()
{
	timeval ts;
	gettimeofday(&ts, NULL); 
	timeStamp = (double)(ts.tv_sec + ts.tv_usec/1e6);	
}

void CtimeStamp::set(timeval ts)
{
	timeStamp = (double)(ts.tv_sec + ts.tv_usec/1e6);	
}

void CtimeStamp::set(const unsigned long int sec, const unsigned long int nanosec)
{
	timeStamp = (double)(sec + nanosec/1e9);	
}

void CtimeStamp::set(const double ts)
{
	timeStamp = ts;
}

double CtimeStamp::get() const
{
	return timeStamp;
}

unsigned long int CtimeStamp::getSeconds() const
{
	unsigned long int ts;
	ts = (unsigned long int)floor(timeStamp);
	return ts;
}

unsigned long int CtimeStamp::getNanoSeconds() const 
{
	double ts;
	ts = floor(timeStamp);
	return (unsigned long int)((timeStamp-ts)*1e9);
}

void CtimeStamp::print(std::ostream & ost) const
{
        std::streamsize nn;
        //std::streamsize ww;
        std::ios_base::fmtflags fmtfl;
        //char cc; 
        
        fmtfl = ost.flags(std::ios::left);
        ost.setf(std::ios::fixed,std::ios::floatfield);
        //cc = ost.fill('0');
        nn = ost.precision(TIME_STAMP_DIGITS);
        //ww = ost.width(TIME_STAMP_DIGITS);
        
        ost << this->timeStamp;
        
        ost.flags(fmtfl);
        //ost.fill(cc);
        ost.precision(nn);
        //ost.width(ww);
}

void CtimeStamp::operator=(const double ts)
{
        this->timeStamp = ts;
}

void CtimeStamp::operator=(const CtimeStamp ts)
{
        this->timeStamp = ts.get();
}

bool CtimeStamp::operator<(const CtimeStamp ts) const
{
        if ( this->timeStamp < ts.get() ) return true;
        else return false;
}

bool CtimeStamp::operator<=(const CtimeStamp ts) const
{
        if ( this->timeStamp <= ts.get() ) return true;
        else return false;
}

double CtimeStamp::operator-(const CtimeStamp ts) const
{
      return ( this->timeStamp - ts.get() );      
}
