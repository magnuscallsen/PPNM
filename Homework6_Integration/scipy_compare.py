#!/usr/bin/env python3
"""Comparison with scipy.integrate.quad for Task B.
The script writes a gnuplot-friendly table. If SciPy is unavailable, the file is
still written with NaN entries so `make` and gnuplot do not fail.
"""
import math
from pathlib import Path

rows = [
    (1, "1/sqrt(x)", lambda x: 1/math.sqrt(x), 0.0, 1.0, 2.0),
    (2, "log(x)/sqrt(x)", lambda x: math.log(x)/math.sqrt(x), 0.0, 1.0, -4.0),
    (3, "exp(-x), 0..inf", lambda x: math.exp(-x), 0.0, math.inf, 1.0),
    (4, "exp(-x*x), -inf..inf", lambda x: math.exp(-x*x), -math.inf, math.inf, math.sqrt(math.pi)),
]

try:
    from scipy.integrate import quad
    scipy_available = True
except Exception as e:
    scipy_available = False
    scipy_error = e

class Counter:
    def __init__(self, f):
        self.f = f
        self.calls = 0
    def __call__(self, x):
        self.calls += 1
        return self.f(x)

with open("taskB_scipy_comparison.txt", "w") as out:
    out.write("# id name scipy_calls scipy_value scipy_actual_error scipy_estimated_error\n")
    if scipy_available:
        print("Task B: SciPy comparison using scipy.integrate.quad")
        print("------------------------------------------------------------")
        for i, name, f, a, b, exact in rows:
            c = Counter(f)
            val, err = quad(c, a, b, epsabs=1e-6, epsrel=1e-6)
            actual = abs(val - exact)
            print(f"{name:25s} value={val:.12g} actual_error={actual:.3g} scipy_error={err:.3g} calls={c.calls}")
            out.write(f'{i} "{name}" {c.calls} {val:.16g} {actual:.16g} {err:.16g}\n')
    else:
        print("SciPy is not installed; writing NaN rows to taskB_scipy_comparison.txt.")
        print(scipy_error)
        for i, name, *_ in rows:
            out.write(f'{i} "{name}" NaN NaN NaN NaN\n')

# Merge C++ and SciPy call counts into one table for gnuplot.
cpp_file = Path("taskB_cpp_for_scipy_comparison.txt")
if cpp_file.exists():
    cpp = {}
    with cpp_file.open() as f:
        for line in f:
            if not line.strip() or line.startswith("#"):
                continue
            # Format: id "name" cpp_calls cpp_value cpp_actual_error
            parts = line.split('"')
            idx = int(parts[0].strip())
            name = parts[1]
            rest = parts[2].split()
            cpp_calls = rest[0]
            cpp[idx] = (name, cpp_calls)

    scipy = {}
    with open("taskB_scipy_comparison.txt") as f:
        for line in f:
            if not line.strip() or line.startswith("#"):
                continue
            parts = line.split('"')
            idx = int(parts[0].strip())
            name = parts[1]
            rest = parts[2].split()
            scipy_calls = rest[0]
            scipy[idx] = (name, scipy_calls)

    with open("taskB_cpp_vs_scipy_calls.txt", "w") as out:
        out.write("# id name cpp_calls scipy_calls\n")
        for idx in sorted(cpp):
            name, cpp_calls = cpp[idx]
            scipy_calls = scipy.get(idx, (name, "NaN"))[1]
            out.write(f'{idx} "{name}" {cpp_calls} {scipy_calls}\n')
        print("Wrote taskB_cpp_vs_scipy_calls.txt")
else:
    print("taskB_cpp_for_scipy_comparison.txt not found; run ./main first.")
