from math import floor
from typing import List, Tuple

import numpy as np
from ortools.linear_solver import pywraplp

def first_n_primes(n: int) -> List[int]:
    """ Simple sieve to get the first n primes. """
    primes = []
    limit = max(200, n * 20)
    sieve = [True] * limit
    sieve[0:2] = [False, False]
    for i in range(2, limit):
        if sieve[i]:
            primes.append(i)
            if len(primes) >= n:
                break
            for j in range(i * i, limit, i):
                sieve[j] = False
    return primes


def compute_factorial_valuations(primes: List[int]) -> List[List[int]]:
    """
    For each prime p_i and each m up to p_n, compute v_{p_i}(m!).
    Returns v[i][m] = exponent of primes[i] in m!.
    """
    n = len(primes)
    M = primes[-1]
    v = [[0] * (M + 1) for _ in range(n)]
    for i, p in enumerate(primes):
        for m in range(2, M + 1):
            t, cnt = m, 0
            while t:
                t //= p
                cnt += t
            v[i][m] = cnt
    return v


def solve_primorial_ratio(n: int) -> Tuple[List[int], List[int], int]:
    """
    Solve P_n = p1*p2*...*pn = ( ∏ m!^u[m] ) / ( ∏ m!^d[m] )
    minimizing sum(m * (u[m] + d[m])).
    Returns (u_list, d_list, objective_value).
    """
    primes = first_n_primes(n)
    M = primes[-1]

    v = compute_factorial_valuations(primes)

    solver = pywraplp.Solver.CreateSolver('CBC')
    if not solver:
        raise RuntimeError("CBC solver unavailable")

    # Create integer variables u[m], d[m] for m = 2..M
    u = {}
    d = {}
    UB = n
    for m in range(2, M + 1):
        u[m] = solver.IntVar(0, UB, f"u[{m}]")
        d[m] = solver.IntVar(0, UB, f"d[{m}]")

    # Coverage constraints: for each prime i
    for i in range(n):
        ct = solver.RowConstraint(1, 1, f"cover_p{i}")
        for m in range(2, M + 1):
            ct.SetCoefficient(u[m],  v[i][m])
            ct.SetCoefficient(d[m], -v[i][m])

    # Objective: minimize sum of m*(u[m] + d[m])
    obj = solver.Objective()
    for m in range(2, M + 1):
        obj.SetCoefficient(u[m], m)
        obj.SetCoefficient(d[m], m)
    obj.SetMinimization()

    status = solver.Solve()
    if status != pywraplp.Solver.OPTIMAL:
        raise RuntimeError("No optimal solution found")

    # Extract solution, initializing zeros for indices < 2
    u_sol = [0] * (M + 1)
    d_sol = [0] * (M + 1)
    for m in range(2, M + 1):
        u_sol[m] = int(u[m].solution_value())
        d_sol[m] = int(d[m].solution_value())

    return u_sol, d_sol, int(obj.Value())


if __name__ == "__main__":
    n = 10  # Change this to compute P_n for any n
    while True:
        n = int(input('n='))
        if n <= 0:
            break
        u_sol, d_sol, cost = solve_primorial_ratio(n)
        primes = first_n_primes(n)
        M = primes[-1]

        print(f"Minimum total sum of arguments = {cost}")
        # print("Numerator factorials:")
        # for m in range(2, M + 1):
        #     if u_sol[m] > 0:
        #         print(f"  {m}!  × {u_sol[m]}")
        # print("Denominator factorials:")
        # for m in range(2, M + 1):
        #     if d_sol[m] > 0:
        #         print(f"  {m}!  × {d_sol[m]}")

        string = ""
        for m in range(M, 1, -1):
            for _ in range(u_sol[m]):
                string += f'{m}! '
        string += '/ '
        for m in range(M, 1, -1):
            for _ in range(d_sol[m]):
                string += f'{m}! '
        print(string)
