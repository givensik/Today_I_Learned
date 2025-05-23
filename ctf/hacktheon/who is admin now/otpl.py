import base64

s = "LAl79SQGEdPzoZGs0a2HJi-lHJqVG-jF8ghQsmpA4rA"
# base64 URL safe 디코딩 (padding 추가)
decoded = base64.urlsafe_b64decode(s + '==')
print(decoded)