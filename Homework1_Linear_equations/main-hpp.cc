#include"matrix.hpp"
#include <random>
pp::matrix eye(int n){
	pp::matrix M(n,n);
	for(int i=0;i<n;i++)M(i,i)=1;
	return M;
	}
int main(){
	// TEST MATRICES START

	int n=3;
	pp::vector v(n);
	v.print("v=");
	for(int i=0;i<v.size();i++)v[i]=i+1;
	v.print("v=");
	pp::vector u=v;
	u.print("u=");
	v[0]=999;
	u.print("u=");
	pp::vector w(v);
	w.print("w=");
	w+=v;
	w.print("w+=v");
	(w/10).print("w/10");
	(u+u).print("u+u");

	pp::matrix M(n,n);
	M.print("M=");
	for(int i=0;i<M.size1();i++)
	for(int j=0;j<M.size2();j++)
		M(i,j)=i+j;
	M.print("M=");
	(M+M).print("M+M");
	(M-M).print("M-M");
	(M*(M+M)*M).print("M*(M+M)*M");
	(M*u).print("M*u");
	(M/10).print("M/10");
	pp::matrix O=eye(n);
	O.print("O=");

	// TEST MATRICES FINISH

	// TEST QR START

	int k = 5; // rows
	int l = 3; // columns (n > m → tall matrix)

	pp::matrix A(k, l);

	// random number generator
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<double> dist(-1.0, 1.0);

	// fill matrix
	for(int i = 0; i < k; i++) {
		for(int j = 0; j < l; j++) {
			A(i,j) = dist(gen);
		}
	}

	A.print("Random matrix A:");
	
	pp::qr decomp(A);   // apply QR factorization

	pp::matrix QR = decomp.Q * decomp.R;
	QR.print("Q*R:");

	decomp.Q.print("Q:");
	decomp.R.print("R:");
	std::cout << "det = " << decomp.det() << "\n";
	

	pp::matrix QTQ = decomp.Q.transpose() * decomp.Q;
	QTQ.print("Q^TQ:");


	// TEST QR FINISH

	// TEST SOLVER START
	pp::matrix A_solver(k, k);
	for(int i = 0; i < k; i++) {
		for(int j = 0; j < k; j++) {
			A_solver(i,j) = dist(gen);
		}
	}
	A_solver.print("Random matrix solver A:");

	pp::vector b(k);
	for(int i = 0; i < k; i++) {
		b[i] = dist(gen);
	}
	b.print("random b:");
	pp::qr decomp_solver(A_solver);
	pp::vector x = decomp_solver.solve(b);
	x.print("found solution:");
	pp::vector b_solution = A_solver*x;
	b_solution.print("b from solution:");

	// TEST SOLVER FINISH 
	
	// TEST INVERSE START

	
	pp::matrix Ainv = decomp_solver.inverse(A_solver);
	Ainv.print("A inverse:");
	pp::matrix test = A_solver * Ainv;
	test.print("A * Ainv:");

	// TEST INVERSE FINISH


	







	return 0;
}
