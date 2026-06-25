#include "minimization.hpp"
#include <cmath>
#include <stdexcept>
#include <limits>

namespace minopt {

pp::vector gradient(const Function& phi, const pp::vector& x) {
    const int n = x.size();
    pp::vector g(n);
    pp::vector x_step = x;
    const double phi_x = phi(x);

    for(int i = 0; i < n; ++i) {
        const double dxi = (1.0 + std::abs(x[i])) * std::pow(2.0, -26);
        x_step[i] += dxi;
        g[i] = (phi(x_step) - phi_x) / dxi;
        x_step[i] = x[i];
    }

    return g;
}

pp::matrix hessian(const Function& phi, const pp::vector& x) {
    const int n = x.size();
    pp::matrix H(n, n);
    const pp::vector g_x = gradient(phi, x);
    pp::vector x_step = x;

    for(int j = 0; j < n; ++j) {
        const double dxj = (1.0 + std::abs(x[j])) * std::pow(2.0, -13);
        x_step[j] += dxj;
        pp::vector dg = gradient(phi, x_step) - g_x;

        for(int i = 0; i < n; ++i) {
            H(i, j) = dg[i] / dxj;
        }

        x_step[j] = x[j];
    }

    for(int i = 0; i < n; ++i) {
        for(int j = i + 1; j < n; ++j) {
            const double Hij = 0.5 * (H(i, j) + H(j, i));
            H(i, j) = Hij;
            H(j, i) = Hij;
        }
    }

    return H;
}
// Functions below is made by AI
NewtonResult newton(const Function& phi, pp::vector x, double acc, int max_steps) {
    NewtonResult result;

    for(int step = 0; step < max_steps; ++step) {
        pp::vector g = gradient(phi, x);
        const double gnorm = g.norm();

        result.steps = step;
        result.x = x;
        result.fx = phi(x);
        result.grad_norm = gnorm;

        if(gnorm < acc) {
            result.converged = true;
            return result;
        }

        pp::matrix H = hessian(phi, x);

        for(int i = 0; i < H.size1(); ++i) {
            H(i, i) += 1e-6;
        }

        pp::vector dx;

        try {
            dx = pp::qr(H).solve(-g);
        } catch(...) {
            throw std::runtime_error("QR solve failed. Hessian may be singular or ill-conditioned.");
        }

        const double phi_x = result.fx;
        double lambda = 1.0;

        while(lambda >= 1.0 / 1024.0) {
            if(phi(x + lambda * dx) < phi_x) break;
            lambda /= 2.0;
        }

        x = x + lambda * dx;
    }

    result.x = x;
    result.fx = phi(x);
    result.grad_norm = gradient(phi, x).norm();
    result.steps = max_steps;
    result.converged = result.grad_norm < acc;

    return result;
}

static void gradient_hessian_central(
    const Function& phi,
    const pp::vector& x,
    pp::vector& g,
    pp::matrix& H
) {
    const int n = x.size();
    const double eps = std::numeric_limits<double>::epsilon();

    g = pp::vector(n);
    H = pp::matrix(n, n);

    const double fx = phi(x);

    for(int i = 0; i < n; ++i) {
        const double dxi = std::pow(eps, 1.0 / 3.0) * (1.0 + std::abs(x[i]));

        pp::vector xp = x;
        pp::vector xm = x;

        xp[i] += dxi;
        xm[i] -= dxi;

        const double fp = phi(xp);
        const double fm = phi(xm);

        g[i] = (fp - fm) / (2.0 * dxi);
        H(i, i) = (fp - 2.0 * fx + fm) / (dxi * dxi);
    }

    for(int i = 0; i < n; ++i) {
        for(int j = i + 1; j < n; ++j) {
            const double dxi = std::pow(eps, 1.0 / 3.0) * (1.0 + std::abs(x[i]));
            const double dxj = std::pow(eps, 1.0 / 3.0) * (1.0 + std::abs(x[j]));

            pp::vector xpp = x;
            pp::vector xpm = x;
            pp::vector xmp = x;
            pp::vector xmm = x;

            xpp[i] += dxi;
            xpp[j] += dxj;

            xpm[i] += dxi;
            xpm[j] -= dxj;

            xmp[i] -= dxi;
            xmp[j] += dxj;

            xmm[i] -= dxi;
            xmm[j] -= dxj;

            const double fpp = phi(xpp);
            const double fpm = phi(xpm);
            const double fmp = phi(xmp);
            const double fmm = phi(xmm);

            const double Hij = (fpp - fpm - fmp + fmm) / (4.0 * dxi * dxj);

            H(i, j) = Hij;
            H(j, i) = Hij;
        }
    }
}

NewtonResult newton_central(const Function& phi, pp::vector x, double acc, int max_steps) {
    NewtonResult result;

    for(int step = 0; step < max_steps; ++step) {
        pp::vector g;
        pp::matrix H;

        gradient_hessian_central(phi, x, g, H);

        const double gnorm = g.norm();

        result.steps = step;
        result.x = x;
        result.fx = phi(x);
        result.grad_norm = gnorm;

        if(gnorm < acc) {
            result.converged = true;
            return result;
        }

        for(int i = 0; i < H.size1(); ++i) {
            H(i, i) += 1e-6;
        }

        pp::vector dx;

        try {
            dx = pp::qr(H).solve(-g);
        } catch(...) {
            throw std::runtime_error("QR solve failed in central Newton.");
        }

        const double phi_x = result.fx;
        double lambda = 1.0;

        while(lambda >= 1.0 / 1024.0) {
            if(phi(x + lambda * dx) < phi_x) break;
            lambda /= 2.0;
        }

        x = x + lambda * dx;
    }

    pp::vector g;
    pp::matrix H;
    gradient_hessian_central(phi, x, g, H);

    result.x = x;
    result.fx = phi(x);
    result.grad_norm = g.norm();
    result.steps = max_steps;
    result.converged = result.grad_norm < acc;

    return result;
}

} // namespace minopt