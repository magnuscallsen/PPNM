#include "ode.hpp"
#include <fstream>
#include <cmath>
#include <string>

int main() {
    auto F = [](double /*x*/, pp::vector y) {
        return pp::vector{
            y[1],
           -y[0]
        };
    };

    double a = 0;
    double b = 10;
    pp::vector yinit = {1.0, 0.0};

    auto [xs, ys] = ode::driver(F, a, b, yinit, 0.125, 1e-4, 1e-4);

    std::ofstream out("harmonic.data");

    for (size_t i = 0; i < xs.size(); ++i) {
        double x = xs[i];

        out << x << " "
            << ys[i][0] << " "
            << ys[i][1] << " "
            << std::cos(x) << " "
            << -std::sin(x) << "\n";
    } // Plotting done by AI
    std::cout << "===========Simple Harmonic=========\n";
    std::cout << "Solving u'' = -u using driver and rkstep12 with Euler midpoint method\n";
    std::cout << "Solution fits very well with analytical one, see plotHarmonic\n";
    std::cout << "===========Lotka-Volterra=========\n";
    std::cout << "Reproduces plots from link. Both Lotka-Volterra and simple harmonic oscilation plots are seen in solution.png\n";
    // Made with AI
    double alpha = 1.5;
    double beta  = 1.0;
    double gamma = 3.0;
    double delta = 1.0;

    auto lotka_volterra = [=](double /*t*/, pp::vector y) {
        double prey     = y[0];
        double predator = y[1];

        return pp::vector{
            alpha * prey - beta * prey * predator,
            delta * prey * predator - gamma * predator
        };
    };

    double a2 = 0;
    double b2 = 15;
    pp::vector yinit2 = {10.0, 5.0};

    auto [ts, zs] =
        ode::driver(lotka_volterra, a2, b2, yinit2, 0.05, 1e-4, 1e-4);

    std::ofstream lotka_out("lotka.data");

    lotka_out << "# t prey predator\n";

    for (size_t i = 0; i < ts.size(); ++i) {
        lotka_out
            << ts[i] << " "
            << zs[i][0] << " "
            << zs[i][1] << "\n";
    }
    std::cout << "===========Relativistic Precession of Planetary orbit=========\n";
    std::cout << " See plots in Rel_solution.png. We see newtonian circular and elliptical orbit\n";
    std::cout << " and we see relativistic precession for finite epsilon as expected.\n";
    
    // MAde by AI

        // ------------------------------------------------------------
    // Task B: Relativistic precession of planetary orbit
    //
    // u'' + u = 1 + eps*u^2
    //
    // Rewrite as:
    // y0 = u
    // y1 = u'
    //
    // y0' = y1
    // y1' = 1 - y0 + eps*y0*y0
    // ------------------------------------------------------------

    auto solve_rel_orbit = [](double eps,
                              pp::vector yinit,
                              double phi_max,
                              const std::string& filename) {

        auto F = [eps](double /*phi*/, pp::vector y) {
            double u  = y[0];
            double up = y[1];

            return pp::vector{
                up,
                1.0 - u + eps*u*u
            };
        };

        auto [phis, ys] =
             ode::driver(F, 0.0, phi_max, yinit,
                0.1, 1e-4, 1e-4, 0.2);

        std::ofstream out(filename);

        out << "# phi u up\n";

        for (size_t i = 0; i < phis.size(); ++i) {
            out << phis[i] << " "
                << ys[i][0] << " "
                << ys[i][1] << "\n";
        }
    };

    double rotations = 6;
    double phi_max = rotations * 2.0 * M_PI;

    solve_rel_orbit(
        0.0,
        pp::vector{1.0, 0.0},
        2.0 * M_PI,
        "rel_circular.data"
    );

    solve_rel_orbit(
        0.0,
        pp::vector{1.0, -0.5},
        phi_max,
        "rel_ellipse.data"
    );

    solve_rel_orbit(
        0.01,
        pp::vector{1.0, -0.5},
        phi_max,
        "rel_precession.data"
    );

    std::cout << "===========Three-body figure-8 orbit=========\n";
    std::cout << "I indeed find this stable 8 figure orbit See Eight Solution.png.\n";
    
    // Below made by AI

    auto threebody = [](double /*t*/, pp::vector z) {
        // z = {vx1, vy1, vx2, vy2, vx3, vy3,
        //      x1,  y1,  x2,  y2,  x3,  y3}

        pp::vector dz(12);

        // velocities
        double vx1 = z[0], vy1 = z[1];
        double vx2 = z[2], vy2 = z[3];
        double vx3 = z[4], vy3 = z[5];

        // positions
        double x1 = z[6],  y1 = z[7];
        double x2 = z[8],  y2 = z[9];
        double x3 = z[10], y3 = z[11];

        auto accel = [](double xi, double yi,
                        double xj, double yj,
                        double& ax, double& ay) {
            double dx = xj - xi;
            double dy = yj - yi;
            double r2 = dx*dx + dy*dy;
            double r = std::sqrt(r2);
            double r3 = r2*r;

            ax += dx/r3;
            ay += dy/r3;
        };

        double ax1 = 0, ay1 = 0;
        double ax2 = 0, ay2 = 0;
        double ax3 = 0, ay3 = 0;

        accel(x1, y1, x2, y2, ax1, ay1);
        accel(x1, y1, x3, y3, ax1, ay1);

        accel(x2, y2, x1, y1, ax2, ay2);
        accel(x2, y2, x3, y3, ax2, ay2);

        accel(x3, y3, x1, y1, ax3, ay3);
        accel(x3, y3, x2, y2, ax3, ay3);

        // derivative of velocities = accelerations
        dz[0] = ax1;
        dz[1] = ay1;
        dz[2] = ax2;
        dz[3] = ay2;
        dz[4] = ax3;
        dz[5] = ay3;

        // derivative of positions = velocities
        dz[6]  = vx1;
        dz[7]  = vy1;
        dz[8]  = vx2;
        dz[9]  = vy2;
        dz[10] = vx3;
        dz[11] = vy3;

        return dz;
    };

    pp::vector zinit = {
         0.4662036850,  0.4323657300,
         0.4662036850,  0.4323657300,
        -0.9324073700, -0.8647314600,

         0.9700043600, -0.2430875300,
        -0.9700043600,  0.2430875300,
         0.0000000000,  0.0000000000
    };

    double T8 = 6.32591398;
    double tmax8 = T8;

    auto [t8, z8] =
        ode::driver(threebody, 0.0, tmax8, zinit,
                    0.01, 1e-5, 1e-5, 0.05);

    std::ofstream eight_out("eight.data");
    eight_out << "# t x1 y1 x2 y2 x3 y3\n";

    for (size_t i = 0; i < t8.size(); ++i) {
        eight_out
            << t8[i] << " "
            << z8[i][6] << " " << z8[i][7] << " "
            << z8[i][8] << " " << z8[i][9] << " "
            << z8[i][10] << " " << z8[i][11] << "\n";
    }

    return 0;

}