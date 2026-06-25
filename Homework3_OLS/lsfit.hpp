#pragma once
#include <tuple>
#include "matrix.hpp"
#include <functional>
#include <vector>
#include <cassert>

namespace pp {

using Func = std::function<double(double)>;

inline std::tuple<vector, matrix> lsfit(const std::vector<Func>& fs,
                    const vector& x,
                    const vector& y,
                    const vector& dy)
{
    int n = x.size();       // number of data points
    int m = fs.size();      // number of fitting functions

    assert(y.size() == n);
    assert(dy.size() == n);
    assert(n >= m);

    matrix A(n,m);
    vector b(n);

    for(int i=0; i<n; i++){
        b[i] = y[i]/dy[i];
        for(int k=0; k<m; k++){
            A(i,k) = fs[k](x[i])/dy[i];
        }
    }

    qr QR(A);   
    vector c = QR.solve(b);

    matrix AtA = A.T()*A;
    matrix cov = QR.inverse(AtA);

    return {c,cov};    
    
}

} // namespace pp
