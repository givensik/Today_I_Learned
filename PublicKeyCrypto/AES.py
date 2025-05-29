# AES Full Encryption Example with Round-by-Round Output

# Required S-box and RCON definitions
S_BOX = [...]

RCON = [...]

# AES utility functions for Key Expansion
...

# Round functions

def format_state(state):
    return "\n".join(" ".join(f"{b:02X}" for b in row) for row in state)

def sub_bytes(state):
    return [[S_BOX[b >> 4][b & 0x0F] for b in row] for row in state]

def shift_rows(state):
    return [row[i:] + row[:i] for i, row in enumerate(state)]

def xtime(a):
    return ((a << 1) ^ 0x1B) & 0xFF if a & 0x80 else a << 1

def mix_single_column(col):
    a = col
    return [
        xtime(a[0]) ^ xtime(a[1]) ^ a[1] ^ a[2] ^ a[3],
        a[0] ^ xtime(a[1]) ^ xtime(a[2]) ^ a[2] ^ a[3],
        a[0] ^ a[1] ^ xtime(a[2]) ^ xtime(a[3]) ^ a[3],
        xtime(a[0]) ^ a[0] ^ a[1] ^ a[2] ^ xtime(a[3])
    ]

def mix_columns(state):
    return list(map(list, zip(*[mix_single_column(col) for col in zip(*state)])))

def add_round_key(state, key_schedule, round):
    for r in range(4):
        for c in range(4):
            state[r][c] ^= key_schedule[round * 4 + c][r]
    return state

def bytes_to_state(byte_array):
    return [[byte_array[row + 4 * col] for col in range(4)] for row in range(4)]

def aes_encrypt(plaintext_bytes, key_schedule):
    state = bytes_to_state([b for b in plaintext_bytes])
    print("\nInitial State:")
    print(format_state(state))

    state = add_round_key(state, key_schedule, 0)
    print("\nAfter AddRoundKey (Round 0):")
    print(format_state(state))

    for round in range(1, 10):
        state = sub_bytes(state)
        print(f"\nAfter SubBytes (Round {round}):")
        print(format_state(state))

        state = shift_rows(state)
        print(f"\nAfter ShiftRows (Round {round}):")
        print(format_state(state))

        state = mix_columns(state)
        print(f"\nAfter MixColumns (Round {round}):")
        print(format_state(state))

        state = add_round_key(state, key_schedule, round)
        print(f"\nAfter AddRoundKey (Round {round}):")
        print(format_state(state))

    # Final round (no mix_columns)
    state = sub_bytes(state)
    print("\nAfter SubBytes (Round 10):")
    print(format_state(state))

    state = shift_rows(state)
    print("\nAfter ShiftRows (Round 10):")
    print(format_state(state))

    state = add_round_key(state, key_schedule, 10)
    print("\nAfter AddRoundKey (Round 10):")
    print(format_state(state))

    print("\nFinal Ciphertext (Hex, column-major):")
    ciphertext = "".join(f"{state[c][r]:02X}" for r in range(4) for c in range(4))
    print(ciphertext)
    return state

# Sample execution
plaintext = "iadmirethisclass"
plaintext_bytes = plaintext.encode("utf-8")[:16]
key = "goodcryptography"
key_bytes = key.encode("utf-8")[:16]
key_schedule = key_expansion(key_bytes)
final_state = aes_encrypt(plaintext_bytes, key_schedule)
