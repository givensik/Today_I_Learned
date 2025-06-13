
import tkinter as tk
from tkinter import messagebox
import random
import customtkinter as ctk
from Crypto.Util import number

# ------------------ ElGamal Functions ------------------
elg_params = {}

def generate_elg_params():
    try:
        key_size = int(entry_keysize.get())
    except ValueError:
        messagebox.showerror("Error", "정수 키 사이즈를 입력하세요")
        return
    p = number.getPrime(key_size)
    g = 2
    a = random.randrange(2, p-2)
    e = pow(g, a, p)
    elg_params.update({'p':p, 'g':g, 'a':a, 'e':e})
    entry_p.delete(0, tk.END); entry_p.insert(0, str(p))
    entry_g.delete(0, tk.END); entry_g.insert(0, str(g))
    entry_e.delete(0, tk.END); entry_e.insert(0, str(e))
    entry_a.delete(0, tk.END); entry_a.insert(0, str(a))  # display private key
    # Clear downstream fields
    entry_plain.delete(0, tk.END)
    entry_c1.delete(0, tk.END)
    entry_c2.delete(0, tk.END)
    entry_dec.delete(0, tk.END)
    entry_guess.delete(0, tk.END)
    entry_guess_dec.delete(0, tk.END)

def encrypt_elg():
    try:
        m = int(entry_plain.get())
    except ValueError:
        messagebox.showwarning("Warn", "정수 평문 < p 입력하세요")
        return
    p = elg_params['p']; g = elg_params['g']; e = elg_params['e']
    k = random.randrange(2, p-2)
    c1 = pow(g, k, p)
    c2 = (m * pow(e, k, p)) % p
    elg_params.update({'c1':c1, 'c2':c2, 'm':m})
    entry_c1.delete(0, tk.END); entry_c1.insert(0, str(c1))
    entry_c2.delete(0, tk.END); entry_c2.insert(0, str(c2))
    entry_dec.delete(0, tk.END)

def decrypt_elg():
    if 'c1' not in elg_params:
        messagebox.showwarning("Warn", "먼저 Encrypt를 실행하세요")
        return
    p = elg_params['p']; a = elg_params['a']
    c1 = elg_params['c1']; c2 = elg_params['c2']
    m = (c2 * pow(c1, p-1-a, p)) % p
    entry_dec.delete(0, tk.END); entry_dec.insert(0, str(m))

def attacker_guess_elg():
    try:
        guess = int(entry_guess.get())
    except ValueError:
        messagebox.showwarning("Warn", "정수 추측 입력하세요")
        return
    if 'm' not in elg_params:
        messagebox.showwarning("Warn", "먼저 Encrypt를 실행하세요")
        return
    real = elg_params['m']
    entry_guess_dec.delete(0, tk.END); entry_guess_dec.insert(0, str(real))
    if guess == real:
        messagebox.showinfo("Result", "정답입니다!")
    else:
        messagebox.showinfo("Result", f"틀렸습니다. 실제 메시지 = {real}")

# ------------------ GUI Setup ------------------
ctk.set_appearance_mode("System")
root = ctk.CTk()
root.title("ElGamal Cryptosystem")
root.geometry("600x500")

# Scrollable Frame
scrollable = ctk.CTkScrollableFrame(root, fg_color="#ffffff")
scrollable.pack(fill="both", expand=True)


# Key Generation Section
ctk.CTkLabel(scrollable, text="ElGamal Key Generation", font=("Arial", 16, "bold"), text_color="#000000").pack(pady=10)
ctk.CTkLabel(scrollable, text="Key Size (bits):", text_color="#000000").pack(anchor="w", padx=20)
entry_keysize = ctk.CTkEntry(scrollable, placeholder_text="e.g. 2048")
entry_keysize.pack(fill="x", padx=20, pady=5)
ctk.CTkButton(scrollable, text="Generate Parameters", command=generate_elg_params, text_color="#000000").pack(pady=10)

# Public Parameters Section
ctk.CTkLabel(scrollable, text="Public Parameters", font=("Arial", 14, "bold"), text_color="#000000").pack(pady=10)
ctk.CTkLabel(scrollable, text="p (Prime) =", text_color="#000000").pack(anchor="w", padx=20)
entry_p = ctk.CTkEntry(scrollable)
entry_p.pack(fill="x", padx=20, pady=5)
ctk.CTkLabel(scrollable, text="g (Generator) =", text_color="#000000").pack(anchor="w", padx=20)
entry_g = ctk.CTkEntry(scrollable)
entry_g.pack(fill="x", padx=20, pady=5)
ctk.CTkLabel(scrollable, text="e (Public Key) =", text_color="#000000").pack(anchor="w", padx=20)
entry_e = ctk.CTkEntry(scrollable)
entry_e.pack(fill="x", padx=20, pady=5)
ctk.CTkLabel(scrollable, text="a (Private Key) =", font=("Arial", 14, "bold"), text_color="#000000").pack(anchor="w", padx=20)
entry_a = ctk.CTkEntry(scrollable)
entry_a.pack(fill="x", padx=20, pady=(0,10))

# Encryption / Decryption Section
ctk.CTkLabel(scrollable, text="Encryption & Decryption", font=("Arial", 14, "bold"), text_color="#000000").pack(pady=10)
ctk.CTkLabel(scrollable, text="Plaintext (integer < p):", text_color="#000000").pack(anchor="w", padx=20)
entry_plain = ctk.CTkEntry(scrollable)
entry_plain.pack(fill="x", padx=20, pady=5)
ctk.CTkButton(scrollable, text="Encrypt", command=encrypt_elg, text_color="#000000").pack(pady=5)
ctk.CTkLabel(scrollable, text="Ciphertext c1:", text_color="#000000").pack(anchor="w", padx=20)
entry_c1 = ctk.CTkEntry(scrollable)
entry_c1.pack(fill="x", padx=20, pady=5)
ctk.CTkLabel(scrollable, text="Ciphertext c2:", text_color="#000000").pack(anchor="w", padx=20)
entry_c2 = ctk.CTkEntry(scrollable)
entry_c2.pack(fill="x", padx=20, pady=5)
ctk.CTkButton(scrollable, text="Decrypt with Private Key", command=decrypt_elg, text_color="#000000").pack(pady=5)
ctk.CTkLabel(scrollable, text="Decrypted Plaintext:", text_color="#000000").pack(anchor="w", padx=20)
entry_dec = ctk.CTkEntry(scrollable)
entry_dec.pack(fill="x", padx=20, pady=5)

# Attacker Guess Section
ctk.CTkLabel(scrollable, text="Attacker Guess", font=("Arial", 14, "bold"), text_color="#000000").pack(pady=10)
ctk.CTkLabel(scrollable, text="Guess Plaintext:", text_color="#000000").pack(anchor="w", padx=20)
entry_guess = ctk.CTkEntry(scrollable)
entry_guess.pack(fill="x", padx=20, pady=5)
ctk.CTkButton(scrollable, text="Submit Guess", command=attacker_guess_elg, text_color="#000000").pack(pady=5)
ctk.CTkLabel(scrollable, text="Actual Plaintext:", text_color="#000000").pack(anchor="w", padx=20)
entry_guess_dec = ctk.CTkEntry(scrollable)
entry_guess_dec.pack(fill="x", padx=20, pady=5)

root.mainloop()
