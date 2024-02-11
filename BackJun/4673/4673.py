def makeNum(n):
    sum = n
    i = n
    while i > 0 :
        sum += i % 10
        i = i // 10
    return sum

def digit(n) :
    i = n
    cnt = 0
    while i > 0 :
        cnt += 1
        i = i // 10
    return cnt

def isSelf(n) :
    for i in range(n - 10 * digit(n), n) :
        if makeNum(i) == n :
            return False
    return True

for i in range(1, 10000) :
    if isSelf(i) :
        print(i)