import tkinter as tk
from tkinter import messagebox
from sympy import randprime, mod_inverse
from random import randint

class ElGamalGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("ElGamal")

        tk.Label(root, text="Key size (bits):").grid(row=0, column=0, padx=5, pady=5)
        self.entry_keysize = tk.Entry(root, width=10)
        self.entry_keysize.insert(0, "512")
        self.entry_keysize.grid(row=0, column=1, padx=5, pady=5)

        self.btn_generate_key = tk.Button(root, text="Generate p & g", command=self.generate_keys)
        self.btn_generate_key.grid(row=0, column=2, padx=5, pady=5)

        self.text_area = tk.Text(root, width=100, height=30)
        self.text_area.grid(row=1, column=0, columnspan=3, padx=10, pady=10)

        tk.Label(root, text="Message (number):").grid(row=2, column=0, padx=5, pady=5)
        self.entry_message = tk.Entry(root, width=50)
        self.entry_message.grid(row=2, column=1, padx=5, pady=5)
        self.btn_encrypt = tk.Button(root, text="Encrypt", command=self.encrypt_message)
        self.btn_encrypt.grid(row=2, column=2, padx=5, pady=5)

        tk.Label(root, text="Ciphertext (Y1,Y2):").grid(row=3, column=0, padx=5, pady=5)
        self.entry_cipher = tk.Entry(root, width=50)
        self.entry_cipher.grid(row=3, column=1, padx=5, pady=5)
        self.btn_decrypt = tk.Button(root, text="Decrypt", command=self.decrypt_message)
        self.btn_decrypt.grid(row=3, column=2, padx=5, pady=5)

        self.btn_attack = tk.Button(root, text="Attacker", command=self.attack_simulation)
        self.btn_attack.grid(row=4, column=0, columnspan=3, pady=10)

    def generate_keys(self):
        self.text_area.delete(1.0, tk.END)
        try:
            bits = int(self.entry_keysize.get())
            if bits < 3:
                raise ValueError("Key size must be ≥ 3")

            self.text_area.insert(tk.END, f"Generating {bits}-bit prime number...\n")
            self.p = randprime(2**(bits - 1), 2**bits)
            self.g = randint(2, self.p - 2)
            self.d = randint(2, self.p - 2)
            self.e = pow(self.g, self.d, self.p)
            self.text_area.insert(tk.END, f"p (prime): {self.p}\n")
            self.text_area.insert(tk.END, f"g: {self.g}\n")
            self.text_area.insert(tk.END, f"Private key d: {self.d}\n")
            self.text_area.insert(tk.END, f"Public key e = g^d mod p: {self.e}\n")
        except Exception as e:
            messagebox.showerror("Error", f"Key generation failed:\n{str(e)}")

    def encrypt_message(self):
        try:
            if not all(hasattr(self, attr) for attr in ['p', 'g', 'e']):
                messagebox.showwarning("Missing Keys", "Please generate keys first.")
                return

            message = self.entry_message.get().strip()
            if not message.isdigit():
                messagebox.showwarning("Invalid Input", "Message must be a non-negative integer.")
                return

            M = int(message)
            if M >= self.p:
                messagebox.showwarning("Input Too Large", "Message must be less than prime p.")
                return

            k = randint(2, self.p - 2)
            Y1 = pow(self.g, k, self.p)
            Y2 = (M * pow(self.e, k, self.p)) % self.p

            self.Y1 = Y1
            self.Y2 = Y2

            self.text_area.insert(tk.END, f"\n--- Encryption ---\n")
            self.text_area.insert(tk.END, f"Random k: {k}\n")
            self.text_area.insert(tk.END, f"Y1 = g^k mod p = {Y1}\n")
            self.text_area.insert(tk.END, f"Y2 = M * e^k mod p = {Y2}\n")
            self.text_area.insert(tk.END, f"Ciphertext: ({Y1}, {Y2})\n")

        except Exception as e:
            messagebox.showerror("Error", f"Encryption failed:\n{str(e)}")

    def decrypt_message(self):
        try:
            if not all(hasattr(self, attr) for attr in ['p', 'd']):
                messagebox.showwarning("Missing Keys", "Please generate keys first.")
                return

            ciphertext = self.entry_cipher.get().strip()
            if not ciphertext or ',' not in ciphertext:
                messagebox.showwarning("Invalid Format", "Ciphertext must be in format: Y1,Y2")
                return

            try:
                Y1_str, Y2_str = ciphertext.split(',')
                Y1 = int(Y1_str.strip())
                Y2 = int(Y2_str.strip())
            except:
                messagebox.showwarning("Invalid Input", "Could not parse ciphertext. Use integers.")
                return

            s = pow(Y1, self.d, self.p)
            s_inv = mod_inverse(s, self.p)
            M = (Y2 * s_inv) % self.p

            self.text_area.insert(tk.END, f"\n--- Decryption ---\n")
            self.text_area.insert(tk.END, f"s = Y1^d mod p = {s}\n")
            self.text_area.insert(tk.END, f"s^(-1) mod p = {s_inv}\n")
            self.text_area.insert(tk.END, f"Recovered Plaintext: {M}\n")

        except Exception as e:
            messagebox.showerror("Error", f"Decryption failed:\n{str(e)}")

    def attack_simulation(self):
        try:
            self.text_area.insert(tk.END, "\n--- Attacker Simulation ---\n")
            if not all(hasattr(self, attr) for attr in ['p', 'g', 'e', 'Y1', 'Y2']):
                self.text_area.insert(tk.END, "Missing public key or ciphertext.\n")
                return

            self.text_area.insert(tk.END, f"Known:\np = {self.p}\ng = {self.g}\ne = {self.e}\n")
            self.text_area.insert(tk.END, f"Y1 = {self.Y1}, Y2 = {self.Y2}\n")
            self.text_area.insert(tk.END, "Trying to find k such that g^k ≡ Y1 (mod p)...\n")

            found = False
            limit = min(100000, self.p)
            for k_guess in range(1, limit):
                if pow(self.g, k_guess, self.p) == self.Y1:
                    recovered_k = k_guess
                    recovered_e_k = pow(self.e, recovered_k, self.p)
                    recovered_m = (self.Y2 * mod_inverse(recovered_e_k, self.p)) % self.p

                    self.text_area.insert(tk.END, f"\nGuessed k = {recovered_k}\n")
                    self.text_area.insert(tk.END, f"Recovered plaintext: {recovered_m}\n")
                    found = True
                    break

            if not found:
                self.text_area.insert(tk.END, "Failed to guess k (Discrete Log). Cannot recover message.\n")

        except Exception as e:
            messagebox.showerror("Error", f"Attack simulation failed:\n{str(e)}")

if __name__ == "__main__":
    root = tk.Tk()
    app = ElGamalGUI(root)
    root.mainloop()
