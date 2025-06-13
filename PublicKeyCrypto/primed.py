#!/usr/bin/env python3
import math

def euler_phi(n: int) -> int:
    """오일러 토티언 함수 φ(n) 계산"""
    result, temp = n, n
    p = 2
    while p*p <= temp:
        if temp % p == 0:
            while temp % p == 0:
                temp //= p
            result -= result // p
        p += 1
    if temp > 1:
        result -= result // temp
    return result

def prime_factors(n: int) -> set:
    """n의 소인수 집합 반환"""
    factors, temp = set(), n
    p = 2
    while p*p <= temp:
        if temp % p == 0:
            factors.add(p)
            while temp % p == 0:
                temp //= p
        p += 1
    if temp > 1:
        factors.add(temp)
    return factors

def is_primitive_root(g: int, m: int) -> bool:
    """g가 mod m 의 원시근인지 여부"""
    if math.gcd(g, m) != 1:
        return False
    phi = euler_phi(m)
    for q in prime_factors(phi):
        if pow(g, phi // q, m) == 1:
            return False
    return True

def print_table(m: int):
    # 칸 너비 결정 (숫자들이 정렬되도록)
    w = max(len(str(m-1)), 3)
    # 헤더
    header = " b ".rjust(w+2) + " | " + " ".join(f"{i:>{w}}" for i in range(1, m)) + " | PR"
    sep    = "-" * len(header)
    print(header)
    print(sep)
    # 각 b 에 대해 행 출력
    for b in range(1, m):
        row = [pow(b, i, m) for i in range(1, m)]
        mark = "*" if is_primitive_root(b, m) else " "
        print(f"{b:>{w}}    | " + " ".join(f"{v:>{w}}" for v in row) + f" |  {mark}")

if __name__ == "__main__":
    try:
        m = int(input("모듈로 값 m(소수)을 입력하세요: "))
        if m < 2:
            raise ValueError
    except ValueError:
        print("잘못된 입력입니다. 정수 m ≥ 2를 입력하세요.")
        exit(1)

    print()
    print_table(m)

