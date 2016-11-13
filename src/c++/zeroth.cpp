#include "andersionz.h"
//#include <chkp.h>
int main()
{
	std::cout<<"Starting NRG .... "<<std::endl;
	param.setparam() ;
	param.maxStates=500;
	param.U=0.50; 
	param.V=std::sqrt(0.014/std::acos(-1)); 
	param.eps=-param.U/2  ;
	std::ostringstream ss;
	ss <<"spectr-U"<<param.U<<".dat";
	std::string s(ss.str());
	std::ofstream pfile(s) ; 

	//Create first Initial Matrix 
	double * a = new double [param.currDim*param.currDim] () ; //matrix
	double * exa = new double [param.currDim*param.currDim] () ; //matrix
	double * w = new double [param.currDim] () ; //eigen value
	double * exw = new double [param.currDim] () ; //eigen value
	double * up = new double [param.currDim*param.currDim] () ; //eigen value
	double * oldup = new double [param.currDim*param.currDim] () ; //eigen value
	std::cout<<"Starting NRG Dim: "<<param.currDim<<std::endl;
	QDanderson.zinitMatrix(a) ; //create matrix
	matrix.diag(a,w,param.currDim) ; //Diagonalize the matrix
	QDanderson.initOperator(a,oldup);
	matrix.dispArray(w,param.currDim) ;


	double  hopping =1.0;
	int Noiter=60,iter=1;
	
	for(int iter=0; iter<Noiter;iter++)
	{
	
	hopping=param.hopping(param.wchain) ;
	std::cout<<"Starting Total Dim: "<<param.currDim<<std::endl;
	//Add a site to the wilson Chain.
	param.currDim=param.noStates*4 ; 
	delete [] exa ; 
	exa = new (std::nothrow) double [param.currDim*param.currDim]() ; 
	if (!exa) std::cout<<"Unable to allocate memory for exa"<<std::endl;  
	QDanderson.zaddaSite(exa,a,w,hopping);
	//matrix.dispMatrix(exa,param.currDim) ;
	
	delete [] exw; 
	exw = new (std::nothrow) double [param.currDim] () ; //eigen value
	if (!exw) std::cout<<"Unable to allocate memory for exw"<<std::endl;  
	
	matrix.diag(exa,exw,param.currDim) ;
	//matrix.dispArray(exw,param.currDim) ;
	
	delete [] a;
	a = new (std::nothrow)  double [param.currDim*param.currDim] () ; //matrix
	if (!a) std::cout<<"Unable to allocate memory for a"<<std::endl;  
	delete [] w ;
	w = new (std::nothrow)  double [param.currDim] () ; //matrix
	if (!w) std::cout<<"Unable to allocate memory for w"<<std::endl;  
	matrix.copyMatrix(a,exa,param.currDim) ; //Keep a copy of the matrix 
	matrix.copyArrG(w,exw,param.currDim) ; //Keep a copy of the matrix 
	std::cout<<"Lowest eigenvalue: "<<w[0]<<" and Highest eigenvalue: "<<w[param.currDim-1]<<std::endl;

	delete [] up;
	up = new (std::nothrow)  double [param.currDim*param.currDim] () ; //matrix
	if (!up) std::cout<<"Unable to allocate memory for a"<<std::endl; 
	QDanderson.updateUP(exa,up,oldup);

	delete [] oldup;
	oldup = new (std::nothrow)  double [param.currDim*param.currDim] () ; //matrix
	if (!oldup) std::cout<<"Unable to allocate memory for a"<<std::endl;  
	matrix.copyMatrix(oldup,up,param.currDim) ;

	param.preDim=param.currDim;
	param.wchain=param.wchain+1;
	if(4*param.noStates < param.maxStates )
		param.noStates = 4*param.noStates ; 
	else 
		param.noStates = param.maxStates ;


	}
	
	std::cout<<"Calc for spectrum started .. "<<std::endl;
	QDanderson.calcSpec(up,w,pfile ) ;
		
	
	pfile.close();
	delete [] a;
	delete [] w;
	delete [] exa;
	delete [] exw;		
	return 0;

}

