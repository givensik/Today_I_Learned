import random
import string

def generate_substitution_cipher():
    # original alphabet
    original_alphabet = list(string.ascii_uppercase) # a-z
    shuffled_alphabet = original_alphabet.copy() # create copy of original alphabet
    random.shuffle(shuffled_alphabet) # suffle

    return shuffled_alphabet

def encrypt_message(message, shuffled_alphabet):
    original_alphabet = list(string.ascii_uppercase)  # original 
    substitution_dict = {original: shuffled for original, shuffled in zip(original_alphabet, shuffled_alphabet)}
    
    encrypted_message = ''.join(substitution_dict.get(char, char) for char in message.upper())
    return encrypted_message

message = ['attack', 'retreat', 'hold current position']

shuffled_alphabet = generate_substitution_cipher()
result = ''.join(shuffled_alphabet)
print("Substitution Cipher Mapping:", result)

encrypted1 = encrypt_message(message[0], shuffled_alphabet)
encrypted2 = encrypt_message(message[1], shuffled_alphabet)
encrypted3 = encrypt_message(message[2], shuffled_alphabet)

print("attack :", encrypted1, "\nretreat :", encrypted2, "\nhold current postition :", encrypted3)