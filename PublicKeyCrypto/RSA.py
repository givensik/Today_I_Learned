import tkinter as tk
from tkinter import messagebox
import random
from sympy import isprime, mod_inverse

def generate_prime(start=100, end=300):
    while True:
        p = random.randint(start, end)
        if isprime(p):
            return p

def generate_keys():
    p = generate_prime()
    q = generate_prime()
    while p == q:
        q = generate_prime()
    n = p * q
    phi = (p - 1) * (q - 1)
    e = 65537
    if phi % e == 0:
        for cand in range(3, phi, 2):
            if isprime(cand) and phi % cand != 0:
                e = cand
                break
    d = mod_inverse(e, phi)
    return (e, n), (d, n), p, q

def encrypt(text, pubkey):
    e, n = pubkey
    return [pow(ord(char), e, n) for char in text]

def decrypt(cipher_list, privkey):
    d, n = privkey
    return ''.join([chr(pow(c, d, n)) for c in cipher_list])

class RSAGUI:
    def __init__(self, master):
        self.master = master
        master.title("RSA 암호화 시스템")

        self.pubkey = None
        self.privkey = None

        tk.Button(master, text="키 생성", command=self.generate_keys_gui).grid(row=1, column=0, columnspan=4, pady=5)

        tk.Label(master, text="Plain Text:").grid(row=2, column=0, sticky='e')
        self.plain_entry = tk.Entry(master, width=50)
        self.plain_entry.grid(row=2, column=1, columnspan=2)
        tk.Button(master, text="Encrypt", command=self.encrypt_text).grid(row=2, column=3)

        tk.Label(master, text="Cipher Text:").grid(row=3, column=0, sticky='e')
        self.cipher_entry = tk.Entry(master, width=50)
        self.cipher_entry.grid(row=3, column=1, columnspan=2)
        tk.Button(master, text="Decrypt", command=self.decrypt_text).grid(row=3, column=3)

        tk.Label(master, text="출력 결과:").grid(row=4, column=0, columnspan=4)
        self.output_text = tk.Text(master, height=20, width=80)
        self.output_text.grid(row=5, column=0, columnspan=4, padx=10, pady=10)

    def generate_keys_gui(self):
        self.pubkey, self.privkey, p, q = generate_keys()
        self.print_output(f"[키 생성 완료]\n p = {p}, q = {q}\n 공개키 (e, n) = {self.pubkey}\n 개인키 (d, n) = {self.privkey}\n")

    def encrypt_text(self):
        if not self.pubkey:
            messagebox.showerror("오류", "먼저 키를 생성하세요.")
            return
        plain = self.plain_entry.get()
        cipher = encrypt(plain, self.pubkey)
        self.print_output(f"[암호화]\n 입력 평문: {plain}\n 암호문: {' '.join(map(str, cipher))}\n")

    def decrypt_text(self):
        if not self.privkey:
            messagebox.showerror("오류", "먼저 키를 생성하세요.")
            return
        cipher_str = self.cipher_entry.get().strip()
        try:
            cipher_list = list(map(int, cipher_str.split()))
            decrypted = decrypt(cipher_list, self.privkey)
            self.print_output(f"[복호화]\n 입력 암호문: {cipher_str}\n 복호문: {decrypted}\n")
        except:
            messagebox.showerror("오류", "암호문 형식이 잘못되었습니다.")

    def print_output(self, text):
        self.output_text.insert(tk.END, text + "\n")
        self.output_text.see(tk.END)

if __name__ == "__main__":
    root = tk.Tk()
    app = RSAGUI(root)
    root.mainloop()
