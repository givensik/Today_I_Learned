#!/usr/bin/env python3
import sys
import math
import secrets

# --- 소수 판정 ---
def is_prime(n: int) -> bool:
    if n <= 1:
        return False
    if n <= 3:
        return True
    if n % 2 == 0 or n % 3 == 0:
        return False
    i = 5
    while i * i <= n:
        if n % i == 0 or n % (i + 2) == 0:
            return False
        i += 6
    return True

# --- 오일러 토티언 & 소인수분해 ---
def euler_phi(n: int) -> int:
    result, temp = n, n
    p = 2
    while p * p <= temp:
        if temp % p == 0:
            while temp % p == 0:
                temp //= p
            result -= result // p
        p += 1
    if temp > 1:
        result -= result // temp
    return result

def prime_factors(n: int) -> set:
    factors, temp = set(), n
    p = 2
    while p * p <= temp:
        if temp % p == 0:
            factors.add(p)
            while temp % p == 0:
                temp //= p
        p += 1
    if temp > 1:
        factors.add(temp)
    return factors

# --- 원시근 검증 ---
def is_primitive_root(g: int, p: int) -> bool:
    if not (1 < g < p) or math.gcd(g, p) != 1:
        return False
    phi = euler_phi(p)
    for q in prime_factors(phi):
        if pow(g, phi // q, p) == 1:
            return False
    return True

# --- ElGamal 키 생성 ---
def elgamal_keygen(p: int, g: int) -> tuple[int,int]:
    x = secrets.randbelow(p - 2) + 1  # private key in [1, p-2]
    y = pow(g, x, p)                  # public key
    return x, y

# --- ElGamal 암호화 ---
def elgamal_encrypt(m: int, p: int, g: int, y: int) -> tuple[int,int]:
    k = secrets.randbelow(p - 2) + 1
    c1 = pow(g, k, p)
    c2 = (m * pow(y, k, p)) % p
    return c1, c2

# --- 모듈러 역원 ---
def modinv(a: int, m: int) -> int:
    return pow(a, -1, m)

# --- ElGamal 복호화 ---
def elgamal_decrypt(c1: int, c2: int, p: int, x: int) -> int:
    s = pow(c1, x, p)
    s_inv = modinv(s, p)
    return (c2 * s_inv) % p

# --- 메인 실행 ---
if __name__ == "__main__":
    try:
        p = int(input("소수 p를 입력하세요: "))
        g = int(input("원시근 g를 입력하세요: "))
    except ValueError:
        print("오류: 정수를 입력하세요.")
        sys.exit(1)

    if not is_prime(p):
        print(f"오류: {p}는 소수가 아닙니다.")
        sys.exit(1)
    if not is_primitive_root(g, p):
        print(f"오류: {g}는 모듈로 {p}의 원시근이 아닙니다.")
        sys.exit(1)

    x, y = elgamal_keygen(p, g)
    print("\n=== 키 생성 ===")
    print(f"비밀키 x = {x}")
    print(f"공개키 y = g^x mod p = {y}\n")

    try:
        m = int(input(f"암호화할 메시지 m을 입력하세요 (1 ≤ m < {p}): "))
    except ValueError:
        print("오류: 정수 메시지를 입력하세요.")
        sys.exit(1)
    if not (1 <= m < p):
        print(f"오류: 메시지 m은 1 이상 {p-1} 이하여야 합니다.")
        sys.exit(1)

    c1, c2 = elgamal_encrypt(m, p, g, y)
    print("=== 암호문 ===")
    print(f"c1 = {c1}, c2 = {c2}\n")

    m_rec = elgamal_decrypt(c1, c2, p, x)
    print("=== 복호화 ===")
    print(f"복호문 m = {m_rec}")
