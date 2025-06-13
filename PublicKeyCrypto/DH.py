#!/usr/bin/env python3
import sys
import math
import secrets

# --- 소수 판정 함수 ---
def is_prime(n: int) -> bool:
    """n이 소수인지 여부를 판정하는 함수 (6k±1 최적화)"""
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

# --- 오일러 토티언, 소인수분해 ---
def euler_phi(n: int) -> int:
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

# --- 원시근 검사 ---
def is_primitive_root(g: int, p: int) -> bool:
    if not (1 < g < p) or math.gcd(g, p) != 1:
        return False
    phi = euler_phi(p)
    for q in prime_factors(phi):
        if pow(g, phi // q, p) == 1:
            return False
    return True

# --- Diffie–Hellman 키 교환 ---
def derive_shared_secret(p: int, g: int, a: int, b: int) -> tuple[int,int,int,int]:
    A = pow(g, a, p)
    B = pow(g, b, p)
    s1 = pow(B, a, p)
    s2 = pow(A, b, p)
    assert s1 == s2, "키 교환 불일치"
    return A, B, s1

# --- Shift Cipher ---
def shift_encrypt(text: str, shift: int) -> str:
    result = []
    for ch in text:
        if 'A' <= ch <= 'Z':
            result.append(chr((ord(ch)-65 + shift) % 26 + 65))
        elif 'a' <= ch <= 'z':
            result.append(chr((ord(ch)-97 + shift) % 26 + 97))
        else:
            result.append(ch)
    return ''.join(result)

def shift_decrypt(text: str, shift: int) -> str:
    return shift_encrypt(text, -shift)

# --- 메인 ---
if __name__ == "__main__":
    try:
        p = int(input("소수 p를 입력하세요: "))
        g = int(input("원시근 g를 입력하세요: "))
    except ValueError:
        print("잘못된 입력입니다. 정수를 입력하세요.")
        sys.exit(1)

    # p가 소수인지 확인
    if not is_prime(p):
        print(f"오류: {p}는 소수가 아닙니다.")
        sys.exit(1)
    # g가 원시근인지 확인
    if not is_primitive_root(g, p):
        print(f"오류: {g}는 모듈로 {p}의 원시근이 아닙니다.")
        sys.exit(1)

    plaintext = input("암호화할 평문을 입력하세요: ")

    # 비밀키 랜덤 생성
    a = secrets.randbelow(p-3) + 2  # [2, p-2]
    b = secrets.randbelow(p-3) + 2

    # 공개키와 공유 비밀키 계산
    A, B, s = derive_shared_secret(p, g, a, b)

    print("\n=== Diffie–Hellman 결과 ===")
    print(f"p = {p}, g = {g}")
    print(f"a = {a}, b = {b}")
    print(f"A = g^a mod p = {A}")
    print(f"B = g^b mod p = {B}")
    print(f"공유 비밀키 s = {s}\n")

    # Shift Cipher 적용
    cipher = shift_encrypt(plaintext, s)
    recovered = shift_decrypt(cipher, s)

    print("=== Shift Cipher ===")
    print("원문:", plaintext)
    print("암호문:", cipher)
    print("복호문:", recovered)
