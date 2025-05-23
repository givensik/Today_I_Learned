import numpy as np
import random
from math import gcd
from collections import Counter
def egcd(a, b):
    """유클리드 호제법으로 최대공약수(GCD)와 베주 항등식의 계수를 계산합니다."""
    if a == 0:
        return b, 0, 1
    else:
        g, y, x = egcd(b % a, a)
        return g, x - (b // a) * y, y

def mod_matrix_inverse(matrix, modulus=26):
    """
    주어진 정수 행렬에 대해 모듈러 역행렬을 계산합니다.
    """
    matrix = np.array(matrix)
    n = matrix.shape[0]

    # 행렬식과 그 모듈러 역수
    det = int(round(np.linalg.det(matrix))) % modulus
    det_inv = modinv(det, modulus)

    if det_inv is None:
        raise ValueError("역행렬이 존재하지 않습니다 (행렬식과 모듈로가 서로소가 아님).")

    # 여인수 행렬 계산
    cofactors = np.zeros((n, n), dtype=int)
    for i in range(n):
        for j in range(n):
            minor = np.delete(np.delete(matrix, i, axis=0), j, axis=1)
            cofactors[i, j] = ((-1) ** (i + j)) * int(round(np.linalg.det(minor)))

    adjugate = cofactors.T % modulus
    inverse_matrix = (det_inv * adjugate) % modulus
    return inverse_matrix.astype(int)


def modinv(a, m):
    """모듈러 역수를 계산합니다."""
    g, x, y = egcd(a, m)
    if g != 1:
        return None  # 역수가 존재하지 않음
    else:
        return x % m

def hill_cipher_encrypt(plaintext, key): 
    key_matrix = np.array(key) # make array to matrix
    key_size = key_matrix.shape[0] 

    # if key_size != 3:
    #     raise ValueError("키 행렬은 3x3이어야 합니다.") # key should be 3 * 3 matrix

    plaintext = ''.join(filter(str.isalpha, plaintext)).upper()
    padding_length = key_size - (len(plaintext) % key_size)
    if padding_length != key_size:
        plaintext += 'X' * padding_length

    blocks = [plaintext[i:i+key_size] for i in range(0, len(plaintext), key_size)]

    ciphertext = ''
    for block in blocks:
        numerical_block = [ord(char) - ord('A') for char in block]
        encrypted_block = np.dot(key_matrix, numerical_block) % 26
        encrypted_chars = [chr(int(num) + ord('A')) for num in encrypted_block]
        ciphertext += ''.join(encrypted_chars)

    return ciphertext

def hill_cipher_decrypt(ciphertext, key):
    key_matrix = np.array(key)
    key_size = key_matrix.shape[0]

    ciphertext = ''.join(filter(str.isalpha, ciphertext)).upper()

    det = int(round(np.linalg.det(key_matrix))) % 26
    det_inv = modinv(det, 26)

    if det_inv is None:
        raise ValueError("키 행렬이 가역 행렬이 아닙니다 (역행렬이 존재하지 않음).")

    # 여인수 행렬 계산 (일반 n x n 지원)
    n = key_size
    C = np.zeros((n, n), dtype=int)
    for i in range(n):
        for j in range(n):
            minor = np.delete(np.delete(key_matrix, i, axis=0), j, axis=1)
            C[i, j] = ((-1) ** (i + j)) * int(round(np.linalg.det(minor)))

    adjugate_matrix = C.T % 26
    inverse_matrix = (det_inv * adjugate_matrix) % 26
    inverse_matrix = inverse_matrix.astype(int)

    blocks = [ciphertext[i:i+key_size] for i in range(0, len(ciphertext), key_size)]
    plaintext = ''
    for block in blocks:
        numerical_block = [ord(char) - ord('A') for char in block]
        decrypted_block = np.dot(inverse_matrix, numerical_block) % 26
        decrypted_chars = [chr(int(num) + ord('A')) for num in decrypted_block]
        plaintext += ''.join(decrypted_chars)

    return plaintext

def generate_3x3_key():
    while True:
        # 3x3 행렬을 랜덤하게 생성합니다. (0-25 사이의 정수)
        key_matrix = [[random.randint(0, 25) for _ in range(3)] for _ in range(3)]
        key_matrix = np.array(key_matrix)

        # 행렬식을 계산합니다.
        determinant = int(round(np.linalg.det(key_matrix))) % 26

        # 행렬식이 음수일 경우, 26을 더해 양수로 만듭니다.
        if determinant < 0:
            determinant += 26

        # 행렬식이 26과 서로소인지 확인합니다.
        if gcd(determinant, 26) == 1:
            return key_matrix.tolist()  # 유효한 키를 찾으면 반환합니다.

def generate_5x5_key():
    while True:
        # 3x3 행렬을 랜덤하게 생성합니다. (0-25 사이의 정수)
        key_matrix = [[random.randint(0, 25) for _ in range(5)] for _ in range(5)]
        key_matrix = np.array(key_matrix)

        # 행렬식을 계산합니다.
        determinant = int(round(np.linalg.det(key_matrix))) % 26

        # 행렬식이 음수일 경우, 26을 더해 양수로 만듭니다.
        if determinant < 0:
            determinant += 26

        # 행렬식이 26과 서로소인지 확인합니다.
        if gcd(determinant, 26) == 1:
            return key_matrix.tolist()  # 유효한 키를 찾으면 반환합니다.


# 영어 문자 빈도 (대략적인 값)
english_frequencies = {
    'A': 0.08167, 'B': 0.01492, 'C': 0.02782, 'D': 0.04253, 'E': 0.12702,
    'F': 0.02228, 'G': 0.02015, 'H': 0.06094, 'I': 0.06966, 'J': 0.00153,
    'K': 0.00772, 'L': 0.04025, 'M': 0.02406, 'N': 0.06749, 'O': 0.07507,
    'P': 0.01929, 'Q': 0.00095, 'R': 0.05987, 'S': 0.06327, 'T': 0.09056,
    'U': 0.02758, 'V': 0.00978, 'W': 0.02360, 'X': 0.00150, 'Y': 0.01974,
    'Z': 0.00074
}

def calculate_frequency_score(text):
    """텍스트의 빈도 점수를 계산합니다."""
    text = ''.join(filter(str.isalpha, text)).upper()
    if not text:
        return 0
    frequencies = Counter(text)
    score = 0
    for char, count in frequencies.items():
        score += count * english_frequencies.get(char, 0)
    return score

def attack_hill_cipher(ciphertext, key_size=3, num_attempts=1000):
    """힐 암호 암호문을 공격하여 평문을 추측합니다."""
    best_plaintext = ''
    best_score = float('-inf')

    for _ in range(num_attempts):
        try:
            key = generate_3x3_key() # 가능한 키 생성
            plaintext = hill_cipher_decrypt(ciphertext, key) # 복호화 시도
            score = calculate_frequency_score(plaintext) # 빈도 점수 계산
            # print("score :",score)
            if score > best_score:
                best_score = score
                best_plaintext = plaintext
        except ValueError:
            # 키가 유효하지 않은 경우 (역행렬이 존재하지 않는 경우) 무시하고 다음 키 시도
            continue
        except Exception as e:
            # 다른 예외 발생 시 오류 메시지 출력하고 다음 키 시도
            print(f"오류 발생: {e}")
            continue
    return best_plaintext


if __name__ == "__main__":
    # 예시 암호문
    plaintext = "pay more money"
    key = [[17, 21, 2], [17, 18, 2], [5, 21, 19]]
    cipher = hill_cipher_encrypt(plaintext, key)
    print(cipher)
    print(hill_cipher_decrypt(cipher, key))
    print(attack_hill_cipher(cipher))
    print(mod_matrix_inverse(key))