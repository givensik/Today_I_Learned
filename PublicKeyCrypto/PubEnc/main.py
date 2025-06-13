import tkinter as tk
from tkinter import ttk, messagebox

import DEStandard
import AESStandard
import DH
import random
import Elgamal  # 추가됨

# integrate encryption
def encrypt():
    if check_method1.get():
        DES_Enc()
    elif check_method2.get():
        AES_Enc()
    

# DES encrytion function
def DES_Enc():
    p = plaintext.get()
    k = key.get()
    c, round_keys, round_data = DEStandard.des_encrypt(p, k)
    ciphertext.set(c)
    
    key_bin = DEStandard.hex_to_bin(k)
    round_keys = DEStandard.generate_round_keys(key_bin)
    # 라운드 키 GUI에 출력
    roundkey_text.delete("1.0", tk.END)
    roundkey_text.insert(tk.END, "[Round Keys]\n")
    for i, rk in enumerate(round_keys, 1):
        roundkey_text.insert(tk.END, f"K{i:2}: {rk}\n")
    
    
    round1_output.delete("1.0", tk.END)
    round1_output.insert(tk.END, "[Round-wise Output]\n")
    for data in round_data:
        round1_output.insert(tk.END, f"Round {data['round']:2}: L = {data['L']} | R = {data['R']}\n")
        round1_output.insert(tk.END, f"           S-box Output: {data['S-box']}\n")

# AES encrytion function
def AES_Enc():
    p = plaintext.get().lower()
    k = key.get().lower()
    # AESStandard.encrypt_detailed: (cipher_hex, [subkey0,...,subkey10], [{round, state_after_subbytes, state_after_shift, state_after_mix, state_after_key_add},...])
    c, round_keys, round_data = AESStandard.encrypt_detailed(p, k)
    ciphertext.set(c)

    # AES 서브키(라운드 키) 출력
    roundkey_text.delete("1.0", tk.END)
    roundkey_text.insert(tk.END, "[AES Round Keys]\n")
    for i, rk in enumerate(round_keys):
        roundkey_text.insert(tk.END, f"K{i:02d}: {rk}\n")

    # AES 라운드별 상태 출력
    round1_output.delete("1.0", tk.END)
    round1_output.insert(tk.END, "[AES Round-wise State]\n")
    for data in round_data:
        round1_output.insert(tk.END,
            f"Round {data['round']:2d}: SubBytes={data['subbytes']} | ShiftRows={data['shiftrows']} | MixColumns={data.get('mixcolumns','-')} | AddKey={data['addkey']}\n"
        )    
    


def decrypt():
    if check_method1.get():
        DES_Dec()
    elif check_method2.get():
        AES_Dec()



# DES decryption function
def DES_Dec():
    c = ciphertext.get()
    k = key.get()
    p = DEStandard.des_decrypt(c, k)
    decrypt_value.set(p)

# --- AES Decryption ---
def AES_Dec():
    c = ciphertext.get()
    k = key.get().lower()
    p = AESStandard.decrypt_gui(c, k)
    decrypt_value.set(p)

def generate_dh_keys():
    
    try:
        key_bits = int(keysize_var.get())  # 콤보박스에서 선택된 키 사이즈
        plaintext = DH_plaintext.get()
        result = DH.run_dh_simulation_gui(key_bits, plaintext)
        label_p_value.set(str(result["p"]))
        label_g_value.set(str(2))
        label_A_value.set(str(result["A"]))
        label_B_value.set(str(result["B"]))
        label_shared_value.set(str(result["shared"]))
    except Exception as e:
        messagebox.showerror("Error", f"키 생성 중 오류 발생:\n{e}")

def encrypt_dh():
    plaintext = DH_plaintext.get()
    shift_key = int(label_shared_value.get())%26
    cipher = DH.shift_encrypt(plaintext, shift_key)
    DH_ciphertext.set(cipher)
    
def decrypt_dh():
    cipher = DH_ciphertext.get()
    shift_key = int(label_shared_value.get())%26
    plaintext = DH.shift_decrypt(cipher, shift_key)
    DH_decrypt.set(plaintext)


def attacker_try_guess():
    try:
        p = int(label_p_value.get())
        g = int(label_g_value.get())
        A = int(label_A_value.get())
        B = int(label_B_value.get())
        cipher = DH_ciphertext.get()
        result = DH.attacker_attempt(p, g, A, B, cipher)
        attacker_attempt_result.set(result)
    except Exception as e:
        messagebox.showerror("Error", f"공격자 시도 중 오류:\n{e}")

# ---------------- ElGamal 관련 함수 ----------------
def generate_elgamal_keys():
    try:
        key_bits = int(keysize_var.get())
        pk, sk = Elgamal.generate_keys(key_bits)

        elgamal_p.set(str(pk['p']))
        elgamal_g.set(str(pk['g']))
        elgamal_e.set(str(pk['e']))

        global elgamal_private_key
        elgamal_private_key = sk

    except Exception as e:
        messagebox.showerror("Error", f"ElGamal 키 생성 오류: {e}")

def encrypt_elgamal():
    try:
        message = elgamal_plaintext.get()
        pk = {
            'p': int(elgamal_p.get()),
            'g': int(elgamal_g.get()),
            'e': int(elgamal_e.get())
        }
        c1, c2 = Elgamal.encrypt(pk, message)
        elgamal_cipher_c1.set(str(c1))
        elgamal_cipher_c2.set(str(c2))
    except Exception as e:
        messagebox.showerror("Error", f"ElGamal 암호화 오류: {e}")

def decrypt_elgamal():
    try:
        p = int(elgamal_p.get())
        c1 = int(elgamal_cipher_c1.get())
        c2 = int(elgamal_cipher_c2.get())
        ciphertext = (c1, c2)
        plaintext = Elgamal.decrypt(ciphertext, elgamal_private_key, p)
        elgamal_decrypted.set(plaintext)
    except Exception as e:
        messagebox.showerror("Error", f"ElGamal 복호화 중 오류 발생:\n{e}")

# Function to simulate attacker's brute-force attempt
def elgamal_attacker_try():
    try:
        p = int(elgamal_p.get())
        g = int(elgamal_g.get())
        e = int(elgamal_e.get())
        c1 = int(elgamal_cipher_c1.get())
        c2 = int(elgamal_cipher_c2.get())
        recovered = Elgamal.attacker_try_bruteforce(p, g, e, (c1, c2))
        elgamal_attacker_result.set(recovered)
    except Exception as e:
        messagebox.showerror("Error", f"ElGamal Attacker Error: {e}")

# Create main window
root = tk.Tk()
root.title("Assignment")  # 창 제목
root.geometry("1200x1000")       # 초기 크기 설정


### define variables
# value variables
plaintext = tk.StringVar()
key = tk.StringVar()
ciphertext = tk.StringVar()
decrypt_value = tk.StringVar()


# checkbox variables
check_method1 = tk.BooleanVar(value=False)  # default False 
check_method2 = tk.BooleanVar(value=False)  # default False 


# Assignment 6
# combo box variables
keysize_var = tk.StringVar()

# variables for DH
label_p_value = tk.StringVar()
label_g_value = tk.StringVar()
label_A_value = tk.StringVar()
label_B_value = tk.StringVar()
label_shared_value = tk.StringVar()

DH_plaintext = tk.StringVar()
DH_ciphertext = tk.StringVar()
DH_decrypt = tk.StringVar()
attacker_attempt_result = tk.StringVar()

# Assignment 6 Elgamal
elgamal_p = tk.StringVar()
elgamal_g = tk.StringVar()
elgamal_e = tk.StringVar()
elgamal_plaintext = tk.StringVar()
elgamal_cipher_c1 = tk.StringVar()
elgamal_cipher_c2 = tk.StringVar()
elgamal_decrypted = tk.StringVar()
elgamal_private_key = None
elgamal_attacker_result = tk.StringVar()


# for checkbox    
def on_check(var, checkbox_vars): # check only one checkbox
    for v in checkbox_vars:
        if v != var: # only var is checked
            v.set(False)
            

check = ttk.Checkbutton(
    root,
    text="DES",
    variable=check_method1,
    command=lambda: on_check(check_method1, [check_method1, check_method2])  # 상태 변경시 호출될 함수
)
check.grid(row=0, column=0, sticky=tk.W, padx=5, pady=5)

check = ttk.Checkbutton(
    root,
    text="AES",
    variable=check_method2,
    command=lambda: on_check(check_method2, [check_method1, check_method2])  # 상태 변경시 호출될 함수
)
check.grid(row=0, column=1, sticky=tk.W, padx=5, pady=5)



# for input text area

# plain text GUI
plaintextGUI = ttk.Label(root, text="Plain text")
plaintextGUI.grid(row=1, column=0)
# plain text input    
plaintext_input = ttk.Entry(root, textvariable=plaintext) # plain text 입력 칸
plaintext_input.grid(row=1, column=1)


# key GUI
key_GUI = ttk.Label(root, text="Key")
key_GUI.grid(row=2, column=0)
# key input    
key_input = ttk.Entry(root, textvariable=key) # plain text 입력 칸
key_input.grid(row=2, column=1)

# output GUI
output_label = ttk.Label(root, text="Output") 
output_label.grid(row = 3, column= 0)
output_text = ttk.Label(root, textvariable=ciphertext)
output_text.grid(row = 3, column= 1)

# ciphertext GUI
ciphertextGUI = ttk.Label(root, text="Cipher text")
ciphertextGUI.grid(row=5, column=0)
# plain text input    
ciphertext_input = ttk.Entry(root, textvariable=ciphertext) # cipher text 입력 칸
ciphertext_input.grid(row=5, column=1)


# key GUI
key_GUI = ttk.Label(root, text="Key")
key_GUI.grid(row=6, column=0)
# key input    
key_input = ttk.Entry(root, textvariable=key) # plain text 입력 칸
key_input.grid(row=6, column=1)


# decrypt GUI
output_label = ttk.Label(root, text="Decrypted") 
output_label.grid(row = 7, column= 0)
output_text = ttk.Label(root, textvariable=decrypt_value)
output_text.grid(row = 7, column= 1)

# Round Key GUI
# key GUI
key_GUI = ttk.Label(root, text="Round Key")
key_GUI.grid(row=1, column=2)

roundkey_text = tk.Text(root, width=60, height=20, font=("Courier", 10))
roundkey_text.grid(row=1, column=3, rowspan=8, padx=10, pady=5)

# L1, R1 output
round1_output = tk.Text(root, width=60, height=6, font=("Courier", 10))
round1_output.grid(row=12, column=0, columnspan=3, padx=10, pady=10)



# button
# encrypt button GUI
btn = ttk.Button(root, text="Encrypt", command = encrypt)
btn.grid(row=4, column=0, padx=10, pady=5, sticky="w")

# decrypt button GUI
btn = ttk.Button(root, text="Decrypt", command = decrypt)
btn.grid(row=5, column=0, padx=10, pady=5, sticky="w")



# combo box for select 

keysize_combo = ttk.Combobox(
    root,
    textvariable=keysize_var,
    values=["16", "32", "64", "128", "256", "512", "1024", "2048", "3072"],
    state="readonly"  # 사용자가 직접 입력하지 못하도록 설정
)
keysize_combo.current(0)
keysize_label = ttk.Label(root, text="Key Size (bits):")
keysize_label.grid(row=16, column=0, padx=5, pady=5, sticky="w")
keysize_combo.grid(row=16, column=1, padx=5, pady=5, sticky="w")

# --- Generate Key Button ---
ttks = ttk.Separator(root, orient="horizontal")
ttks.grid(row=15, column=0, columnspan=4, sticky="ew", pady=10)

btn_genkey = ttk.Button(root, text="Generate Keys (DH)", command=generate_dh_keys)
btn_genkey.grid(row=17, column=0, padx=5, pady=5, sticky="w")

btn = ttk.Button(root, text="Encrypt", command = encrypt_dh) 
btn.grid(row=27, column=0, padx=10, pady=5, sticky="w")


btn = ttk.Button(root, text="Decrypt", command = decrypt_dh) 
btn.grid(row=27, column=1, padx=10, pady=5, sticky="w")

ttk.Label(root, text="p (prime):").grid(row=18, column=0, sticky="w")
ttk.Label(root, textvariable=label_p_value).grid(row=18, column=1, sticky="w")

ttk.Label(root, text="g (primitive root):").grid(row=19, column=0, sticky="w")
ttk.Label(root, textvariable=label_g_value).grid(row=19, column=1, sticky="w")

ttk.Label(root, text="Alice's Public Key A:").grid(row=20, column=0, sticky="w")
ttk.Label(root, textvariable=label_A_value).grid(row=20, column=1, sticky="w")

ttk.Label(root, text="Bob's Public Key B:").grid(row=21, column=0, sticky="w")
ttk.Label(root, textvariable=label_B_value).grid(row=21, column=1, sticky="w")

ttk.Label(root, text="Shared Secret Key:").grid(row=22, column=0, sticky="w")
ttk.Label(root, textvariable=label_shared_value).grid(row=22, column=1, sticky="w")

# plain text GUI
plaintextGUI = ttk.Label(root, text="Alice Message")
plaintextGUI.grid(row=23, column=0)
# plain text input    
plaintext_input = ttk.Entry(root, textvariable=DH_plaintext) # plain text 입력 칸
plaintext_input.grid(row=23, column=1)

# output GUI
output_label = ttk.Label(root, text="Alice's Encrypted message") 
output_label.grid(row = 24, column= 0)
output_text = ttk.Label(root, textvariable=DH_ciphertext)
output_text.grid(row = 24, column= 1)


# output GUI
output_label = ttk.Label(root, text="Message Bob received") 
output_label.grid(row = 25, column= 0)
output_text = ttk.Label(root, textvariable=DH_decrypt)
output_text.grid(row = 25, column= 1)

# 수직 구분선 추가 (선택사항)
ttk.Separator(root, orient="vertical").grid(row=24, column=2, rowspan=7, sticky="ns", padx=10)

# 공격자 시뮬레이션 (우측에 배치)
ttk.Label(root, text="Attacker Simulation").grid(row=25, column=3, sticky="w", padx=5)
ttk.Button(root, text="Try to Guess Key", command=attacker_try_guess).grid(row=26, column=3, sticky="w", padx=5)

ttk.Label(root, text="Attacker Recovered Message:").grid(row=27, column=3, sticky="w")
ttk.Label(root, textvariable=attacker_attempt_result).grid(row=28, column=3, sticky="w")

# ---------------- ElGamal GUI ----------------
ttk.Separator(root, orient="horizontal").grid(row=31, column=0, columnspan=4, sticky="ew", pady=10)

# 제목
ttk.Label(root, text="ElGamal Encryption").grid(row=32, column=0, sticky="w")

# Key Generation
ttk.Button(root, text="Generate ElGamal Keys", command=generate_elgamal_keys).grid(row=33, column=0, padx=5, pady=5)

# 공개키 출력
ttk.Label(root, text="p:").grid(row=34, column=0, sticky="w")
ttk.Label(root, textvariable=elgamal_p).grid(row=34, column=1, sticky="w")

ttk.Label(root, text="g:").grid(row=35, column=0, sticky="w")
ttk.Label(root, textvariable=elgamal_g).grid(row=35, column=1, sticky="w")

ttk.Label(root, text="e (public key):").grid(row=36, column=0, sticky="w")
ttk.Label(root, textvariable=elgamal_e).grid(row=36, column=1, sticky="w")

# 평문 입력
ttk.Label(root, text="Message to Encrypt:").grid(row=37, column=0, sticky="w")
ttk.Entry(root, textvariable=elgamal_plaintext).grid(row=37, column=1, sticky="w")

# 암호화 버튼
ttk.Button(root, text="Encrypt (ElGamal)", command=encrypt_elgamal).grid(row=38, column=0, padx=5, pady=5)

# 암호문 출력
ttk.Label(root, text="Ciphertext Y1:").grid(row=39, column=0, sticky="w")
ttk.Label(root, textvariable=elgamal_cipher_c1).grid(row=39, column=1, sticky="w")

ttk.Label(root, text="Ciphertext Y2:").grid(row=40, column=0, sticky="w")
ttk.Label(root, textvariable=elgamal_cipher_c2).grid(row=40, column=1, sticky="w")

# 복호화 버튼
ttk.Button(root, text="Decrypt (ElGamal)", command=decrypt_elgamal).grid(row=41, column=0, padx=5, pady=5)

# 복호문 출력
ttk.Label(root, text="Decrypted Message:").grid(row=42, column=0, sticky="w")
ttk.Label(root, textvariable=elgamal_decrypted).grid(row=42, column=1, sticky="w")

# ElGamal 공격자 시뮬레이션 (오른쪽 열에 배치)
ttk.Separator(root, orient="vertical").grid(row=32, column=2, rowspan=15, sticky="ns", padx=10)

ttk.Label(root, text="ElGamal Attacker Simulation").grid(row=32, column=3, sticky="w", padx=5)
ttk.Button(root, text="Try to Guess Secret (ElGamal)", command=elgamal_attacker_try).grid(row=33, column=3, sticky="w", padx=5)
ttk.Label(root, text="Recovered Message:").grid(row=34, column=3, sticky="w")
ttk.Label(root, textvariable=elgamal_attacker_result).grid(row=35, column=3, sticky="w")


if __name__ == "__main__":
    root.mainloop()  # 이 줄이 없으면 창이 바로 닫힘!