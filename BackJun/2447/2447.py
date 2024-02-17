N = int(input())

star = [[0]*(N+1) for i in range(N+1)]

def writeStar(y,x,L) :
    if L == 1:
        star[y][x] = 1
    else :
        d = int(L/3)
        for i in range(0,3) :
            for j in range(0,3) :
                if i == 1 and j == 1 :
                    continue
                writeStar(y+d*i, x +d*j, d)

def printStar() :
    for i in range(1, N+1) :
        for j in range(1, N+1) :
            if star[j][i] == 1 :
                print('*', end = "")
            else :
                print(' ', end='')
        print("")

writeStar(1,1,N)
printStar()
