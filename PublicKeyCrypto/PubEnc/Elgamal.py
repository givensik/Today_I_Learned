# elgamal.py
import secrets
from sympy import randprime, primitive_root
import random

def generate_keys(bits):
    p = randprime(2**(bits-1), 2**bits)
    g = primitive_root(p)
    d = random.randint(2, p - 2)  # 비밀키
    e = pow(g, d, p)              # 공개키
    return {'p': p, 'g': g, 'e': e}, d

# ElGamal 키쌍 생성
def generate_elgamal_keys(key_bits=16):
    low = 2 ** (key_bits - 1)
    high = 2 ** key_bits
    p = randprime(low, high)
    g = primitive_root(p)
    x = secrets.randbelow(p - 2) + 1  # Alice 개인키
    e = pow(g, x, p)                  # 공개키
    return {"p": p, "g": g, "x": x, "e": e}

# ElGamal 암호화 (Bob)
def encrypt_elgamal(p, g, e, message):
    k = secrets.randbelow(p - 2) + 1
    y1 = pow(g, k, p)
    y2 = [(ord(char) * pow(e, k, p)) % p for char in message]
    return y1, y2

# ElGamal 복호화 (Alice)
def decrypt_elgamal(p, x, y1, y2):
    s = pow(y1, x, p)
    s_inv = pow(s, -1, p)  # 역원
    plaintext = "".join([chr((c * s_inv) % p) for c in y2])
    return plaintext


# 암호화 함수
def encrypt(pk, message):
    p, g, e = pk['p'], pk['g'], pk['e']
    m = int.from_bytes(message.encode(), 'big')
    if m >= p:
        raise ValueError("Message too large for the selected prime.")
    k = random.randint(2, p - 2)
    c1 = pow(g, k, p)
    c2 = (m * pow(e, k, p)) % p
    return c1, c2

# 복호화 함수
def decrypt(ciphertext, private_key, p):
    c1, c2 = ciphertext
    s = pow(c1, private_key, p)
    s_inv = pow(s, -1, p)
    m = (c2 * s_inv) % p
    return m.to_bytes((m.bit_length() + 7) // 8, 'big').decode(errors='ignore')

# Elgamal Attacker code
def attacker_try_bruteforce(p, g, e, ciphertext, max_try=100000):
    for d_guess in range(1, max_try):
        if pow(g, d_guess, p) == e:
            try:
                decrypted = decrypt(ciphertext, d_guess, p)
                return f"[+] Found d = {d_guess}, message = {decrypted}"
            except:
                continue
    return "[-] Failed to recover the message with brute-force."


# 시뮬레이션 테스트용 함수
if __name__ == "__main__":
    msg = "Hello"
    keys = generate_elgamal_keys(16)
    y1, y2 = encrypt_elgamal(keys["p"], keys["g"], keys["e"], msg)
    decrypted = decrypt_elgamal(keys["p"], keys["x"], y1, y2)

    print("[+] Message:", msg)
    print("[+] Encrypted:", (y1, y2))
    print("[+] Decrypted:", decrypted)
