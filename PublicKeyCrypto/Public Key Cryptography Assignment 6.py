
import tkinter as tk
from tkinter import messagebox
import random
import customtkinter as ctk
from Crypto.Util import number

# ------------------------ Caesar Cipher ------------------------
def shift_encrypt(text, key):
    result = []
    for c in text:
        if c.isupper():
            result.append(chr((ord(c) - ord('A') + key) % 26 + ord('A')))
        elif c.islower():
            result.append(chr((ord(c) - ord('a') + key) % 26 + ord('a')))
        else:
            result.append(c)
    return ''.join(result)

def shift_decrypt(text, key):
    return shift_encrypt(text, -key)

# ------------------------ Diffie-Hellman ------------------------
dh_params = {}
def generate_dh_params():
    try:
        bits = int(entry_bits.get())
    except ValueError:
        messagebox.showerror("Error", "키 사이즈를 정수로 입력하세요")
        return
    p = number.getPrime(bits)
    g = 2
    a = random.randrange(2, p-2)  # Alice private
    b = random.randrange(2, p-2)  # Bob private
    A = pow(g, a, p)
    B = pow(g, b, p)
    shared = pow(B, a, p)
    dh_params.update({'p':p,'g':g,'a':a,'b':b,'A':A,'B':B,'shared':shared})
    # Update entries
    for var in ['p','g','a','b','A','B','shared']:
        entry = globals()[f"entry_{var}"]
        entry.configure(state='normal')
        entry.delete(0, tk.END)
        entry.insert(0, str(dh_params[var]))
        entry.configure(state='readonly')
    # Clear plaintext and ciphertext and attacker fields
    entry_plain.delete(0, tk.END)
    entry_cipher.delete(0, tk.END)
    entry_guess.delete(0, tk.END)
    entry_guess_plain.delete(0, tk.END)

def encrypt_action():
    pt = entry_plain.get()
    if not pt:
        messagebox.showwarning("Warning", "Plaintext를 입력하세요")
        return
    key = dh_params.get('shared', 0) % 26
    ct = shift_encrypt(pt, key)
    entry_cipher.configure(state='normal')
    entry_cipher.delete(0, tk.END)
    entry_cipher.insert(0, ct)
    entry_cipher.configure(state='readonly')

def decrypt_action():
    ct = entry_cipher.get()
    if not ct:
        messagebox.showwarning("Warning", "먼저 암호문을 생성하세요")
        return
    key = dh_params.get('shared', 0) % 26
    pt = shift_decrypt(ct, key)
    entry_plain.delete(0, tk.END)
    entry_plain.insert(0, pt)

def attacker_guess():
    try:
        guess = int(entry_guess.get())
    except ValueError:
        messagebox.showwarning("Warning", "0-25 사이 정수 입력")
        return
    ct = entry_cipher.get()
    if not ct:
        messagebox.showwarning("Warning", "먼저 암호문을 생성하세요")
        return
    ptg = shift_decrypt(ct, guess)
    entry_guess_plain.configure(state='normal')
    entry_guess_plain.delete(0, tk.END)
    entry_guess_plain.insert(0, ptg)
    entry_guess_plain.configure(state='readonly')
    real = dh_params.get('shared', 0) % 26
    if guess == real:
        messagebox.showinfo("Result", f"정답입니다! Key = {real}")
    else:
        messagebox.showinfo("Result", f"틀렸습니다. 실제 Key = {real}")

# ------------------------ GUI Setup ------------------------
ctk.set_appearance_mode("System")
root = ctk.CTk()
root.title("Diffie-Hellman + Caesar")
root.geometry("600x520")
root.configure(fg_color="#f0f0f0")

# Scrollable Frame
scrollable = ctk.CTkScrollableFrame(root, fg_color="#ffffff")
scrollable.pack(fill="both", expand=True)

# Title
ctk.CTkLabel(scrollable, text="Diffie-Hellman + Caesar Cipher", font=("Arial", 18, "bold"), text_color="#000000").pack(pady=10)

# DH Parameter Input
ctk.CTkLabel(scrollable, text="키 사이즈 (bits):", text_color="#000000").pack(anchor="w", padx=20)
entry_bits = ctk.CTkEntry(scrollable)
entry_bits.insert(0, "2048")
entry_bits.pack(fill="x", padx=20, pady=(0,10))
ctk.CTkButton(scrollable, text="DH 파라미터 생성", command=generate_dh_params).pack(pady=(0,20))

# DH Values Display
labels = [("p (Prime)", "p"), ("g (Generator)", "g"),
          ("a (Alice Private)", "a"), ("b (Bob Private)", "b"),
          ("A (Alice Public)", "A"), ("B (Bob Public)", "B"),
          ("Shared Secret", "shared")]
for text, var in labels:
    ctk.CTkLabel(scrollable, text=text+":", font=("Arial", 12, "bold"), text_color="#000000").pack(anchor="w", padx=20)
    entry = ctk.CTkEntry(scrollable, state="readonly")
    entry.pack(fill="x", padx=20, pady=(0,10))
    globals()[f"entry_{var}"] = entry

# Caesar Encryption / Decryption
ctk.CTkLabel(scrollable, text="Plaintext:", font=("Arial", 12, "bold"), text_color="#000000").pack(anchor="w", padx=20)
entry_plain = ctk.CTkEntry(scrollable)
entry_plain.pack(fill="x", padx=20, pady=(0,10))
ctk.CTkButton(scrollable, text="Encrypt (Alice→Bob)", command=encrypt_action).pack(pady=(0,10))

ctk.CTkLabel(scrollable, text="Ciphertext:", font=("Arial", 12, "bold"), text_color="#000000").pack(anchor="w", padx=20)
entry_cipher = ctk.CTkEntry(scrollable, state="readonly")
entry_cipher.pack(fill="x", padx=20, pady=(0,10))
ctk.CTkButton(scrollable, text="Decrypt (Bob→Alice)", command=decrypt_action).pack(pady=(0,20))

# Attacker View
ctk.CTkLabel(scrollable, text="Attacker View", font=("Arial", 14, "bold"), text_color="#000000").pack(pady=(0,10))
ctk.CTkLabel(scrollable, text="Guess Key (0-25):").pack(anchor="w", padx=20)
entry_guess = ctk.CTkEntry(scrollable)
entry_guess.pack(fill="x", padx=20, pady=(0,10))
ctk.CTkButton(scrollable, text="Guess", command=attacker_guess).pack(pady=(0,10))

ctk.CTkLabel(scrollable, text="Decrypted with Guess:", font=("Arial", 12, "bold"), text_color="#000000").pack(anchor="w", padx=20)
entry_guess_plain = ctk.CTkEntry(scrollable, state="readonly")
entry_guess_plain.pack(fill="x", padx=20, pady=(0,20))

root.mainloop()
