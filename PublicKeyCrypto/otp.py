def char_to_bin(c):
    
    return format(ord(c), '08b')

def get_control_char_info(val):
    
    control_chars = {
        0: 'NUL', 1: 'SOH', 2: 'STX', 3: 'ETX', 4: 'EOT',
        5: 'ENQ', 6: 'ACK', 7: 'BEL', 8: 'BS',  9: 'HT',
        10: 'LF', 11: 'VT', 12: 'FF', 13: 'CR', 14: 'SO',
        15: 'SI', 16: 'DLE',17: 'DC1',18: 'DC2',19: 'DC3',
        20: 'DC4',21: 'NAK',22: 'SYN',23: 'ETB',24: 'CAN',
        25: 'EM', 26: 'SUB',27: 'ESC',28: 'FS', 29: 'GS',
        30: 'RS', 31: 'US', 127: 'DEL'
    }
    name = control_chars.get(val, chr(val))
    return f"{name}('\\x{val:02x}')"



def xor_encrypt_verbose_ascii(plaintext, key):
    print(f"\n XOR 과정 (총 {len(plaintext)} 문자):\n")
    result_list = []
    cipher_text = ''

    for i, (p, k) in enumerate(zip(plaintext, key), 1):
        p_bin = char_to_bin(p)
        k_bin = char_to_bin(k)
        xor_val = ord(p) ^ ord(k)
        xor_bin = format(xor_val, '08b')
        xor_char_info = get_control_char_info(xor_val)
        cipher_char = chr(xor_val)

        print(f"{p_bin} ({p}, {ord(p)}) xor {k_bin} ({k}, {ord(k)}) -> {xor_bin} ({xor_char_info})")
        result_list.append(xor_char_info)
        cipher_text += cipher_char

    print("\n RESULT :")
    print(', '.join(result_list))

    print("\n Only ASCII characters:")
    print(cipher_text)
    return cipher_text


if __name__ == "__main__":
    # 예시 입력
    plaintext = "LOVE CRYPTOGRAPHY"
    key       = "GOOD APPRECIATION"
    print(f"[Key]: {repr(key)}")
    cipher = xor_encrypt_verbose_ascii(plaintext, key)
    xor_encrypt_verbose_ascii(cipher, key)
