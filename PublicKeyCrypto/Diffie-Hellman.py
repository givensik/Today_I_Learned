import tkinter as tk
from tkinter import messagebox
from Crypto.Util import number
import random

def caesar_encrypt(text, key):
    return ''.join(chr((ord(c) + key) % 256) for c in text)

def caesar_decrypt(cipher, key):
    return ''.join(chr((ord(c) - key) % 256) for c in cipher)

class DiffieHellmanGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Diffie-Hellman Key Exchange GUI")

        tk.Label(root, text="Enter Key Size (bits):").grid(row=0, column=0)
        self.entry_keysize = tk.Entry(root)
        self.entry_keysize.grid(row=0, column=1)
        self.entry_keysize.insert(0, "2048")

        self.btn_generate = tk.Button(root, text="Generate p, g", command=self.generate_pg)
        self.btn_generate.grid(row=0, column=2, padx=10)

        self.text_area = tk.Text(root, width=100, height=30)
        self.text_area.grid(row=1, column=0, columnspan=3, pady=10)

        self.frame_users = tk.Frame(root)
        self.frame_users.grid(row=2, column=0, columnspan=3)

        self.frame_alice = tk.Frame(self.frame_users)
        self.frame_alice.grid(row=0, column=0, padx=30)

        tk.Label(self.frame_alice, text="Alice's Message:").pack()
        self.entry_alice = tk.Entry(self.frame_alice, width=40)
        self.entry_alice.pack()
        self.btn_encrypt = tk.Button(self.frame_alice, text="Encrypt", command=self.encrypt)
        self.btn_encrypt.pack(pady=5)

        self.frame_bob = tk.Frame(self.frame_users)
        self.frame_bob.grid(row=0, column=1, padx=30)

        tk.Label(self.frame_bob, text="Bob's Ciphertext:").pack()
        self.entry_bob = tk.Entry(self.frame_bob, width=40)
        self.entry_bob.pack()
        self.btn_decrypt = tk.Button(self.frame_bob, text="Decrypt", command=self.decrypt)
        self.btn_decrypt.pack(pady=5)

        self.frame_attacker = tk.Frame(self.frame_users)
        self.frame_attacker.grid(row=0, column=2, padx=30)

        self.btn_attack = tk.Button(self.frame_attacker, text="Attacker", command=self.attacker_view)
        self.btn_attack.pack(pady=20)

        self.shared_key_alice = None
        self.shared_key_bob = None
        self.ciphertext = ""

    def generate_pg(self):
        try:
            keysize = int(self.entry_keysize.get())
            if keysize < 3:
                raise ValueError("Key size too small")
        except ValueError:
            messagebox.showerror("입력 오류", "키 사이즈는 3 이상 정수여야 합니다.")
            return

        self.text_area.delete('1.0', tk.END)
        self.text_area.insert(tk.END, f"Generating {keysize}-bit prime...\n\n")
        self.root.update()

        self.p = number.getPrime(keysize)
        self.g = 2 
        self.a = random.randint(2, self.p - 2)
        self.b = random.randint(2, self.p - 2)
        self.A = pow(self.g, self.a, self.p)
        self.B = pow(self.g, self.b, self.p)
        self.shared_key_alice = pow(self.B, self.a, self.p)
        self.shared_key_bob = pow(self.A, self.b, self.p)

        self.text_area.insert(tk.END, f"p ({keysize} bits): {self.p}\n")
        self.text_area.insert(tk.END, f"g: {self.g}\n")
        self.text_area.insert(tk.END, f"Alice's private key: {self.a}\n")
        self.text_area.insert(tk.END, f"Bob's private key: {self.b}\n")
        self.text_area.insert(tk.END, f"Alice's public key: {self.A}\n")
        self.text_area.insert(tk.END, f"Bob's public key: {self.B}\n")
        self.text_area.insert(tk.END, f"Shared key (Alice): {self.shared_key_alice}\n")
        self.text_area.insert(tk.END, f"Shared key (Bob): {self.shared_key_bob}\n")

    def encrypt(self):
        message = self.entry_alice.get()
        if not message:
            messagebox.showwarning("입력 오류", "Alice의 메시지를 입력하세요.")
            return

        if self.shared_key_alice is None:
            messagebox.showerror("오류", "먼저 p와 g를 생성하세요.")
            return

        key = self.shared_key_alice % 256
        self.ciphertext = caesar_encrypt(message, key)

        self.text_area.insert(tk.END, f"\nCiphertext: \"{self.ciphertext}\"\n")

    def decrypt(self):
        ciphertext_input = self.entry_bob.get()
        if not ciphertext_input:
            messagebox.showwarning("입력 오류", "Bob의 Ciphertext를 입력하세요.")
            return

        if self.shared_key_bob is None:
            messagebox.showerror("오류", "먼저 p와 g를 생성하세요.")
            return

        key = self.shared_key_bob % 256
        decrypted = caesar_decrypt(ciphertext_input, key)

        self.text_area.insert(tk.END, f"Recovered plain text: \"{decrypted}\"\n")

    def attacker_view(self):
        if not self.ciphertext:
            messagebox.showwarning("공격자 보기 오류", "Ciphertext가 없습니다. 먼저 암호화 하세요.")
            return

        self.text_area.insert(tk.END, "\n--- Attacker's View ---\n")
        self.text_area.insert(tk.END, f"g: {self.g}\n")
        self.text_area.insert(tk.END, f"p: {self.p}\n")
        self.text_area.insert(tk.END, f"Alice Public Key: {self.A}\n")
        self.text_area.insert(tk.END, f"Bob Public Key: {self.B}\n")
        self.text_area.insert(tk.END, f"Ciphertext: \"{self.ciphertext}\"\n")

        self.text_area.insert(tk.END, "\nAttacker tries to guess shared key by brute-force:\n")
        for guess in range(256):
            attempt = caesar_decrypt(self.ciphertext, guess)
            self.text_area.insert(tk.END, f"Key {guess}: \"{attempt}\"\n")
            if guess >= 10:
                self.text_area.insert(tk.END, "...\n")
                break

root = tk.Tk()
app = DiffieHellmanGUI(root)
root.mainloop()
