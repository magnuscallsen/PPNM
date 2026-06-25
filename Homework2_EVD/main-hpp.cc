#include"matrix.hpp"
#include <random>
pp::matrix eye(int n){
	pp::matrix M(n,n);
	for(int i=0;i<n;i++)M(i,i)=1;
	return M;
	}
int main(){
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<double> dist(-1.0, 1.0);
	int n = 4;
	pp::matrix A(n, n);
	for(int i = 0; i < n; i++) {
		for(int j = i; j < n; j++) {
			double num = dist(gen);
			A(i,j) = num;
			A(j,i) = num;
		}
	}

	A.print("Random symmetric matrix A:");

	pp::EVD evd(A);
	evd.w.print("Eigenvalues:");
	pp::matrix VTAV = evd.V.transpose()*A*evd.V;
	VTAV.print("VTAV");

	pp::matrix VDVT = evd.V*VTAV*evd.V.transpose();
	VDVT.print("VDV^T:");

	pp::matrix VVT = evd.V.transpose()*evd.V;
	VVT.print("VVT:");

	pp::matrix VTV = evd.V*evd.V.transpose();
	VTV.print("VTV:");



	return 0;
}
