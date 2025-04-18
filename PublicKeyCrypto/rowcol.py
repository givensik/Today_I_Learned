import math

def encrypt_row_column_transposition(plain_text, key):
    # 1. 데이터 준비 (공백 제거, 대문자 변환)
    processed_text = ''.join(plain_text.upper().split())
    column_count = len(key)
    row_count = math.ceil(len(processed_text) / column_count)

    # 2. 평문 패딩 (Padding)
    required_padding = row_count * column_count
    if len(processed_text) < required_padding:
        processed_text += 'X' * (required_padding - len(processed_text))

    # 3. 행렬 생성
    matrix = []
    text_index = 0
    for _ in range(row_count):
        matrix_row = [char for char in processed_text[text_index:text_index + column_count]]
        matrix.append(matrix_row)
        text_index += column_count

    # 4. 열 순서 결정 (키 기반 정렬)
    key_with_index = sorted((num, index) for index, num in enumerate(key))
    column_order = [index for _, index in key_with_index]

    # 5. 암호화 수행
    cipher_text = ''
    for col in column_order:
        for row in range(row_count):
            cipher_text += matrix[row][col]
    return cipher_text


def decrypt_row_column_transposition(cipher_text, key):

    column_count = len(key)
    row_count = math.ceil(len(cipher_text) / column_count)

    # 1. 매트릭스 생성 (암호문을 기반으로)
    matrix = [['' for _ in range(column_count)] for _ in range(row_count)]
    text_index = 0

    # 2. 열 순서 결정 (암호화에 사용된 키의 역순)
    key_with_index = sorted((num, index) for index, num in enumerate(key))
    column_order = [index for _, index in key_with_index]

    # 3. 매트릭스 채우기 (열 순서에 따라)
    for col in column_order:
        for row in range(row_count):
            matrix[row][col] = cipher_text[text_index]
            text_index += 1

    # 4. 평문 복원
    plain_text = ''
    for row in range(row_count):
        for col in range(column_count):
            plain_text += matrix[row][col]

    # 5. 패딩 제거 (X 제거)
    plain_text = plain_text.rstrip('X')

    return plain_text

def get_key_from_user():
    while True:
        try:
            key_str = input("input key array : ")
            key = [int(x) for x in key_str.split(' ')]
            # 키의 유효성 검사 (예: 양의 정수인지, 중복된 값이 없는지 등)를 추가할 수 있습니다.
            if len(set(key)) != len(key) or any(x <= 0 for x in key):
                print("잘못된 키 형식입니다. 양의 정수이고 중복된 값이 없어야 합니다.")
                continue
            return key
        except ValueError:
            print("잘못된 키 형식입니다. 정수만 입력하세요.")



message = input("input message : ")
encryption_key = get_key_from_user()
encryption_rounds = int(input("input frequency : "))

# message = "Cryptography and mathematics are the best"
# encryption_key = [9, 4, 5, 1, 3, 2, 8, 6, 7]
# encryption_rounds = 3  # 암호화 반복 횟수




# 암호화
encrypted_message = message
for _ in range(encryption_rounds):
    encrypted_message = encrypt_row_column_transposition(encrypted_message, encryption_key)
    print("%d :" %(_), encrypted_message)

print("Encrypt message :", encrypted_message)

# 복호화
decrypted_message = encrypted_message
for _ in range(encryption_rounds):
    decrypted_message = decrypt_row_column_transposition(decrypted_message, encryption_key)

print("Plain text :", decrypted_message)
