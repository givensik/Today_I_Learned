import base64

# XOR 함수
def xor_bytes(a, b):
    return bytes(x ^ y for x, y in zip(a, b))

# payload, signature 리스트
pairs = [
    ("i7bp4GxqyX_uvIyyRCcPblDOQU8F6ZhfW-N5xMj3RDY", "x9Y-41rk-TqDvnJoCv3qWMaV2V0"),
    ("uztIM67PQq9vZkaVCLjZo5cXTpzG87CdOV6tekzSzcY", "3-9DKD3PrRNcTDQcjI5Mn1q8hHA"),
    ("sAY0ZKBwhs63WDVbJYdreygsLX3mQCd9-C2fQXZKSPQ", "BqBmoUsY80h5pq64dU3bovz15oI"),
    ("eil4AHePItCufTm_GJaD44llogjMLGkQC2n9fN2c2IA", "X8NEm_gPU8ozc3e0L7EgRpdsWzM"),
    ("wbszoYbxiz73CYxzhwDtKLDAHqh_ihv0h25xSNPaWI", "PHGEn1ZeunGYWv0BpTQV3sKNPeU"),
    ("G6GmFjoUomcpzl2GlpCGR5j50HksuHhpPwKRJu9u1Kc", "Z_DzUlf2d0FFe9TYJfpzO51wpqM"),
]

print("[*] payload ⊕ signature 결과:")

for payload_b64, signature_b64 in pairs:
    payload_decoded = base64.urlsafe_b64decode(payload_b64 + '==')
    signature_decoded = base64.urlsafe_b64decode(signature_b64 + '==')
    
    xor_result = xor_bytes(payload_decoded, signature_decoded)
    
    print("\n[*] 새로운 세션:")
    print("HEX:", xor_result.hex())
    try:
        print("ASCII:", xor_result.decode('ascii'))
    except:
        print("ASCII (best effort):", xor_result.decode('ascii', errors='replace'))
