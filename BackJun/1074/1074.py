N, Y, X = map(int, input().split())

cnt = 0

def find(y,x,N):
    #print(y,x,N)
    if N == 1 :
        return 0
    
    d = int(N/2)
    
    if Y < y + d and X < x + d :
        return find(y,x,int(N/2))
    elif Y < y + d and X < x + 2*d :
        return find(y,x+d,int(N/2)) + int(N ** 2/ 4) * 1
    elif Y < y + 2*d and X < x + d :
        return find(y+d,x,int(N/2)) + int(N ** 2/ 4) * 2
    elif Y < y + 2*d and X < x + 2*d :
        return find(y+d,x+d,int(N/2)) + int(N ** 2/ 4) * 3
    
print(find(0,0,2**N))