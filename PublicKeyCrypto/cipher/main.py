import tkinter as tk
from tkinter import ttk, messagebox
import bf
import string
import random
import hill
import assgin1

# integrate encryption
def encrypt():
    if check1_var.get() : # Shift Cipher Checkbox is checked
        shift_cipher_encrypt()
    elif check2_var.get() : # substitution Cipher Checkbox is checked
        substitution_cipher_encrypt()
    elif check3_var.get() : # vigener Cipher Checkbox is checked
        vigener_cipher_encrypt()
    elif check5_var.get() : # vigener Cipher Checkbox is checked
        playfair_encrypt()
    elif check8_var.get() : # hillcipher
        hillcipher_encrypt()

def decrypt():
    if check1_var.get() : # Shift Cipher Checkbox is checked
        shift_cipher_decrypt()
    elif check2_var.get() : # substitution Cipher Checkbox is checked
        substitution_cipher_decrypt()
    elif check3_var.get() : # vigener Cipher Checkbox is checked
        vigenere_decrypt()
    elif check5_var.get() : # vigener Cipher Checkbox is checked
        playfair_decrypt()
    elif check8_var.get() : # hillcipher is checked
        hillcipher_decrypt()

def bruteforce():
    if check4_var.get() : # Shift Cipher Checkbox is checked
        bf_shift()
    elif check7_var.get() : # substitution Cipher Checkbox is checked
        frequency_analysis()
    elif check6_var.get() : # vigener Cipher Checkbox is checked
        frequency_analysis()
    elif check9_var.get() :
        hillattack()
    else :
        messagebox.showerror("Error","Choose Method")


def shift_cipher_encrypt():
    pvalue = pinput.get() # get plaintext value
    key = kvalue.get()
    output = assgin1.shift_cipher_encrypt(pvalue, key)
    ovalue.set(output)
    cvalue.set(output)

def shift_cipher_decrypt():
    pvalue = ovalue.get() # get plaintext value
    key = kvalue.get()
    output = assgin1.shift_cipher_decrypt(pvalue, key)
    dvalue.set(output)
    
# for make random substitution key
def shuffle() :
    key = assgin1.generate_substitution_cipher()
    key = "".join(key)
    kvalue.set(key)
# for encrypt substitution cipher
def substitution_cipher_encrypt():
    plaintext = pinput.get()
    key = kvalue.get()
    cipher = assgin1.substitution_cipher(plaintext, key)
    
    ovalue.set(cipher)
    cvalue.set(cipher)

# for decrypt substitution cipher
def substitution_cipher_decrypt():
    ciphertext = cvalue.get()
    key = kvalue.get()
    
    decrypt = assgin1.substitution_cipher_decrypt(ciphertext, key)
    dvalue.set(decrypt)
    
# function to vigenere_cipher
def vigener_cipher_encrypt():
    plaintext = pinput.get()
    key = kvalue.get()
    output = assgin1.vigenere_encrypt(plaintext, key)
    ovalue.set(output)
    cvalue.set(output)
    
def vigenere_decrypt():
    key = kvalue.get().lower()  # 키는 소문자로 통일
    ciphertext = ovalue.get()
    output = assgin1.vigenere_decrypt(ciphertext, key)
    dvalue.set(output)
    

# encrypt playfair encryption
def playfair_encrypt():
    # get plain, key value
    key = kvalue.get()
    plaintext = pinput.get()
    
    encrypted = bf.playfair_tech(plaintext, key)
    ovalue.set("".join(encrypted))
    cvalue.set("".join(encrypted))
    
# decrypt playfair encryption
def playfair_decrypt():
    # get plain, key value
    key = kvalue.get()
    output = cvalue.get()
    
    decrypted = bf.playfair_decrypt(output, key)
    dvalue.set("".join(decrypted))
    
# decrypt by frequency analysis
def frequency_analysis():
    cipher = bfinput.get()
    result = bf.bf_freq_analysis(cipher)
    bfvalue.set(result)

def hillcipher_encrypt():
    
    if check_key1.get() : # Shift Cipher Checkbox is checked
        key = hill.generate_3x3_key()
    elif check_key2.get() :
        key = hill.generate_5x5_key()
    else :
        key = hill.generate_3x3_key()    
    kvalue.set(key)
    plaintext = pinput.get()
    output = hill.hill_cipher_encrypt(plaintext, key)
    ovalue.set(output)
    cvalue.set(output)

def hillcipher_decrypt():
    key = eval(kvalue.get()) # be aware it is not secure function
    cipher = cvalue.get()
    output = hill.hill_cipher_decrypt(cipher, key)
    dvalue.set(output)
    detk.set(hill.mod_matrix_inverse(key))

def hillattack():
    cipher = bfinput.get()
    
    bfvalue.set(hill.attack_hill_cipher(cipher))
    

# for only one checkbox    
def on_check(var, checkbox_vars): # check only one checkbox
    for v in checkbox_vars:
        if v != var: # only var is checked
            v.set(False)


def bf_shift():
    cipher = bfinput.get()
    output = bf.brute_force_shift_cipher(cipher)
    # print("output :", output)
    bfvalue.set("".join(output))






# Create main window
root = tk.Tk()
root.title("Assignment")  # 창 제목
root.geometry("800x800")       # 초기 크기 설정


# variables for save checkbox states if variable is true, it means that checkbox is checked.
check1_var = tk.BooleanVar(value=False)  # default False 
check2_var = tk.BooleanVar(value=False)  
check3_var = tk.BooleanVar(value=False) 

# checkbox variables for bruteforce methods 
check4_var = tk.BooleanVar(value=False)  
check5_var = tk.BooleanVar(value=False)  
check6_var = tk.BooleanVar(value=False)  
check7_var = tk.BooleanVar(value=False)
check8_var = tk.BooleanVar(value=False)
check9_var = tk.BooleanVar(value=False)

check_key1 = tk.BooleanVar(value=False)
check_key2 = tk.BooleanVar(value=False)


# checkbox for selecting crypto type GUI
check1 = ttk.Checkbutton(
    root,
    text="Shift Cipher",
    variable=check1_var,
    command=lambda: on_check(check1_var, [check1_var, check2_var, check3_var, check5_var, check8_var])  # 상태 변경시 호출될 함수
)
check1.grid(row=0, column=0, sticky=tk.W, padx=5, pady=5)

check2 = ttk.Checkbutton(
    root,
    text="Substitution Cipher",
    variable=check2_var,
    command=lambda: on_check(check2_var, [check1_var, check2_var, check3_var, check5_var, check8_var]) # 상태 변경시 호출될 함수
)
    
check2.grid(row=0, column=1, sticky=tk.W, padx=5, pady=5)

check3 = ttk.Checkbutton(
    root,
    text="Viginere Cipher",
    variable=check3_var,
    command=lambda: on_check(check3_var, [check1_var, check2_var, check3_var, check5_var, check8_var])
)

check3.grid(row=0, column=2, sticky=tk.W, padx=5, pady=5)

check4 = ttk.Checkbutton(
    root,
    text="ShiftCipher BF",
    variable=check4_var,
    command=lambda: on_check(check4_var, [check4_var, check6_var, check7_var, check9_var])
)

check4.grid(row=15, column=0, sticky=tk.W, padx=5, pady=5)

check5 = ttk.Checkbutton(
    root,
    text="Playfair Technic",
    variable=check5_var,
    command=lambda: on_check(check5_var, [check1_var, check2_var, check3_var, check5_var, check8_var])
)

check5.grid(row=0, column=3, sticky=tk.W, padx=5, pady=5)

check6 = ttk.Checkbutton(
    root,
    text="Frequency Analysis BF",
    variable=check6_var,
    command=lambda: on_check(check6_var, [check4_var, check6_var, check7_var, check9_var])
)

check6.grid(row=15, column=1, sticky=tk.W, padx=5, pady=5)

check7 = ttk.Checkbutton(
    root,
    text="Playfair technique BF",
    variable=check7_var,
    command=lambda: on_check(check7_var, [check4_var, check6_var, check7_var, check9_var])
)

check7.grid(row=15, column=2, sticky=tk.W, padx=5, pady=5)

check8 = ttk.Checkbutton(
    root,
    text="Hillcipher",
    variable=check8_var,
    command=lambda: on_check(check8_var, [check1_var, check2_var, check3_var, check5_var, check8_var])
)

check8.grid(row=0, column=4, sticky=tk.W, padx=5, pady=5)

check9 = ttk.Checkbutton(
    root,
    text="Hillcipher BF",
    variable=check9_var,
    command=lambda: on_check(check9_var, [check4_var, check6_var, check7_var, check9_var])
)

check9.grid(row=15, column=3, sticky=tk.W, padx=5, pady=5)

hillcheck = ttk.Checkbutton(
    root,
    text="Key : 3",
    variable=check_key1,
    command=lambda: on_check(check_key1, [check_key1, check_key2])
)

hillcheck.grid(row=2, column=3, sticky=tk.W, padx=5, pady=5)

hillcheck = ttk.Checkbutton(
    root,
    text="Key : 5",
    variable=check_key2,
    command=lambda: on_check(check_key2, [check_key1, check_key2])
)

hillcheck.grid(row=2, column=4, sticky=tk.W, padx=5, pady=5)


# shift ciphers variables
pinput = tk.StringVar() # 입력 값 저장하는 변수
sinput = tk.StringVar() # 입력 값 저장하는 변수
ovalue = tk.StringVar() # 입력 값 저장
ovalue.set("output value")
cvalue = tk.StringVar() # 암호문 저장
dvalue = tk.StringVar() # 복호문 저장
detk = tk.StringVar() # for Det Key

# substituition cipher variables
kvalue = tk.StringVar() # key 값

bfvalue = tk.StringVar() # bf 결과
bfinput = tk.StringVar() # bf input


# plain text GUI
plaintext = ttk.Label(root, text="Plain text")
plaintext.grid(row=1, column=0)    
plaintext_input = ttk.Entry(root, textvariable=pinput) # plain text 입력 칸
plaintext_input.grid(row=1, column=1)

# key value GUI
key_label = ttk.Label(root, text="key")
key_label.grid(row=2, column=0)
key_input = ttk.Entry(root, textvariable=kvalue) # key value
key_input.grid(row=2, column=1)

# output GUI
output_label = ttk.Label(root, text="Output") 
output_label.grid(row = 3, column= 0)
output_text = ttk.Label(root, textvariable=ovalue)
output_text.grid(row = 3, column= 1)

# cipher text GUI
ciphertext = ttk.Label(root, text="Cipher text")
ciphertext.grid(row=7, column=0)    
ciphertext_input = ttk.Entry(root, textvariable=cvalue) # plain text 입력 칸
ciphertext_input.grid(row=7, column=1)

# decrypt output value GUI
output_label = ttk.Label(root, text="Output") 
output_label.grid(row = 8, column= 0)
output_text = ttk.Label(root, textvariable=dvalue)
output_text.grid(row = 8, column= 1)

# Det Key value 
output_label = ttk.Label(root, text="Det K") 
output_label.grid(row = 8, column= 2)
output_text = ttk.Label(root, textvariable=detk)
output_text.grid(row = 8, column= 3)

# encrypt button GUI
btn1 = ttk.Button(root, text="Encrypt", command = encrypt)
btn1.grid(row=4, column=0, padx=10, pady=5, sticky="w")

# shuffle button GUI
shuffle_btn = ttk.Button(root, text="Shuffle", command = shuffle )
shuffle_btn.grid(row=4, column=1, padx=10, pady=5, sticky="w")

# decrypt button GUI
btn2 = ttk.Button(root, text="Decrypt", command = decrypt)
btn2.grid(row=7, column=2, padx=10, pady=5, sticky="w")

# bruteforce button GUI
btn3 = ttk.Button(root, text="Bruteforce", command=bruteforce )
btn3.grid(row=20, column=2, padx=10, pady=5, sticky="w")

# Bruteforce input GUI
plaintext = ttk.Label(root, text="Bruteforce input")
plaintext.grid(row=20, column=0)    
plaintext_input = ttk.Entry(root, textvariable=bfinput) # plain text 입력 칸
plaintext_input.grid(row=20, column=1)

# Bruteforce result GUI
output_label = ttk.Label(root, text="Bruteforce Result") 
output_label.grid(row = 21, column= 0)
output_text = ttk.Label(root, textvariable=bfvalue)
output_text.grid(row = 21, column= 1)


if __name__ == "__main__":
    root.mainloop()  # 이 줄이 없으면 창이 바로 닫힘!
    
    
    



# playfair

# cryptography my dear love 

# best maths