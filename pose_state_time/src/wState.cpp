#include "wState.h"

CwState::CwState()
{

}

CwState::~CwState()
{

}

void CwState::set(const dlib::matrix<double,VQ_STATE_DIMENSION*WINDOW_STATE_SIZE,1> & wsVec)
// void CwState::set(const dlib::matrix<double> & wsVec)
{
	unsigned int ii=0;
	unsigned int jj=0;
	
        //ws.resize(WINDOW_STATE_SIZE);//!!
	for(ii=0; ii<WINDOW_STATE_SIZE; ii++)
	{
		this->ws.at(ii).vv = wsVec(jj),wsVec(jj+1),wsVec(jj+2);
		this->ws.at(ii).qt.set(wsVec(jj+3),wsVec(jj+4),wsVec(jj+5));
		jj += VQ_STATE_DIMENSION;
	}
}

void CwState::getVector(dlib::matrix<double,VQ_STATE_DIMENSION*WINDOW_STATE_SIZE,1> & wsVec) const
// void CwState::getVector(dlib::matrix<double> & wsVec) const
{
	dlib::matrix<double,VQ_STATE_DIMENSION,1> pstv; 	
	unsigned int ii;
		
	for (ii=0; ii<WINDOW_STATE_SIZE; ii++)
	{
		this->ws.at(ii).getVector(pstv);//pstv is now a VQ_STATE_DIMENSION-vector of vx,vy,vz,q1,q2,q3
		dlib::set_colm( wsVec , dlib::range( (ii*VQ_STATE_DIMENSION) , ((ii+1)*VQ_STATE_DIMENSION-1) ) ) = pstv;
	}
}

bool CwState::tsInWsPeriod(const CtimeStamp & t0, const CtimeStamp & te) const
{
      if ( (te <= t0) || (t0 < ws.front().ts) || (ws.back().ts < te) ) return false;
      else return true;
}

void CwState::getTransform(const CtimeStamp & t0, const CtimeStamp & te, Cpose3d & pose) const
{
      unsigned int ii,ii0,iie;
      CwState winSt;
      CvqStateTs vqSt;
      double dt1, dt2, aux, dT;
      dlib::matrix<double,3,1> trv;
      dlib::matrix<double,3,3> rM, rMinv;
      
      //find ws indexes corresponding to time stamps t0 and te
      ii0 = 0;
      while(ws[ii0].ts < t0) ii0++;
      iie = ii0;
      while(ws[iie].ts <= te) iie++;
      
      //1.set winSt with vqStates inside [t0,te]. 
      
      //1a. First element is approximated as linear interpolation.
      dt1 = ws[ii0].ts-ws[ii0-1].ts;
      dt2 = t0-ws[ii0-1].ts;
      aux = dt2/dt1;
      vqSt.vv = (1-aux)*ws[ii0-1].vv + aux*ws[ii0].vv;
      vqSt.qt.qq = (1-aux)*ws[ii0-1].qt.qq + aux*ws[ii0].qt.qq;
      vqSt.qt.normalize();
      vqSt.ts = t0;
      winSt.ws.push_back(vqSt);
      
      //1b. Middle elements are copied
      for (ii=ii0; ii<iie; ii++) winSt.ws.push_back(this->ws[ii]);
      
      //1c. last element is approximated as linear interpolation.
      dt1 = ws[iie].ts-ws[iie-1].ts;
      dt2 = te-ws[iie-1].ts;
      aux = dt2/dt1;
      vqSt.vv = (1-aux)*ws[iie-1].vv + aux*ws[iie].vv;
      vqSt.qt.qq = (1-aux)*ws[iie-1].qt.qq + aux*ws[iie].qt.qq;
      vqSt.qt.normalize();
      vqSt.ts = te;
      winSt.ws.push_back(vqSt);
      
      //debug: print winSt
      //winSt.print();
      
      //2. Compute transform
      
      //2a. translation part
      trv = dlib::zeros_matrix<double>(3,1);
      for (ii=0; ii<winSt.ws.size()-1; ii++)
      {
            dT = winSt.ws[ii+1].ts - winSt.ws[ii].ts;
            trv += winSt.ws[ii].vv*dT;
      }
      
      //2b. Here, trv is in terms of reference. Rotate to be in terms of wState(t0)
      ws.at(0).qt.getMatrix(rM); //rotation matrix at t0
      rMinv = dlib::inv(rM);
      pose.pt = rMinv*trv;
      
      //2c. rotation part
      ws.back().qt.getMatrix(rM); //rotation matrix at te
      pose.rt.setMatrix(rMinv*rM);
      
}

void CwState::print() const
{
	unsigned int ii;
	
	for (ii=0; ii<ws.size(); ii++)
	{
		ws.at(ii).ts.print();
		std::cout << " , (" 
			<< ws.at(ii).vv(0) << " , "
			<< ws.at(ii).vv(1) << " , "
			<< ws.at(ii).vv(2) << " , "
			<< ws.at(ii).qt.qq(0) << " , "
			<< ws.at(ii).qt.qq(1) << " , "
			<< ws.at(ii).qt.qq(2) << " , "			
			<< ws.at(ii).qt.qq(3) << ")" 
			<< std::endl;
	}
	std::cout << std::endl;		
}