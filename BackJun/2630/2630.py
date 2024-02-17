N = int(input())

cnt0 = 0
cnt1 = 0
Paper = []

for i in range(N):
	Paper.append(list(map(int,input().split())))

def count(p):
    global cnt0, cnt1
    if p == 0 :
        cnt0 += 1
    elif p != -1 :
        cnt1 += 1
    #print("cnt0, cnt1 :", cnt0, cnt1)
    
def cutPaper(y, x, N) :
    #print(y,x,N)
    if N == 1 :
        return Paper[y][x]
    else :
        d = int(N/2)
        p1 = cutPaper(y,x,d)
        p2 = cutPaper(y+d, x, d)
        p3 = cutPaper(y, x+d, d)
        p4 = cutPaper(y+d, x+d, d)
        #print(p1,p2,p3,p4)
        sum = p1 + p2 + p3 + p4
        
        if p1 >= 0 and p2 >= 0 and p3 >= 0 and p4 >=0 :
            if p1 == p2 and p1 == p3 and p1 == p4 and p2 == p3 and p3 == p4 :
                return int(sum / 4)
        count(p1)
        count(p2)
        count(p3)
        count(p4)
        return -1
  
result = cutPaper(0,0,N)

if result == 0 :
    cnt0 += 1
elif result != -1 :
    cnt1 += 1

print(cnt0)
print(cnt1)
        