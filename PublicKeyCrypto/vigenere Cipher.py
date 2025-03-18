
# SAVETHEQUEEN

# KEYWORD = BLADE

def vigenere_encrypt(plaintext, key):
    # 대문자로 변환
    plaintext = plaintext.upper()
    key = key.upper()
    
    encrypted_message = []
    key_length = len(key)
    
    key_index = 0  # 키의 인덱스
    for char in plaintext:
        print(char)
        if char.isalpha():  # 알파벳인 경우에만 암호화
            # 평문 문자와 키 문자의 인덱스 계산
            p_index = ord(char) - ord('a')
            k_index = ord(key[key_index % key_length]) - ord('a')
            # 암호문 문자 계산
            c_index = (p_index + k_index) % 26
            print("c_index :", c_index)
            encrypted_char = chr(c_index + ord('a'))
            encrypted_message.append(encrypted_char)
            key_index += 1  # 키 인덱스 증가
        else:
            # 알파벳이 아닌 경우는 그대로 추가
            encrypted_message.append(char)
            key_index = 0
    
    return ''.join(encrypted_message)

# 예제 실행
message = ['attack', 'retreat', 'hold', 'current', 'position']
key = "BLADE"
encrypted_message1 = vigenere_encrypt(message[0], key)
encrypted_message2 = vigenere_encrypt(message[1], key)
encrypted_message3 = vigenere_encrypt(message[2], key)
encrypted_message4 = vigenere_encrypt(message[3], key)
encrypted_message5 = vigenere_encrypt(message[4], key)
print("attack :", encrypted_message1,"\nretreat :", encrypted_message2, "\nhold :",encrypted_message3, "\ncurrent :", encrypted_message4, "\nposition :", encrypted_message5)
