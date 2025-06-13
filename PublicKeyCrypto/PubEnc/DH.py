# DH.py
import secrets
from Crypto.Util.number import getRandomNBitInteger
from sympy import randprime, primitive_root

# 소수 판별 함수
def is_prime(n):
    if n <= 1: return False
    if n <= 3: return True
    if n % 2 == 0 or n % 3 == 0: return False
    i = 5
    while i * i <= n:
        if n % i == 0 or n % (i + 2) == 0:
            return False
        i += 6
    return True

# 원시근 찾기
def find_primitive_root(p):
    # 일반적으로 2 또는 5가 primitive root인 경우가 많음
    for g in range(2, p):
        if pow(g, (p - 1) // 2, p) != 1:  # 단순 검사
            return g
    raise Exception("No primitive root found.")

# 시저 암호화
def shift_encrypt(text, shift):
    result = ""
    for c in text:
        if c.islower():
            result += chr((ord(c) - 97 + shift) % 26 + 97)
        elif c.isupper():
            result += chr((ord(c) - 65 + shift) % 26 + 65)
        else:
            result += c
    return result

def shift_decrypt(text, shift):
    return shift_encrypt(text, -shift)

def generate_dh_parameters(key_bits=16):
    low = 2 ** (key_bits - 1)
    high = 2 ** key_bits
    p = randprime(low, high)
    g = primitive_root(p)
    return p, g


# 개인키, 공개키 생성
def generate_dh_keypair(p, g, bits=256):
    private_key = getRandomNBitInteger(bits)
    public_key = pow(g, private_key, p)
    return private_key, public_key

# 공유 비밀키 계산
def compute_shared_key(other_public, own_private, p):
    return pow(other_public, own_private, p)

from sympy.ntheory import discrete_log

# attacker attempt
def attacker_attempt(p, g, A, B, cipher):
    try:
        # Alice의 비밀키 a를 찾는다 (작은 p에서만 가능)
        a = discrete_log(p, A, g)  # A = g^a mod p 를 역산
        shared = pow(B, a, p)
        shift = shared % 26
        decrypted = shift_decrypt(cipher, shift)
        
        return {
            "guessed_secret": shared,
            "shift": shift,
            "decrypted": decrypted
        }
    except Exception as e:
        return {"error": str(e)}


def run_dh_simulation_gui(key_bits, plaintext):
    p, g = generate_dh_parameters(key_bits)
    a_priv, A = generate_dh_keypair(p, g)
    b_priv, B = generate_dh_keypair(p, g)

    s = compute_shared_key(B, a_priv, p)
    cipher = shift_encrypt(plaintext, s)
    plain = shift_decrypt(cipher, s)

    return {
        "p": p,
        "g": g,
        "A": A,
        "B": B,
        "shared": s,
        "cipher": cipher,
        "plain": plain
    }


# Diffie–Hellman & 시저 테스트 함수
def run_dh_simulation(p, plaintext):
    if not is_prime(p):
        print("[!] Error: p is not prime")
        return

    g = find_primitive_root(p)
    print(f"[+] Prime p: {p}")
    print(f"[+] Primitive root g: {g}")

    a, A = generate_dh_keypair(p, g)
    b, B = generate_dh_keypair(p, g)

    print(f"Alice 공개키 A = {A}")
    print(f"Bob 공개키 B = {B}")

    s = compute_shared_key(B, a, p)
    print(f"[+] 공유 비밀키: {s}")

    cipher = shift_encrypt(plaintext, s)
    print(f"[+] 암호문: {cipher}")
    plain = shift_decrypt(cipher, s)
    print(f"[+] 복호문: {plain}")

if __name__ == "__main__":
    run_dh_simulation(23, "HelloWorld")