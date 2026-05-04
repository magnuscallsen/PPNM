#pragma once

#include <iostream>
#include <cstdio>
#include <cassert>
#include <cmath>
#include <string>
#include <vector>
#include <initializer_list>
#include <functional>

#define SELF (*this)
#define FOR(i,V) for(int i=0; i<(V).size(); i++)
#define FOR1(i,A) for(int i=0; i<(A).size1(); i++)
#define FOR2(j,A) for(int j=0; j<(A).size2(); j++)

namespace pp {

struct vector {
	std::vector<double> data;

	vector() = default;
	vector(int n) : data(n) {}
	vector(std::initializer_list<double> list) : data(list) {}

	int size() const { return data.size(); }

	double& operator[](int i) { return data[i]; }
	const double& operator[](int i) const { return data[i]; }

	vector& operator+=(const vector& other) {
		assert(size() == other.size());
		FOR(i, SELF) SELF[i] += other[i];
		return SELF;
	}

	vector& operator-=(const vector& other) {
		assert(size() == other.size());
		FOR(i, SELF) SELF[i] -= other[i];
		return SELF;
	}

	vector& operator*=(double c) {
		FOR(i, SELF) SELF[i] *= c;
		return SELF;
	}

	vector& operator/=(double c) {
		assert(c != 0);
		FOR(i, SELF) SELF[i] /= c;
		return SELF;
	}

	// Dot product
	double operator%(const vector& other) const {
		assert(size() == other.size());
		double sum = 0;
		FOR(i, SELF) sum += SELF[i] * other[i];
		return sum;
	}

	double norm() const {
		return std::sqrt(SELF % SELF);
	}

	void print(std::string s = "") const {
		std::cout << s;
		for(auto x : data) std::printf("%10.3g ", x);
		std::cout << "\n";
	}

	vector map(std::function<double(double)> f) const {
		vector r(size());
		FOR(i, SELF) r[i] = f(SELF[i]);
		return r;
	}
};

inline vector operator+(vector a, const vector& b) { a += b; return a; }
inline vector operator-(vector a)                  { a *= -1; return a; }
inline vector operator-(vector a, const vector& b) { a -= b; return a; }
inline vector operator*(vector a, double c)        { a *= c; return a; }
inline vector operator*(double c, vector a)        { a *= c; return a; }
inline vector operator/(vector a, double c)        { a /= c; return a; }

inline bool approx(double x, double y, double acc = 1e-6, double eps = 1e-6) {
	if(std::abs(x - y) < acc) return true;
	if(std::abs(x - y) < eps * std::max(std::abs(x), std::abs(y))) return true;
	return false;
}

inline bool approx(const vector& a, const vector& b, double acc = 1e-6, double eps = 1e-6) {
	if(a.size() != b.size()) return false;
	for(int i = 0; i < a.size(); i++)
		if(!approx(a[i], b[i], acc, eps)) return false;
	return true;
}

struct matrix {
	std::vector<pp::vector> cols;

	matrix() = default;
	matrix(int n, int m) : cols(m, pp::vector(n)) {}

	int size1() const { return cols.empty() ? 0 : cols[0].size(); } // rows
	int size2() const { return cols.size(); }                       // columns

	double& operator()(int i, int j) { return cols[j][i]; }
	const double& operator()(int i, int j) const { return cols[j][i]; }

	vector& operator[](int j) { return cols[j]; }
	const vector& operator[](int j) const { return cols[j]; }

	void setid() {
		assert(size1() == size2());
		for(int i = 0; i < size1(); i++)
			for(int j = 0; j < size2(); j++)
				SELF(i,j) = (i == j ? 1.0 : 0.0);
	}

	matrix transpose() const {
		matrix R(size2(), size1());
		FOR1(i, SELF)
		FOR2(j, SELF)
			R(j,i) = SELF(i,j);
		return R;
	}

	matrix T() const { return transpose(); }

	double get(int i, int j) const { return cols[j][i]; }
	void set(int i, int j, double value) { cols[j][i] = value; }

	matrix& operator+=(const matrix& B) {
		assert(size1() == B.size1());
		assert(size2() == B.size2());
		for(int j = 0; j < size2(); j++) SELF[j] += B[j];
		return SELF;
	}

	matrix& operator-=(const matrix& B) {
		assert(size1() == B.size1());
		assert(size2() == B.size2());
		for(int j = 0; j < size2(); j++) SELF[j] -= B[j];
		return SELF;
	}

	matrix& operator*=(double c) {
		for(auto& col : cols) col *= c;
		return SELF;
	}

	matrix& operator/=(double c) {
		assert(c != 0);
		for(auto& col : cols) col /= c;
		return SELF;
	}

	void print(std::string s = "") const {
		std::printf("%s\n", s.c_str());
		for(int i = 0; i < size1(); i++) {
			for(int j = 0; j < size2(); j++)
				std::printf("%10.3g ", SELF(i,j));
			std::printf("\n");
		}
	}
};

inline matrix operator+(matrix A, const matrix& B) {
	A += B;
	return A;
}

inline matrix operator-(matrix A, const matrix& B) {
	A -= B;
	return A;
}

inline vector operator*(const matrix& A, const vector& v) {
	assert(A.size2() == v.size());

	vector r(A.size1());

	FOR2(j, A) {
		double vj = v[j];
		FOR1(i, A) r[i] += A(i,j) * vj;
	}

	return r;
}

inline matrix operator*(const matrix& A, const matrix& B) {
	assert(A.size2() == B.size1());

	matrix R(A.size1(), B.size2());

	FOR2(j, R)
		R[j] = A * B[j];

	return R;
}

inline matrix operator*(matrix A, double c) {
	A *= c;
	return A;
}

inline matrix operator*(double c, matrix A) {
	A *= c;
	return A;
}

inline matrix operator/(matrix A, double c) {
	A /= c;
	return A;
}

inline bool approx(const matrix& A, const matrix& B, double acc = 1e-6, double eps = 1e-6) {
	if(A.size1() != B.size1()) return false;
	if(A.size2() != B.size2()) return false;

	for(int i = 0; i < A.size1(); i++)
		for(int j = 0; j < A.size2(); j++)
			if(!approx(A(i,j), B(i,j), acc, eps)) return false;

	return true;
}
struct qr {
	matrix Q;
	matrix R;

	qr(const matrix& A) {
		int n = A.size1();
		int m = A.size2();

		assert(n >= m);

		Q = A;
		R = matrix(m, m);

		for(int i = 0; i < m; i++) {
			R(i,i) = Q[i].norm();

			assert(R(i,i) != 0);

			Q[i] /= R(i,i);

			for(int j = i + 1; j < m; j++) {
				R(i,j) = Q[i] % Q[j];
				Q[j] -= Q[i] * R(i,j);
			}
		}
	}

	vector solve(vector b) {
		int m = R.size1();

		vector b_tilde = Q.transpose() * b;
		vector x(m);

		for(int i = m - 1; i >= 0; i--) {
			double sum = 0.0;
			for(int j = i + 1; j < m; j++) {
				sum += R(i,j) * x[j];
			}
			x[i] = (b_tilde[i] - sum) / R(i,i);
		}

		return x;
	}
	double det() const {
		double product = 1.0;
		for (int i=0; i<R.size1(); i++){
			product *= R(i, i);
		}
		return product;
	}

	matrix inverse(const matrix& A){
		assert(A.size1() == A.size2());
		int n = A.size1();
		
		pp::matrix Ainv(n, n);
		pp::qr decomp(A);

		for(int i = 0; i < n; i++) {
			pp::vector e(n);
			e[i] = 1.0;          

			pp::vector x = decomp.solve(e);

			Ainv[i] = x;      
		}

		return Ainv;
	}



	
};

} // namespace pp