#include "position3d.h"

Cposition3d::Cposition3d()
{
	xx=0; yy=0; zz=0; 
	hh.setAngle(0); pp.setAngle(0); rr.setAngle(0);
	pStatus = OLDMATRIX;
}

Cposition3d::Cposition3d(double cx, double cy, double cz, double ah, double ap, double ar, bool rd)
{
	xx=cx; yy=cy; zz=cz;
	hh.setAngle(ah,rd); pp.setAngle(ap,rd); rr.setAngle(ar,rd);
	pStatus = OLDMATRIX;
}

Cposition3d::~Cposition3d()
{
}

void Cposition3d::setH(double ah, bool rd){hh.setAngle(ah,rd);pStatus = OLDMATRIX;} 

void Cposition3d::setP(double ap, bool rd){pp.setAngle(ap,rd);pStatus = OLDMATRIX;} 

void Cposition3d::setR(double ar, bool rd){rr.setAngle(ar,rd);pStatus = OLDMATRIX;} 

void Cposition3d::setHPR(double ah, double ap, double ar, bool rd)
{	
	hh.setAngle(ah,rd); pp.setAngle(ap,rd); rr.setAngle(ar,rd);
	pStatus = OLDMATRIX;
}

void Cposition3d::setFullPose(double cx, double cy, double cz, double ah, double ap, double ar, bool rd)
{
	xx=cx; yy=cy; zz=cz;
	hh.setAngle(ah,rd); pp.setAngle(ap,rd); rr.setAngle(ar,rd);
	pStatus = OLDMATRIX;
}

void Cposition3d::setFullPose(Cposition3d *pose)
{
	xx=pose->getX(); yy=pose->getY(); zz=pose->getZ();
	hh.setAngle(pose->getH()); pp.setAngle(pose->getP()); rr.setAngle(pose->getR());
	pStatus = OLDMATRIX;
}

void Cposition3d::setFullPose(Cposition3d & pose)
{
	xx=pose.getX(); yy=pose.getY(); zz=pose.getZ();
	hh.setAngle(pose.getH()); pp.setAngle(pose.getP()); rr.setAngle(pose.getR());
	pStatus = OLDMATRIX;
}

void Cposition3d::setFullPoseByMatrix(Cposition3d *pose)
{
	Matrix4f A;
	pose->getHomogeneous(&A);
	this->setHomogeneous(&A);
}

void Cposition3d::setFullPoseByMatrix(Cposition3d & pose)
{
	Matrix4f A;
	pose.getHomogeneous(&A);
	this->setHomogeneous(&A);
}

void Cposition3d::setFullPose(Vector3f &xyzV, Vector3f &fwV, Vector3f &lfV)
{
	xx=xyzV(0); yy=xyzV(1); zz=xyzV(2);
	mR.block<3,1>(0,0) = fwV;
	mR.block<3,1>(0,1) = lfV;
	mR.block<3,1>(0,2) = fwV.cross(lfV); //up = fw x lft
	pStatus = OLDANGLES;
}

void Cposition3d::setFullPose(double cx, double cy, double cz, double fwx, double fwy, double fwz, double lfx, double lfy, double lfz)
{
	xx=cx; yy=cy; zz=cz;
	mR << fwx, lfx, 0, fwy, lfy, 0, fwz, lfz, 0;
	mR.block<3,1>(0,2) = mR.block<3,1>(0,0).cross(mR.block<3,1>(0,1));
	pStatus = OLDANGLES;
}

void Cposition3d::setHomogeneous(Matrix4f *H)
{
	mR=H->block<3,3>(0,0);
	xx=(*H)(0,3); yy=(*H)(1,3); zz=(*H)(2,3);
	pStatus = OLDANGLES;
}

void Cposition3d::setQuaternion(const double qReal, const double qi, const double qj, const double qk)
{
	//see http://en.wikipedia.org/wiki/Rotation_representation_%28mathematics%29
	
	Matrix3f QQ, II, qqT;
	Vector3f qq;
	double qTq;
	
	qq << qi, qj, qk;
	QQ << 0, -qk, qj, qk, 0, -qi, -qj, qi, 0;
	qqT = qq*qq.transpose();
	//qTq = qq.transpose()*qq;
	qTq = qq(0)*qq(0)+qq(1)*qq(1)+qq(2)*qq(2);
	II = Matrix3f::Identity();
	mR = (qReal*qReal - qTq)*II + 2*qqT + 2*qReal*QQ;
	
	pStatus = OLDANGLES;
}

double Cposition3d::getH(bool rd)
{
	if (pStatus==OLDANGLES) updateEulerAngles();
	return hh.getAngle(rd);
} 

double Cposition3d::getP(bool rd)
{
	if (pStatus==OLDANGLES) updateEulerAngles();
	return pp.getAngle(rd);
}

double Cposition3d::getR(bool rd)
{
	if (pStatus==OLDANGLES) updateEulerAngles();
	return rr.getAngle(rd);
} 

void Cposition3d::getHomogeneous(Matrix4f *H)
{
	if ( pStatus == OLDMATRIX ) updateRmatrix();
	H->block<3,3>(0,0)=mR;
	H->block<3,1>(0,3) << xx, yy, zz;
	H->block<1,4>(3,0) << 0, 0, 0, 1;
}

void Cposition3d::getLookAtValues(lookAtValues *lav)
{
	if ( pStatus == OLDMATRIX ) updateRmatrix();
	lav->ex = xx; 
	lav->ey = yy;
	lav->ez = zz;
 	lav->ax = xx + mR(0,0)*100;
	lav->ay = yy + mR(1,0)*100;
	lav->az = zz + mR(2,0)*100;
	lav->ux = mR(0,2);
	lav->uy = mR(1,2);
	lav->uz = mR(2,2);
}

void Cposition3d::getXYZFwLf(double *vv)
{
	if ( pStatus == OLDMATRIX ) updateRmatrix();
	vv[0] = xx;
	vv[1] = yy;
	vv[2] = zz;
	vv[3] = mR(0,0);
	vv[4] = mR(1,0);
	vv[5] = mR(2,0);
	vv[6] = mR(0,1);
	vv[7] = mR(1,1);
	vv[8] = mR(2,1);
}

void Cposition3d::getQuaternion(double &qReal, double &qi, double &qj, double &qk)
{
	// see : http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/index.htm
	
	double trace, ss;
	
	if ( pStatus == OLDMATRIX ) updateRmatrix();
	
	trace = mR(0,0) + mR(1,1) + mR(2,2);
	
	if( trace > 0 ) 
	{
		ss = 0.5 / sqrt(trace+1.0f);
		qReal = 0.25f / ss;
		qi = (mR(2,1)-mR(1,2)) * ss;
		qj = (mR(0,2)-mR(2,0)) * ss;
		qk = (mR(1,0)-mR(0,1)) * ss;
	} 
	else
	{
		if ( mR(0,0) > mR(1,1) && mR(0,0) > mR(2,2) )
		{
			ss = 2.0 * sqrt( 1.0 + mR(0,0) - mR(1,1) - mR(2,2));
			qReal = (mR(2,1) - mR(1,2) ) / ss;
			qi = 0.25 * ss;
			qj = (mR(0,1) + mR(1,0) ) / ss;
			qk = (mR(0,2) + mR(2,0) ) / ss;
		} 
		else if (mR(1,1) > mR(2,2))
		{
			ss = 2.0 * sqrt( 1.0 + mR(1,1) - mR(0,0) - mR(2,2));
			qReal = (mR(0,2) - mR(2,0) ) / ss;
			qi = (mR(0,1) + mR(1,0) ) / ss;
			qj = 0.25 * ss;
			qk = (mR(1,2) + mR(2,1) ) / ss;
		} 
		else
		{
			ss = 2.0 * sqrt( 1.0 + mR(2,2) - mR(0,0) - mR(1,1) );
			qReal = (mR(1,0) - mR(0,1) ) / ss;
			qi = (mR(0,2) + mR(2,0) ) / ss;
			qj = (mR(1,2) + mR(2,1) ) / ss;
			qk = 0.25 * ss;
		}
	}
}

void Cposition3d::moveForward(double dd)
{
 	if ( pStatus == OLDMATRIX ) updateRmatrix();
 	xx=xx+mR(0,0)*dd;	
 	yy=yy+mR(1,0)*dd;
 	zz=zz+mR(2,0)*dd;
}

void Cposition3d::rotateUaxis(double alpha, double ux, double uy, double uz, bool rd)
{
	Matrix3f P, Q, I, R;
	Vector3f uu;

 	if ( pStatus == OLDMATRIX ) updateRmatrix();
	if ( rd == inDEGREES ) alpha=alpha*M_PI/180.0;
	
	//updates mR matrix with Rodrigue's formula 
	uu << ux, uy, uz;
	P = uu*uu.transpose();
	Q << 0, -uz, uy, uz, 0, -ux, -uy, ux, 0;
	I = Matrix3f::Identity();
	R=P+(I-P)*cos(alpha)+Q*sin(alpha);
	mR=R*mR;
	//pStatus = OLDANGLES;
	updateEulerAngles();
}

void Cposition3d::turnZaxis(double alpha, bool rd)
{
	if ( pStatus == OLDMATRIX ) updateRmatrix();
	rotateUaxis(alpha, mR(0,2), mR(1,2), mR(2,2), rd);
}

void Cposition3d::coordTr(Cpoint3d *qq, bool targetFrame)
{
	Vector4f qIn,qOut;
	Matrix4f H;

	getHomogeneous(&H);
	qIn << qq->getX(), qq->getY(), qq->getZ(), 1;
	if (targetFrame==TOREF)	qOut = H*qIn;
	if (targetFrame==TOTHIS) qOut = H.inverse()*qIn;
	qq->setXYZ(qOut(0), qOut(1), qOut(2));
}

void Cposition3d::coordTr(Cposition3d *ff, bool targetFrame)
{
	Matrix4f H;
	Matrix4f Hin, Hout;

	getHomogeneous(&H);
	ff->getHomogeneous(&Hin);
	if (targetFrame==TOREF)	Hout = H*Hin;
	if (targetFrame==TOTHIS) Hout = H.inverse()*Hin;
	ff->setHomogeneous(&Hout);
}

void Cposition3d::coordTr(Cposition3d *ffIn, Cposition3d *ffOut, bool targetFrame)
{
	Matrix4f H;
	Matrix4f Hin, Hout;

	getHomogeneous(&H);
	ffIn->getHomogeneous(&Hin);
	if (targetFrame==TOREF)	Hout = H*Hin;
	if (targetFrame==TOTHIS) Hout = H.inverse()*Hin;
	ffOut->setHomogeneous(&Hout);
}

void Cposition3d::twist(double deltaH, double deltaP, double deltaR)
{
	if ( pStatus == OLDANGLES ) updateEulerAngles();
	hh += deltaH;
	pp += deltaP;
	rr += deltaR;
	pStatus = OLDMATRIX;
}

// void Cposition3d::incTheta(const double & deltaThRads)
// {
// 	hh+=deltaThRads;
// 	pStatus = OLDANGLES;
// }
// 
// double Cposition3d::deltaTheta(Cposition3d & posB)
// {
// 	Cangle delta;
// 	delta.setAngle(posB.getH()-this->getH());
// 	return delta.getAngle();
// }

void Cposition3d::printPosition(bool rd)
{
	if ( pStatus == OLDANGLES ) updateEulerAngles();
	cout << "(" << xx << "," << yy << "," << zz << "," << hh.getAngle(rd) << "," << pp.getAngle(rd) << "," << rr.getAngle(rd) << ")" << endl;
}

void Cposition3d::printAttitude(bool rd)
{
	if ( pStatus == OLDANGLES ) updateEulerAngles();
	cout << "h=" << hh.getAngle(rd) << "; p=" << pp.getAngle(rd) << "; r=" << rr.getAngle(rd) << endl;
}

void Cposition3d::printRmatrix()
{
	if ( pStatus == OLDMATRIX ) updateRmatrix();
	cout << "mR:\n" << mR << endl;
}

void Cposition3d::operator=(Cposition3d & pose)
{
	setFullPose(pose);
}

void Cposition3d::updateRmatrix()
{
	Matrix3f mhh, mpp, mrr;

	mhh << cos(hh.getAngle()), -sin(hh.getAngle()), 0, sin(hh.getAngle()), cos(hh.getAngle()), 0, 0, 0, 1;
	mpp << cos(pp.getAngle()), 0, sin(pp.getAngle()), 0, 1, 0, -sin(pp.getAngle()), 0, cos(pp.getAngle());
	mrr << 1, 0, 0, 0, cos(rr.getAngle()), -sin(rr.getAngle()), 0, sin(rr.getAngle()), cos(rr.getAngle());
	
	mR=mhh*mpp*mrr;
	pStatus = UPDATED;
	//cout << "updateRmatrix()" << endl << mR << endl;
}

void Cposition3d::updateEulerAngles()
{
	Vector3f lny, lnz, ys;
	double aux;	

	hh = atan2( mR(1,0) , mR(0,0) );//atan(xs_y/xs_x)
	aux = mR(0,0)*mR(0,0)+mR(1,0)*mR(1,0);
	pp = -atan2( mR(2,0) , aux/sqrt(aux) );//
	rr = atan2( mR(2,1)*aux-mR(2,0)*(mR(0,0)*mR(0,1)+mR(1,0)*mR(1,1)) , mR(0,0)*mR(1,1)-mR(1,0)*mR(0,1) );
	pStatus = UPDATED;
}
