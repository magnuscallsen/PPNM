

#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <functional>
#include <iomanip>
#include <algorithm>
#include <stdexcept>
#include "ode.hpp"
#include <fstream>
using namespace std;

using vec = vector<double>;
using mat = vector<vec>;
using func = function<vec(const vec&)>;

double norm(const vec& v){
    double s=0;
    for(double x:v) s+=x*x;
    return sqrt(s);
}

vec operator+(const vec& a,const vec& b){
    vec c(a.size());
    for(size_t i=0;i<a.size();i++) c[i]=a[i]+b[i];
    return c;
}

vec operator-(const vec& a,const vec& b){
    vec c(a.size());
    for(size_t i=0;i<a.size();i++) c[i]=a[i]-b[i];
    return c;
}

vec operator-(const vec& a){
    vec c(a.size());
    for(size_t i=0;i<a.size();i++) c[i]=-a[i];
    return c;
}

vec operator*(double k,const vec& a){
    vec c(a.size());
    for(size_t i=0;i<a.size();i++) c[i]=k*a[i];
    return c;
}

void print(string name, const vec& x){
    cout << name << " = (";
    for(size_t i=0;i<x.size();i++){
        cout << setprecision(12) << x[i];
        if(i+1<x.size()) cout << ", ";
    }
    cout << ")";
}
// jacobian written by AI

// Numerical Jacobian using finite differences
mat jacobian(func f, const vec& x, const vec& fx){
    int n=x.size();
    mat J(n,vec(n));
    vec z=x;
    double eps=pow(2,-26);

    for(int j=0;j<n;j++){
        double dx=max(abs(x[j]),1.0)*eps;
        z[j]+=dx;
        vec df=f(z)-fx;
        for(int i=0;i<n;i++) J[i][j]=df[i]/dx;
        z[j]=x[j];
    }

    return J;
}
void jacobian_update(func f, const vec& x, const vec& fx, mat& J){
    int n=x.size();
    vec z=x;
    double eps=pow(2,-26);

    for(int j=0;j<n;j++){
        double dx=max(abs(x[j]),1.0)*eps;
        z[j]+=dx;
        vec df=f(z)-fx;

        for(int i=0;i<n;i++)
            J[i][j]=df[i]/dx;

        z[j]=x[j];
    }
} // written by AI

// QR decomposition using classical Gram-Schmidt. Taken from my earlier code
void qr_decomp(const mat& A, mat& Q, mat& R){
    int n=A.size();
    Q=mat(n,vec(n,0));
    R=mat(n,vec(n,0));
    vector<vec> q(n,vec(n));

    for(int j=0;j<n;j++){
        vec v(n);
        for(int i=0;i<n;i++) v[i]=A[i][j];

        for(int k=0;k<j;k++){
            double r=0;
            for(int i=0;i<n;i++) r+=q[k][i]*v[i];
            R[k][j]=r;
            for(int i=0;i<n;i++) v[i]-=r*q[k][i];
        }

        R[j][j]=norm(v);
        if(R[j][j]<1e-14) throw runtime_error("Singular matrix");

        for(int i=0;i<n;i++){
            q[j][i]=v[i]/R[j][j];
            Q[i][j]=q[j][i];
        }
    }
}

// Solve Ax=b with QR decomposition. Taken from earlier code
vec qr_solve(const mat& A, const vec& b){
    int n=A.size();
    mat Q,R;
    qr_decomp(A,Q,R);

    vec c(n,0);
    for(int i=0;i<n;i++)
        for(int k=0;k<n;k++)
            c[i]+=Q[k][i]*b[k];

    vec x(n);
    for(int i=n-1;i>=0;i--){
        double s=c[i];
        for(int j=i+1;j<n;j++) s-=R[i][j]*x[j];
        x[i]=s/R[i][i];
    }

    return x;
}


// Newton method with numerical Jacobian and backtracking line search. Made by AI
vec newton(func f, vec x, double acc=1e-8, double alpha_min=1e-3, int max_iter=100){
    double eps=pow(2,-26);
    vec fx=f(x);

    for(int iter=0; iter<max_iter; iter++){
        if(norm(fx)<acc) break;

        mat J=jacobian(f,x,fx);
        vec Dx=qr_solve(J,-fx);

        double dxsize=0;
        for(double xi:x){
            double d=max(abs(xi),1.0)*eps;
            dxsize+=d*d;
        }
        dxsize=sqrt(dxsize);

        if(norm(Dx)<dxsize) break;

        double alpha=1;
        vec z,fz;

        while(true){
            z=x+alpha*Dx;
            fz=f(z);

            if(norm(fz)<norm(fx)) break;
            if(alpha<alpha_min) break;

            alpha/=2;
        }

        x=z;
        fx=fz;
    }

    return x;
}
vec newton_quadratic(func f, vec x,
                     double acc=1e-8,
                     double alpha_min=1e-3,
                     int max_iter=100){

    double eps=pow(2,-26);
    vec fx=f(x);

    int n=x.size();
    mat J(n, vec(n)); // allocated once

    for(int iter=0; iter<max_iter; iter++){
        if(norm(fx)<acc) break;

        jacobian_update(f,x,fx,J);
        vec Dx=qr_solve(J,-fx);

        double dxsize=0;
        for(double xi:x){
            double d=max(abs(xi),1.0)*eps;
            dxsize+=d*d;
        }
        dxsize=sqrt(dxsize);

        if(norm(Dx)<dxsize) break;

        double alpha=1;
        vec z,fz;

        double phi0 = norm(fx);
        double phi_prime0 = -phi0;

        while(true){
            z = x + alpha*Dx;
            fz = f(z);

            double phi = norm(fz);

            if(phi < (1-alpha/2)*phi0) break;
            if(alpha < alpha_min) break;

            double denom = 2*(phi - phi0 - phi_prime0*alpha);

            if(abs(denom)>1e-14){
                double alpha_new =
                    -phi_prime0*alpha*alpha/denom;

                if(alpha_new < alpha/4) alpha_new = alpha/4;
                if(alpha_new > alpha/2) alpha_new = alpha/2;

                alpha = alpha_new;
            }
            else{
                alpha /= 2;
            }
        }

        x=z;
        fx=fz;
    }

    return x;
} // also this is written by AI

// Rosenbrock function and analytic gradient
double rosenbrock(double x,double y){
    return pow(1-x,2)+100*pow(y-x*x,2);
} //Found analytically 

vec grad_rosenbrock(const vec& v){
    double x=v[0], y=v[1];

    return {
        -2*(1-x)-400*x*(y-x*x),
        200*(y-x*x)
    };
}//Found analytically

// Below made by AI

// Himmelblau function and analytic gradient
double himmelblau(double x,double y){
    double a=x*x+y-11;
    double b=x+y*y-7;
    return a*a+b*b;
}

vec grad_himmelblau(const vec& v){
    double x=v[0], y=v[1];
    double a=x*x+y-11;
    double b=x+y*y-7;

    return {
        4*x*a+2*b,
        2*a+4*y*b
    };
}

void test(string name, func f, vec start){
    vec root=newton(f,start);
    vec fr=f(root);

    cout << "\n" << name << "\n";
    print("root",root);
    cout << "\n";
    print("f(root)",fr);
    cout << "\n|f(root)| = " << norm(fr) << "\n";
}
// Below code is written by AI

pp::vector hydrogen_rhs(double r, pp::vector y, double E){
    return pp::vector{
        y[1],
        -2*(E + 1/r)*y[0]
    };
}

auto hydrogen_solve(double E, double rmin, double rmax,
                    double h, double acc, double eps){
    pp::vector yinit{
        rmin - rmin*rmin,
        1 - 2*rmin
    };

    auto F = [E](double r, pp::vector y){
        return hydrogen_rhs(r,y,E);
    };

    return ode::driver(F,rmin,rmax,yinit,h,acc,eps,h);
}

double M_of_E(double E, double rmin, double rmax,
              double h, double acc, double eps){
    auto [rlist,ylist] = hydrogen_solve(E,rmin,rmax,h,acc,eps);
    return ylist.back()[0];
}

double hydrogen_energy(double rmin, double rmax,
                       double h, double acc, double eps){
    double a=-0.7, b=-0.3;

    double fa=M_of_E(a,rmin,rmax,h,acc,eps);

    while(abs(b-a)>1e-8){
        double c=(a+b)/2;
        double fc=M_of_E(c,rmin,rmax,h,acc,eps);

        if(fa*fc<0){
            b=c;
        }else{
            a=c;
            fa=fc;
        }
    }

    return (a+b)/2;
}

void hydrogen_demo(){
    cout << "\nHydrogen atom shooting method\n";

    double rmin=1e-3;
    double rmax=8;
    double h=0.05;
    double acc=1e-6;
    double eps=1e-6;

    double E = hydrogen_energy(rmin,rmax,h,acc,eps);

    auto [rlist,ylist] = hydrogen_solve(E,rmin,rmax,h,acc,eps);

    ofstream file("hydrogen_wavefunction.txt");
    file << "# r numerical exact\n";

    for(size_t i=0;i<rlist.size();i++){
        double r = rlist[i];
        file << r << " "
             << ylist[i][0] << " "
             << r*exp(-r) << "\n";
    }

    cout << "rmin = " << rmin << ", rmax = " << rmax << "\n";
    cout << "E0 numerical = " << E << "\n";
    cout << "E0 exact     = " << -0.5 << "\n";
    cout << "error        = " << abs(E+0.5) << "\n";
    cout << "M(E0)        = " << M_of_E(E,rmin,rmax,h,acc,eps) << "\n";

    cout << "\nConvergence with rmax\n";
    for(double R : {4.0,6.0,8.0,10.0}){
        double e = hydrogen_energy(rmin,R,h,acc,eps);
        cout << "rmax = " << R << "   E0 = " << e
             << "   error = " << abs(e+0.5) << "\n";
    }

    cout << "\nConvergence with rmin\n";
    for(double R0 : {1e-1,1e-2,1e-3,1e-4}){
        double e = hydrogen_energy(R0,rmax,h,acc,eps);
        cout << "rmin = " << R0 << "   E0 = " << e
             << "   error = " << abs(e+0.5) << "\n";
    }

    cout << "\nConvergence with ODE acc\n";
    for(double a : {1e-3,1e-4,1e-5,1e-6}){
        double e = hydrogen_energy(rmin,rmax,h,a,eps);
        cout << "acc = " << a << "   E0 = " << e
             << "   error = " << abs(e+0.5) << "\n";
    }

    cout << "\nConvergence with ODE eps\n";
    for(double ep : {1e-3,1e-4,1e-5,1e-6}){
        double e = hydrogen_energy(rmin,rmax,h,acc,ep);
        cout << "eps = " << ep << "   E0 = " << e
             << "   error = " << abs(e+0.5) << "\n";
    }
}
