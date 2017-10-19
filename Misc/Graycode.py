n, x, y = input().split()
n = int(n)

def trans(s):
    cur = 0
    x = ''
    for c in s:
        c = int(c)
        nc = c^cur
        cur = nc
        x += str(nc)
    return int(x, 2)

print(trans(y) - trans(x) - 1)