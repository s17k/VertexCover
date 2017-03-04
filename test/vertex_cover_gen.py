import random
n = random.randint(1,3)
m = random.randint(1, n*(n-1)//2)

print str(n) + ' ' + str(m)

for i in range(m):
    a = b = 1
    while a == b:
        a = random.randint(1,n)
        b = random.randint(1,n)
    print str(a) + ' ' + str(b)

print random.randint(0,5)
