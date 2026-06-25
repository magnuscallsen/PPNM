#include "root.h"

int main(){
    cout << fixed << setprecision(10);  // AI

    test("Test 1D: x^2-2=0",
        [](const vec& x){ return vec{x[0]*x[0]-2}; },
        vec{1}); // AIwhave 

    test("Test 2D",
        [](const vec& x){ return vec{x[0]+x[1]-3, x[0]*x[0]+x[1]*x[1]-5}; },
        vec{0.5,2.5}); // AI
    
    cout << "\n We now find minima/minimum of certain functions by finding roots of their analytically found gradients.\n";
    vec r = newton(grad_rosenbrock, vec{0.8,0.8});
    cout << "\nRosenbrock minimum\n";
    print("(x,y)",r);
    cout << "\nf(x,y) = " << rosenbrock(r[0],r[1]) << "\n";

    vector<vec> starts = {{3,2},{-3,3},{-4,-3},{4,-2}}; // AI

    cout << "\nHimmelblau minima\n";
    for(vec s:starts){
        vec h = newton(grad_himmelblau,s);
        print("(x,y)",h);
        cout << "   f(x,y) = " << himmelblau(h[0],h[1]) << "\n";
    }
    hydrogen_demo(); // written by AI

    cout << "\nQuadratic interpolation Newton\n";

    vec q = newton_quadratic(grad_rosenbrock, vec{0.8,0.8});
    print("Rosenbrock quadratic", q);
    cout << "\nf(x,y) = " << rosenbrock(q[0],q[1]) << "\n";

    return 0;
}
