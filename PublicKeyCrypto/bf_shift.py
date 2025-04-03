def shift_cipher_decrypt(ciphertext, shift):
    decrypted = ""
    for char in ciphertext:
        if char.isalpha():
            # 대문자 처리
            if char.isupper():
                decrypted += chr((ord(char) - shift - 65) % 26 + 65)
            # 소문자 처리
            else:
                decrypted += chr((ord(char) - shift - 97) % 26 + 97)
        else:
            decrypted += char  # 알파벳이 아닌 문자는 그대로 유지
    return decrypted

def brute_force_shift_cipher(ciphertext):
    for shift in range(26):
        decrypted_message = shift_cipher_decrypt(ciphertext, shift)
        print(f"Shift {shift}: {decrypted_message}")

# 예시 암호문
ciphertext = "SQDYMZK"
brute_force_shift_cipher(ciphertext)