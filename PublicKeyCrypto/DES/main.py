import tkinter as tk
from tkinter import ttk, messagebox

import DEStandard

# integrate encryption
def encrypt():
    if check_method1.get():
        DES_Enc()
    

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
    
    


def decrypt():
    if check_method1.get():
        DES_Dec()


# DES decryption function
def DES_Dec():
    c = ciphertext.get()
    k = key.get()
    p = DEStandard.des_decrypt(c, k)
    decrypt_value.set(p)

# Create main window
root = tk.Tk()
root.title("Assignment")  # 창 제목
root.geometry("800x800")       # 초기 크기 설정


### define variables
# value variables
plaintext = tk.StringVar()
key = tk.StringVar()
ciphertext = tk.StringVar()
decrypt_value = tk.StringVar()


# checkbox variables
check_method1 = tk.BooleanVar(value=False)  # default False 




# for checkbox    
def on_check(var, checkbox_vars): # check only one checkbox
    for v in checkbox_vars:
        if v != var: # only var is checked
            v.set(False)
            

check = ttk.Checkbutton(
    root,
    text="DES",
    variable=check_method1,
    command=lambda: on_check(check_method1, [check_method1])  # 상태 변경시 호출될 함수
)
check.grid(row=0, column=0, sticky=tk.W, padx=5, pady=5)



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
btn.grid(row=10, column=0, padx=10, pady=5, sticky="w")






if __name__ == "__main__":
    root.mainloop()  # 이 줄이 없으면 창이 바로 닫힘!