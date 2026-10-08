// FUNC 8002a0fc 348 MAIN0
// MATCHING 8002a0fc 348
typedef struct {
    unsigned char b0, b1, b2, b3;
    char p4[0x34 - 4];
    unsigned short *p34;
    char p38[0x44 - 0x38];
    unsigned short f44;
} S;
typedef struct { short v; } SV;
extern SV D_1F8000E6;
extern short D_1F8000E6s;
extern unsigned short D_1F80016A;
extern unsigned short D_1F8000F2;
extern unsigned short D_1F80016E;
extern unsigned char D_8009C966;
extern unsigned char D_8009CDA6;
extern unsigned char D_8009C93F;
extern unsigned char D_8009C93E;
extern unsigned char D_8009C942;
extern void FUN_80027c74(S *o);
extern void func_8002795C(S *o);

void func_8002A0FC(S *o)
{
    short v;
    o->f44 = 0;
    v = D_1F8000E6.v;
    if (v != 0) {
        if (v > 0) {
            v -= 2;
            D_1F8000E6s = v;
            if (v < 0) D_1F8000E6s = 0;
        } else {
            v += 2;
            D_1F8000E6s = v;
            if (v > 0) D_1F8000E6s = 0;
        }
    }
    FUN_80027c74(o);
    func_8002795C(o);
    if ((unsigned short)(o->p34[1] - D_1F80016A + 0x8c) < 0x118 || (o->f44 & 3)) {
        if ((unsigned short)(D_1F8000F2 - D_1F80016E + 0xb4) < 0x168 || (o->f44 & 0xc)) {
            o->b3 = 1;
            if (D_8009C966 == 3 || D_8009CDA6 == 0xff) o->b3 = 0;
            D_8009C93F = 0;
            D_8009C93E = 0;
            D_8009C942 = 0;
        }
    }
}
