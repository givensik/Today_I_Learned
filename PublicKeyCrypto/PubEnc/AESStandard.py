#!/usr/bin/env python3
# AES-128 Encryption/Decryption with Full Key Expansion

# S-Box and RCON tables
S_BOX = [
    [0x63,0x7C,0x77,0x7B,0xF2,0x6B,0x6F,0xC5,0x30,0x01,0x67,0x2B,0xFE,0xD7,0xAB,0x76],
    [0xCA,0x82,0xC9,0x7D,0xFA,0x59,0x47,0xF0,0xAD,0xD4,0xA2,0xAF,0x9C,0xA4,0x72,0xC0],
    [0xB7,0xFD,0x93,0x26,0x36,0x3F,0xF7,0xCC,0x34,0xA5,0xE5,0xF1,0x71,0xD8,0x31,0x15],
    [0x04,0xC7,0x23,0xC3,0x18,0x96,0x05,0x9A,0x07,0x12,0x80,0xE2,0xEB,0x27,0xB2,0x75],
    [0x09,0x83,0x2C,0x1A,0x1B,0x6E,0x5A,0xA0,0x52,0x3B,0xD6,0xB3,0x29,0xE3,0x2F,0x84],
    [0x53,0xD1,0x00,0xED,0x20,0xFC,0xB1,0x5B,0x6A,0xCB,0xBE,0x39,0x4A,0x4C,0x58,0xCF],
    [0xD0,0xEF,0xAA,0xFB,0x43,0x4D,0x33,0x85,0x45,0xF9,0x02,0x7F,0x50,0x3C,0x9F,0xA8],
    [0x51,0xA3,0x40,0x8F,0x92,0x9D,0x38,0xF5,0xBC,0xB6,0xDA,0x21,0x10,0xFF,0xF3,0xD2],
    [0xCD,0x0C,0x13,0xEC,0x5F,0x97,0x44,0x17,0xC4,0xA7,0x7E,0x3D,0x64,0x5D,0x19,0x73],
    [0x60,0x81,0x4F,0xDC,0x22,0x2A,0x90,0x88,0x46,0xEE,0xB8,0x14,0xDE,0x5E,0x0B,0xDB],
    [0xE0,0x32,0x3A,0x0A,0x49,0x06,0x24,0x5C,0xC2,0xD3,0xAC,0x62,0x91,0x95,0xE4,0x79],
    [0xE7,0xC8,0x37,0x6D,0x8D,0xD5,0x4E,0xA9,0x6C,0x56,0xF4,0xEA,0x65,0x7A,0xAE,0x08],
    [0xBA,0x78,0x25,0x2E,0x1C,0xA6,0xB4,0xC6,0xE8,0xDD,0x74,0x1F,0x4B,0xBD,0x8B,0x8A],
    [0x70,0x3E,0xB5,0x66,0x48,0x03,0xF6,0x0E,0x61,0x35,0x57,0xB9,0x86,0xC1,0x1D,0x9E],
    [0xE1,0xF8,0x98,0x11,0x69,0xD9,0x8E,0x94,0x9B,0x1E,0x87,0xE9,0xCE,0x55,0x28,0xDF],
    [0x8C,0xA1,0x89,0x0D,0xBF,0xE6,0x42,0x68,0x41,0x99,0x2D,0x0F,0xB0,0x54,0xBB,0x16]
]
RCON = [
    [0x01,0x00,0x00,0x00],[0x02,0x00,0x00,0x00],[0x04,0x00,0x00,0x00],[0x08,0x00,0x00,0x00],
    [0x10,0x00,0x00,0x00],[0x20,0x00,0x00,0x00],[0x40,0x00,0x00,0x00],[0x80,0x00,0x00,0x00],
    [0x1B,0x00,0x00,0x00],[0x36,0x00,0x00,0x00]
]

# Inverse S-Box built from S_BOX
INV_S_BOX = [[0]*16 for _ in range(16)]
for i in range(16):
    for j in range(16):
        v = S_BOX[i][j]
        INV_S_BOX[v>>4][v&0x0F] = (i<<4)|j

def key_expansion(key: bytes):
    # produce 44 words of 4 bytes each
    key_columns = [list(key[i:i+4]) for i in range(0, 16, 4)]
    i = 4
    while len(key_columns) < 44:
        temp = key_columns[-1][:]
        if i % 4 == 0:
            # RotWord + SubWord + Rcon
            temp = temp[1:] + temp[:1]
            temp = [ S_BOX[b>>4][b&0x0F] for b in temp ]
            temp[0] ^= RCON[i//4 - 1][0]
        word = [a ^ b for a,b in zip(key_columns[-4], temp)]
        key_columns.append(word)
        i += 1
    return key_columns

def sub_bytes(state):     return [[S_BOX[b>>4][b&0x0F] for b in row] for row in state]
def inv_sub_bytes(state): return [[INV_S_BOX[b>>4][b&0x0F] for b in row] for row in state]
def shift_rows(state):    return [row[i:]+row[:i] for i,row in enumerate(state)]
def inv_shift_rows(state):return [row[-i:]+row[:-i] for i,row in enumerate(state)]
def xtime(x):             return ((x<<1)^0x1B)&0xFF if (x&0x80) else (x<<1)&0xFF

def mix_single_column(col):
    a = col
    return [
        xtime(a[0]) ^ xtime(a[1]) ^ a[1] ^ a[2] ^ a[3],
        a[0] ^ xtime(a[1]) ^ xtime(a[2]) ^ a[2] ^ a[3],
        a[0] ^ a[1] ^ xtime(a[2]) ^ xtime(a[3]) ^ a[3],
        xtime(a[0]) ^ a[0] ^ a[1] ^ a[2] ^ xtime(a[3])
    ]

def mix_columns(state):
    # operate on columns, then transpose back
    cols = [mix_single_column(col) for col in zip(*state)]
    return [list(row) for row in zip(*cols)]

def inv_mix_columns(state):
    # fully correct inverse with GF(2^8) multiply by 9,11,13,14
    def gf_mult(a,b):
        p = 0
        for _ in range(8):
            if b & 1: p ^= a
            hi = a & 0x80
            a = (a << 1) & 0xFF
            if hi: a ^= 0x1B
            b >>= 1
        return p

    cols = list(zip(*state))
    new_cols = []
    for c in cols:
        new_cols.append([
            gf_mult(c[0],14) ^ gf_mult(c[1],11) ^ gf_mult(c[2],13) ^ gf_mult(c[3],9),
            gf_mult(c[0],9)  ^ gf_mult(c[1],14) ^ gf_mult(c[2],11) ^ gf_mult(c[3],13),
            gf_mult(c[0],13) ^ gf_mult(c[1],9)  ^ gf_mult(c[2],14) ^ gf_mult(c[3],11),
            gf_mult(c[0],11) ^ gf_mult(c[1],13) ^ gf_mult(c[2],9)  ^ gf_mult(c[3],14),
        ])
    return [list(row) for row in zip(*new_cols)]

def add_round_key(state, key_sched, rnd):
    # key_sched is list of 44 words, we take 4 words per round
    rk = key_sched[4*rnd : 4*(rnd+1)]
    return [
      [ state[r][c] ^ rk[c][r] for c in range(4) ]
      for r in range(4)
    ]

def bytes_to_state(barr):
    # column-major ↔ row matrix
    return [[barr[4*c + r] for c in range(4)] for r in range(4)]

def state_to_bytes(state):
    return bytes(state[r][c] for c in range(4) for r in range(4))

def aes_encrypt(pt: bytes, key_sched):
    st = add_round_key(bytes_to_state(pt), key_sched, 0)
    for r in range(1,10):
        st = sub_bytes(st)
        st = shift_rows(st)
        st = mix_columns(st)
        st = add_round_key(st, key_sched, r)
    # final round
    st = sub_bytes(st)
    st = shift_rows(st)
    st = add_round_key(st, key_sched, 10)
    return state_to_bytes(st)

def aes_decrypt(ct: bytes, key_sched):
    st = add_round_key(bytes_to_state(ct), key_sched, 10)
    for r in range(9,0,-1):
        st = inv_shift_rows(st)
        st = inv_sub_bytes(st)
        st = add_round_key(st, key_sched, r)
        st = inv_mix_columns(st)
    # final inv-round
    st = inv_shift_rows(st)
    st = inv_sub_bytes(st)
    st = add_round_key(st, key_sched, 0)
    return state_to_bytes(st)

# GUI-style hex wrappers
def encrypt_gui(plaintext: str, key: str) -> str:
    ptb = plaintext.encode('utf-8')[:16]
    kb  = key.encode('utf-8')[:16]
    ks  = key_expansion(kb)
    return aes_encrypt(ptb, ks).hex()

def decrypt_gui(ct_hex: str, key: str) -> str:
    ctb = bytes.fromhex(ct_hex)
    if len(ctb) != 16:
        return "[!] 복호화 실패: 잘못된 암호문 길이"
    kb = key.encode('utf-8')[:16]
    ks = key_expansion(kb)
    ptb = aes_decrypt(ctb, ks)
    try:
        return ptb.decode('utf-8')
    except UnicodeDecodeError:
        return "[!] 복호화 실패: 유효하지 않은 UTF-8 디코딩"
# --- Detailed Encryption ---
def encrypt_detailed(plaintext, key):
    pt = plaintext.encode('utf-8')[:16].ljust(16,b'\x00')
    kb = key.encode('utf-8')[:16].ljust(16,b'\x00')
    ks = key_expansion(kb)
    round_keys = []
    for r in range(11):
        rk_bytes = state_to_bytes([ [ks[4*r+c][r] for c in range(4)] for r in range(4)])
        round_keys.append(rk_bytes.hex())

    state = bytes_to_state(pt)
    round_data = []
    # initial AddKey
    state = add_round_key(state, ks, 0)
    round_data.append({'round':0,'subbytes':None,'shiftrows':None,'mixcolumns':None,'addkey':state_to_bytes(state).hex()})

    # rounds 1-9
    for r in range(1,10):
        sb = sub_bytes(state)
        sr = shift_rows(sb)
        mc = mix_columns(sr)
        ak = add_round_key(mc, ks, r)
        round_data.append({
            'round':r,
            'subbytes':state_to_bytes(sb).hex(),
            'shiftrows':state_to_bytes(sr).hex(),
            'mixcolumns':state_to_bytes(mc).hex(),
            'addkey':state_to_bytes(ak).hex()
        })
        state = ak
    # final round
    sb = sub_bytes(state)
    sr = shift_rows(sb)
    ak = add_round_key(sr, ks, 10)
    round_data.append({'round':10,'subbytes':state_to_bytes(sb).hex(),
                       'shiftrows':state_to_bytes(sr).hex(),'mixcolumns':None,
                       'addkey':state_to_bytes(ak).hex()})
    ciphertext_hex = state_to_bytes(ak).hex()
    return ciphertext_hex, round_keys, round_data



# ———— Test ————
if __name__ == "__main__":
    pt = "iadmirethisclass"
    k  = "goodcryptography"

    print("=== ENCRYPTION ===")
    c_hex = encrypt_gui(pt, k)
    print("Ciphertext:", c_hex)

    print("=== DECRYPTION ===")
    d = decrypt_gui(c_hex, k)
    print("Decrypted:", d)

    # Also verify against the NIST vector:
    # AES-128: key=00010203…0e0f, pt=00112233…eeff → ct should be 69c4e0d86a7b0430d8cdb78070b4c55a
    import sys
    if "--test-nist" in sys.argv:
        from binascii import unhexlify
        k2 = unhexlify("000102030405060708090a0b0c0d0e0f")
        p2 = unhexlify("00112233445566778899aabbccddeeff")
        ks2 = key_expansion(k2)
        ct2 = aes_encrypt(p2, ks2)
        pt2 = aes_decrypt(ct2, ks2)
        print("NIST CT:", ct2.hex(), "| matches? ", ct2.hex()=="69c4e0d86a7b0430d8cdb78070b4c55a")
        print("NIST PT:", pt2.hex(), "| round-trip ok? ", pt2==p2)