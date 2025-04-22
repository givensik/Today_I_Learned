from math import gcd, isqrt

def is_prime(n):
    if n < 2:
        return False
    for i in range(2, isqrt(n) + 1):
        if n % i == 0:
            return False
    return True

def is_coprime(a, b):
    return gcd(a, b) == 1

def analyze_numbers(numbers):
    print("Prime Check:")
    for n in numbers:
        if is_prime(n):
            print(f"  {n}: Prime")
        else:
            print(f"  {n}: Not Prime")

    print("\nCoprime Pairs:")
    found = False
    for i in range(len(numbers)):
        for j in range(i + 1, len(numbers)):
            a, b = numbers[i], numbers[j]
            if is_coprime(a, b):
                print(f"  ({a}, {b}) → Coprime")
                found = True
    if not found:
        print("  No coprime pairs found.")



if __name__ == "__main__":
    user_input = input("정수들을 공백으로 구분해 입력하세요 (예: 8 9 13 15): ")
    numbers = list(map(int, user_input.strip().split()))
    analyze_numbers(numbers)