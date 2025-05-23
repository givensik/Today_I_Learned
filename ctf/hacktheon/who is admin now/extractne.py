from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.backends import default_backend
import base64

# public.pem 파일 읽기
with open("public.pem", "rb") as f:
    public_key = serialization.load_pem_public_key(f.read(), backend=default_backend())

# 공개키 정보 가져오기
numbers = public_key.public_numbers()
n = numbers.n
e = numbers.e

def b64url(n):
    return base64.urlsafe_b64encode(n.to_bytes((n.bit_length() + 7) // 8, byteorder="big")).decode().rstrip("=")

print("n:", b64url(n))
print("e:", b64url(e))