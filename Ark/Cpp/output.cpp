#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include "LBMNameSpace.h"

using std::cout;
using std::ofstream;
using std::ostringstream;
using std::string;
using std::ios;
using std::setprecision;
using PhaseFieldAC::radius;

extern double ResidualPer1k;
//
void FileOpenVerify(ofstream &OutFile,string const s)
{
	if(!OutFile)
	{
		_PRINT_ERROR_MSG_FLIP
		cout <<s<<"open failed"<<nl;
		getchar();
		exit(0);
	}
}
void OutputCase()
{
	ofstream OutFile_Case("../FlowField/Case.ark");

	FileOpenVerify(OutFile_Case,"Case.ark");
	
	OutFile_Case <<"#----------------------------------------"<<nl
				 <<ChLength<<"    =    Character Length"<<nl
				 <<Ma<<"    =    Mach"<<nl
				 <<Re<<"    =    Re"<<nl
				 <<U0<<"    =    U0"<<nl
				 <<R0<<"    =    R0"<<nl
				 <<T0<<"    =    T0"<<nl
				 <<Mu0<<"    =    dynamic viscosity"<<nl
				 <<Nu0<<"    =    kinetic viscosity"<<nl
				 <<Tau0<<"    =    tau0"<<nl
				 <<Lambda0<<"    =    Lambda0"<<nl
				 <<RESIDUAL<<"    =    RESIDUAL"<<nl
				 <<"-------------Phase Field--------------"<<nl
				 <<PhaseFieldAC::M_Phi<<"    =    mobility"<<nl
				 <<PhaseFieldAC::Cn<<"    =    Cahn number"<<nl
				 <<PhaseFieldAC::Pe<<"    =    Peclet number"<<nl
				 <<PhaseFieldAC::wI<<"    =    inteface width"<<nl
				 <<PhaseFieldAC::Sigma<<"    =    surface tension coefficient"<<nl
				 <<PhaseFieldAC::Beta<<"    =    Beta"<<nl
				 <<PhaseFieldAC::Kappa<<"    =    Kappa"<<nl
				 <<PhaseFieldAC::RhoL<<"    =    liquid density"<<nl
				 <<PhaseFieldAC::RhoV<<"    =    vapor density"<<nl
				 <<PhaseFieldAC::NuL<<"    =    liquid viscosity"<<nl
				 <<PhaseFieldAC::NuV<<"    =    vapor viscosity"<<nl
				 <<PhaseFieldAC::TauPhi<<"    =    mobility relaxition time";
	OutFile_Case.close();
}
void Output_SumRho(int step)
{
	ostringstream oss_SumRho;

	oss_SumRho << "../Convergence/SumRho_Mesh"<<Lx<<"Nu0"<<Nu0
		   <<"M"<<PhaseFieldAC::M_Phi<<"_"<<_CASE_NAME_ARK<<".dat";

	ofstream OutFile_SumRho(oss_SumRho.str().c_str(),ofstream::app);
	
	OutFile_SumRho << setiosflags(ios::scientific)<<setprecision(8);
	OutFile_SumRho <<  step <<"    "<<SumRho<<nl;
	OutFile_SumRho.close();
}
void Output_Residual(int step,double Residual)
{
	ostringstream oss_Residual;
	oss_Residual <<"../Convergence/Residual_Mesh"<<Lx<<"Nu0"<<Nu0
		   		 <<"M"<<PhaseFieldAC::M_Phi<<"_"<<_CASE_NAME_ARK<<".dat";
	ofstream OutFile_Residual(oss_Residual.str().c_str(),ofstream::app);
//
	OutFile_Residual << setiosflags(ios::scientific)<<setprecision(10);
	OutFile_Residual << step <<"    "<<Residual<<nl;
	OutFile_Residual.close();
}
void Output_flowfield(int step)
{
	ostringstream oss_flowfield;
	oss_flowfield << "../FlowField/global/" << "step"<<step<<"Mesh"<<Lx<<"tau"<<Tau0<<"_LBM.dat";
	ofstream OutFile_flowfield(oss_flowfield.str());
	if(!OutFile_flowfield)
	{
		cout <<"OutFile_flowfield open failed" << '\n';
		getchar();
		return; 
	}
//
	ostringstream title,varName,dataIJK,VarLocation;
	title << "title = \"Step"<<step<<"_"<<"tau"<<Tau0<<"\""<<"\n";
	varName << "variables = x,y,<Greek>f</Greek>,<Greek>r</Greek>,u,v,p,\
			<Greek>f</Greek><sub>x</sub>,<Greek>f</Greek><sub>y</sub>,\
			Fx,Fy\n";
	dataIJK << "zone I = "<<Lx<<", J = "<<Ly<<", F = BLOCK\n";
	string tec360Header[3]={title.str().c_str(),varName.str().c_str(),
							dataIJK.str().c_str()};
//
	OutFile_flowfield<< tec360Header[0]<<tec360Header[1]<<tec360Header[2];
	OutFile_flowfield<< setiosflags(ios::scientific) << setprecision(12);
//
	LoopPS(Lx1,Ly1)
	{
		OutFile_flowfield<<NodeArray[i][j].xn<<"    ";
		if(j%10 == 0)
			OutFile_flowfield<<"\n";
	}
	OutFile_flowfield<<"\n";
//
	LoopPS(Lx1,Ly1)
	{
		OutFile_flowfield<<NodeArray[i][j].yn<<"    ";
		if(j%10 == 0)
			OutFile_flowfield<<"\n";
	}
	OutFile_flowfield<<"\n";
//
	LoopPS(Lx1,Ly1)
	{
		OutFile_flowfield<<NodeArray[i][j].msq->Phi<<"    ";
		if(j%10 == 0)
			OutFile_flowfield<<"\n";
	}
	OutFile_flowfield<<"\n";
//
	LoopPS(Lx1,Ly1)
	{
		OutFile_flowfield<<NodeArray[i][j].msq->Rho<<"    ";
		if(j%10 == 0)
			OutFile_flowfield<<"\n";
	}
	OutFile_flowfield<<"\n";
	
	LoopPS(Lx1,Ly1)
	{
		OutFile_flowfield<<NodeArray[i][j].msq->U<<"    ";
		if(j%10 == 0)
			OutFile_flowfield<<"\n";
	}
	OutFile_flowfield<<"\n";
	
	LoopPS(Lx1,Ly1)
	{
		OutFile_flowfield<<NodeArray[i][j].msq->V<<"    ";
		if(j%10 == 0)
			OutFile_flowfield<<"\n";
	}
	OutFile_flowfield<<"\n";
	
	LoopPS(Lx1,Ly1)
	{
		OutFile_flowfield<<NodeArray[i][j].msq->p<<"    ";
		if(j%10 == 0)
			OutFile_flowfield<<"\n";
	}
	OutFile_flowfield<<"\n";

	// LoopPS(Lx1,Ly1)
	// {
	// 	OutFile_flowfield<<(*NodeArray[i][j].msq->PseudoPsi)<<"    ";
	// 	if(j%10 == 0)
	// 		OutFile_flowfield<<"\n";
	// }
	// OutFile_flowfield<<"\n";
	LoopPS(Lx1,Ly1)
	{
		OutFile_flowfield<<(NodeArray[i][j].MsQ().Phi_x)<<"    ";
		if(j%10 == 0)
			OutFile_flowfield<<"\n";
	}
	OutFile_flowfield<<"\n";
	LoopPS(Lx1,Ly1)
	{
		OutFile_flowfield<<(NodeArray[i][j].MsQ().Phi_y)<<"    ";
		if(j%10 == 0)
			OutFile_flowfield<<"\n";
	}
	LoopPS(Lx1,Ly1)
	{
		OutFile_flowfield<<(NodeArray[i][j].MsQ().Fx)<<"    ";
		if(j%10 == 0)
			OutFile_flowfield<<"\n";
	}
	OutFile_flowfield<<"\n";
	LoopPS(Lx1,Ly1)
	{
		OutFile_flowfield<<(NodeArray[i][j].MsQ().Fy)<<"    ";
		if(j%10 == 0)
			OutFile_flowfield<<"\n";
	}
}
void Output_MidxP()
{
	ostringstream oss_MidxP;
	oss_MidxP << "../FlowField/" << "Midxp_Mesh"<<Lx<<"tau"<<Tau0<<"radius"<<radius<<"_RLBM.dat";
	ofstream OF_MidxP(oss_MidxP.str().c_str());
	OF_MidxP << setiosflags(ios::scientific) << setprecision(12);
	for(int i = 1;i < Lx1;++i)
	{
		OF_MidxP << NodeArray[i][64].xn <<"    "
				 << NodeArray[i][64].msq->p <<"    "
				 << NodeArray[i][64].msq->Rho <<"    "
				 <<'\n';
	}
}
void Output_MidyP()
{
	ostringstream oss_MidYP;
	oss_MidYP << "../FlowField/" << "MidYp_Mesh"<<Lx<<"tau"<<Tau0<<"radius"<<radius<<"_RLBM.dat";
	ofstream OF_MidYP(oss_MidYP.str().c_str());
	OF_MidYP << setiosflags(ios::scientific) << setprecision(12);
	for(int j = 1;j < Ly1;++j)
	{
		OF_MidYP << NodeArray[64][j].yn <<"    "
				 << NodeArray[64][j].msq->p<<"    "
				 << NodeArray[64][j].msq->Rho<<"    "
				 <<'\n';
	}
}
void updateResidual()
{
	SumRho = 0.0;
	double SumdRho = 0.0,dRho = 0.0;
	LoopPS(Lx1,Ly1)
	{
		dRho = NodeArray[i][j].MsQ().Rho - NodeArray[i][j].MsQ().Rho_1K;
		SumdRho += dRho*dRho;
		SumRho += NodeArray[i][j].MsQ().Rho*NodeArray[i][j].MsQ().Rho;
		NodeArray[i][j].MsQ().Rho_1K = NodeArray[i][j].MsQ().Rho;
	}
	ResidualPer1k = sqrt(SumdRho/(SumRho));
//-----------------------------------------------
	// double SumUV = 0.0,Sumdudv = 0.0;
	// double du = 0.0, dv = 0.0;
	// double dRho = 0.0;
	// for(int i = 1;i < Lx1;++i)
	// for(int j = 1;j < Ly1;++j)
	// {
	// 	SumRho += NodeArray[i][j].MsQ().Rho;
	// 	du = NodeArray[i][j].msq->U - NodeArray[i][j].msq->U_1K;
	// 	dv = NodeArray[i][j].msq->V - NodeArray[i][j].msq->V_1K;
	// 	Sumdudv = du*du + dv*dv;
	// 	SumUV += (NodeArray[i][j].msq->U*NodeArray[i][j].msq->U + NodeArray[i][j].msq->V*NodeArray[i][j].msq->V);
	// 	NodeArray[i][j].msq->U_1K = NodeArray[i][j].msq->U;
	// 	NodeArray[i][j].msq->V_1K = NodeArray[i][j].msq->V;
	// }
	// ResidualPer1k = sqrt(Sumdudv/(SumUV+1E-30));
	cout << setiosflags(ios::scientific) << setprecision(12);
	cout <<step <<"    "<<SumRho<<"    "<<ResidualPer1k<<'\n';
	if(step%writeFileStep == 0)
		Output_flowfield(step);
}