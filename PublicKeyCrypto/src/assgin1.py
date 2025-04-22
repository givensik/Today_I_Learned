import random
import string


# shift cipher function 
def shift_cipher_encrypt(plaintext, key):
    pvalue = plaintext # get plaintext value
    
    output = [] # output variable
    
    for i in pvalue:
        if not i.isalpha(): # if not a alphabet like space 
           output.append(i) # append without changes
           continue
        base = ord('A') if i.isupper() else ord('a') 
        shifted = (ord(i) - base + int(key)) % 26 + base # shift each chracters based on key value
        output.append(chr(shifted))
    
    # array -> string    
    output = "".join(output) 
    return output

def shift_cipher_decrypt(ciphertext, key):
    
    ciphertext = ciphertext
    shift = int(key)
    decrypted = ""
    
    for char in ciphertext:
        if char.isalpha():
            base = ord('A') if char.isupper() else ord('a')
            decrypted += chr((ord(char) - base - shift) % 26 + base)
        else:
            decrypted += char
    # print(decrypted)
    
    return decrypted

def generate_substitution_cipher():
    # original alphabet
    original_alphabet = list(string.ascii_lowercase) # a-z
    shuffled_alphabet = original_alphabet.copy() # create copy of original alphabet
    random.shuffle(shuffled_alphabet) # suffle
    return "".join(shuffled_alphabet)


def substitution_cipher(plaintext, key):
    output = []
    for i in plaintext:
        if  not i.isalpha() : # if each character is not alphabet then append it without change
            output.append(i)
            continue
        # if each character is alphabetic then substitute by shuffled alphabet
        base = ord('A') if i.isupper() else ord('a') # b -> p
        output.append(key[ord(i) - base])
    output = "".join(output)
    return output

def substitution_cipher_decrypt(ciphertext, key):
    
    # 복호화용 역변환 딕셔너리 생성
    decrypt_key_upper = {key[i]: chr(ord('A') + i) for i in range(26)}
    decrypt_key_lower = {key[i].lower(): chr(ord('a') + i) for i in range(26)}

    output = []
    for i in ciphertext:
        if not i.isalpha():
            output.append(i)
            continue
        if i.isupper():
            output.append(decrypt_key_upper.get(i, i))
        else:
            output.append(decrypt_key_lower.get(i, i))

    return "".join(output)

def vigenere_encrypt(plaintext, key):
    
    encrypted_message = []
    key_length = len(key)
    
    key_index = 0  # index of the key
    for char in plaintext:
        if char.isalpha():  # encrypt only alpha characters
            # calculate index of plaintext and key
            p_index = ord(char) - ord('a')
            k_index = ord(key[key_index % key_length]) - ord('a')
            # encrypt by adding each character
            c_index = (p_index + k_index) % 26
            encrypted_char = chr(c_index + ord('a'))
            encrypted_message.append(encrypted_char)
            key_index += 1  # 키 인덱스 증가
        else:
            # if char is not alphabet
            encrypted_message.append(char)
            key_index = 0
    output = ''.join(encrypted_message)
    return output

def vigenere_decrypt(ciphertext, key):
    
    decrypted_message = []
    key_length = len(key)
    key_index = 0

    for char in ciphertext:
        if char.isalpha():
            k_index = ord(key[key_index % key_length]) - ord('a')
            
            if char.isupper():
                c_index = ord(char) - ord('A')
                p_index = (c_index - k_index + 26) % 26
                decrypted_char = chr(p_index + ord('A'))
            else:
                c_index = ord(char) - ord('a')
                p_index = (c_index - k_index + 26) % 26
                decrypted_char = chr(p_index + ord('a'))
            
            decrypted_message.append(decrypted_char)
            key_index += 1
        else:
            decrypted_message.append(char)
            # 암호화 함수와 통일할 경우: key_index += 1 유지하거나 그대로 둬도 됨

    return "".join(decrypted_message)