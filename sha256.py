#常量初始化
h=[0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19]
k=[0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2]
#信息预处理（附加填充比特、附加长度值）
def padding(x):
    temp=len(x)%64
    if temp<56:
        res=55-temp
    else:
        res=119-temp
    return bytes(x, "ascii") + b"\x80" + (b"\x00" * res) + ((len(x) << 3).to_bytes(8, "big"))
def rightmove(x,b):
     return ((x >> b) | (x << (32 - b))) & (pow(2,32)-1)
def xor1(a,b,c):
    return (a & b) ^ (~a & c)
def xor2(a,b,c):
    return (a & b) ^ (a & c) ^ (b & c)
def compress(w,k,A,B,C,D,E,F,G,H):
    s0=rightmove(E,6)^rightmove(E,11)^rightmove(E,25)
    s1=rightmove(A,2)^rightmove(A,13)^rightmove(A,22)
    t0=H + s0+ xor1(E,F,G) + w + k
    return (t0 + s1 + xor2(A,B,C)) & ((2**32)-1), A, B, C, (D + t0) & (pow(2,32)-1), E, F, G
def sha256(mes):
    mes=padding(mes)
    length=len(mes)
    for i in range(0,length,64):
        M=mes[i:i+64]
        W=[int.from_bytes(M[e: e + 4], "big") for e in range(0, 64, 4)] + ([0] * 48)
        for j in range(16,64):
            s0 = rightmove(W[j-15], 7) ^ rightmove(W[j-15], 18) ^ (W[j-15] >> 3)
            s1 = rightmove(W[j-2], 17) ^ rightmove(W[j-2], 19) ^ (W[j-2] >> 10)
            W[j] = (W[j-16] + s0 + W[i-7] + s1) & (pow(2,32)-1)
        A,B,C,D,E,F,G,H=h
        for l in range(64):
            A,B,C,D,E,F,G,H=compress(W[l], k[l], A, B, C, D, E, F, G, H)
    return "".join(format(h, "0x") for h in b"".join(
            d.to_bytes(4, "big") for d in [(x + y) & ((2**32)-1) for x, y in zip(h, (A, B, C, D, E, F, G, H))]))

if __name__ == '__main__':
    print(sha256("ryr1234567"))
