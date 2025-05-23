def shift_cipher_decrypt(ciphertext, shift):
    decrypted = ""
    for char in ciphertext:
        if char.isalpha():
            # if Uppder case
            if char.isupper():
                decrypted += chr((ord(char) - shift - 65) % 26 + 65)
            # if lower case
            else:
                decrypted += chr((ord(char) - shift - 97) % 26 + 97)
        else:
            decrypted += char  # is not alphabet -> nothing to change
    return decrypted

def brute_force_shift_cipher(ciphertext):
    output = ""
    for shift in range(26):
        decrypted_message = shift_cipher_decrypt(ciphertext, shift)
        # print(f"Shift {shift}: {decrypted_message}")
        output += "shift " + str(shift) + " : "+ decrypted_message+"\n"
        
    return output


def find_position(matrix, c) :
    for i in range(5):
        for j in range(5):
            # print(" (i,j) : (%d, %d)" %(i,j))
            if matrix[i][j] == c :
                return i, j

def playfair_tech(plaintext, key):
    # 1. create matrix
    matrix = []
    key = key.upper().replace("J","I")
    alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    
    # input key value
    for char in key:
        if char not in matrix and char in alphabet:
            matrix.append(char)
            
    # input alphabet
    for char in alphabet:
        if char not in matrix and char != 'J':
            matrix.append(char)
    
    matrix = [matrix[i*5:(i+1)*5] for i in range(5)]

    # 2. preprocess the plaintext
    plaintext = plaintext.upper().replace("J", "I").replace(" ", "") # make text to upper case and remove space
    length = len(plaintext)
    
    processed = []
    for i in range(length):
        #  1. 같은 문자가 연속으로 나오면 지금까지 안나온 알파벳을 추가한다.
        if len(processed) == 0:
            processed.append(plaintext[i])
        elif len(processed)%2 == 1 and processed[len(processed)-1] == plaintext[i] : # 홀수이면서 이게 plain text랑 같으면
            for c in alphabet :
                if c not in processed:
                    processed.append(c)
                    break
            processed.append(plaintext[i])
        else:
            processed.append(plaintext[i])
    
    if len(processed)%2 == 1 : # 결과의 길이가 홀수인 경우
        for c in alphabet :
            if c not in processed:
                processed.append(c)
                break
    # print(processed)    
    
    # encrypt
    for i in range(0, len(processed) -1, 2):
        ay, ax = find_position(matrix, processed[i])
        by, bx = find_position(matrix, processed[i+1])
        
        if ay == by :
            processed[i] = matrix[ay][(ax + 1)%5]
            processed[i+1] = matrix[ay][(bx + 1)%5]
        elif ax == bx :
            processed[i] = matrix[(ay+1)%5][ax]
            processed[i+1] = matrix[(by+1)%5][ax]
        else :
            processed[i] = matrix[ay][bx]
            processed[i+1] = matrix[by][ax]
    return processed

def playfair_decrypt(ciphertext, key):
    # 1. create matrix
    matrix = []
    key = key.upper().replace("J", "I")
    alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    
    for char in key:
        if char not in matrix and char in alphabet:
            matrix.append(char)
    for char in alphabet:
        if char not in matrix and char != 'J':
            matrix.append(char)
    
    matrix = [matrix[i*5:(i+1)*5] for i in range(5)]

    # 2. convert input if it's a list of chars
    if isinstance(ciphertext, list):
        ciphertext = ''.join(ciphertext)
    ciphertext = ciphertext.upper().replace("J", "I").replace(" ", "")
    
    # 3. decrypt in pairs
    decrypted = []
    for i in range(0, len(ciphertext), 2):
        a, b = ciphertext[i], ciphertext[i+1]
        ay, ax = find_position(matrix, a)
        by, bx = find_position(matrix, b)
        
        if ay == by:
            decrypted.append(matrix[ay][(ax - 1) % 5])
            decrypted.append(matrix[by][(bx - 1) % 5])
        elif ax == bx:
            decrypted.append(matrix[(ay - 1) % 5][ax])
            decrypted.append(matrix[(by - 1) % 5][ax])
        else:
            decrypted.append(matrix[ay][bx])
            decrypted.append(matrix[by][ax])
    
    return ''.join(decrypted)

# 가장 많은 문자 -> 첫번째로 가게
def find_frequency(cipher):
    alphabet_fr = [0] * 26 # 입력 문자 빈도
    if cipher.islower() :
        alphabet = "abcdefghijklmnopqrstuvwxyz"
    else :
        alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    
    for i in cipher :
        # print("i : ",i)
        for j in range(26):
            # print("alphabet : ",alphabet[j])
            if alphabet[j] == i :      
                # print("j : %d"%(j))
                alphabet_fr[j] += 1
                break
            
    # print("alphabet_fr :", alphabet_fr)
    l = len(cipher)
    result = []
    for i in range(l):
        max_value = max(alphabet_fr)
        max_index = alphabet_fr.index(max_value)
        if alphabet[max_index] in result:
            continue
        result.append(alphabet[max_index])
        alphabet_fr[max_index] = 0
    return result
                
# https://github.com/first20hours/google-10000-english/blob/master/google-10000-english-no-swears.txt
# brute force attack using frequency analysis
def bf_freq_analysis(cipher):
    with open("word.txt", "r") as f:
        common_words = [line.strip() for line in f]
        
    if cipher.isupper() :
        frequency_list = ['E', 'T', 'A', 'O', 'I', 'N', 'S', 'R', 'H', 'L', 'D', 'C', 'U', 'M', 'W', 'F', 'G', 'P', 'Y', 'B', 'V', 'K', 'X', 'Q', 'J', 'Z']    
    elif cipher.islower():
        frequency_list = ['e', 't', 'a', 'o', 'i', 'n', 's', 'r', 'h', 'l', 'd', 'c', 'u', 'm', 'w', 'f', 'g', 'p', 'y', 'b', 'v', 'k', 'x', 'q', 'j', 'z']
        
    # balloon -> l o a b n
    cipher_freq = find_frequency(cipher)
    
    # E를 가장 많이 나온 애로 생각하기 -> 그거로 key 값 찾아서 shift 해보기 -> 아니면 다음 빈도수
    list = []
    cnt = 0
    for j in cipher_freq:
        for c in frequency_list :
            key = abs(ord(c) - ord(j))
            decrypt = shift_cipher_decrypt(cipher, key)
            if decrypt in common_words :
                list.append(decrypt)
                cnt += 1
                break
        if cnt != 0 :
            break
    return list
    
    


if __name__ == "__main__":
    # 예시 암호문
    ciphertext = "balloon"
    brute_force_shift_cipher(ciphertext)
    
    key = "test"
    plaintext = "cryptography" # dq wr br lo ho dw tz ht do gr wb
    cipher = playfair_tech(plaintext, key)
    print("".join(cipher))
    output = decrypt_playfair(cipher, key)
    print("".join(output))
    
    bf_freq_analysis(ciphertext)
    