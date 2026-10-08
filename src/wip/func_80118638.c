// FUNC 80118638 468 X000
// r11: score 18; diffs: case1/2 game loads b4,bc,c4,cc into a0,a1,v0,v1 first then adds; final neg uses lh (ours lhu).
#define H(o) (*(unsigned short *)(p + (o)))
#define SH(o) (*(short *)(p + (o)))
extern unsigned int FUN_8001f9e0(void);
extern int FUN_800202b4(char *);

void func_80118638(char *p)
{
    short v;
    unsigned short t;
    short a, b, c, d;
    switch ((unsigned char)p[5]) {
    case 0:
        p[5] = FUN_8001f9e0() & 1;
        H(0x20) = 0xa0;
        H(0x80) = 0x80;
        p[0xe] = 0;
        H(0x22) = 0;
        SH(0x7c) = -2;
        return;
    case 1:
        if (FUN_800202b4(p) == 0)
            return;
        t = H(0x20) - 1;
        H(0x20) = t;
        if ((short)t == -1) {
            H(0x20) = 0x140;
            p[5]++;
            return;
        }
        if (t & 1) {
            b = H(0xbc) - 1;
            a = H(0xb4) - 1;
            c = H(0xc4) + 1;
            d = H(0xcc) + 1;
            goto store;
        }
        break;
    case 2:
        if (FUN_800202b4(p) == 0)
            return;
        t = H(0x20) - 1;
        H(0x20) = t;
        if ((short)t == -1) {
            H(0x20) = 0x140;
            p[5]--;
            return;
        }
        if (t & 1) {
            b = H(0xbc) + 1;
            a = H(0xb4) + 1;
            c = H(0xc4) - 1;
            d = H(0xcc) - 1;
            
        store:
            H(0xb4) = a;
            H(0xbc) = b;
            H(0xc4) = c;
            H(0xcc) = d;
        }
        break;
    default:
        return;
    }
    **(int **)(p + 0x40) += SH(0x80) << 8;
    v = H(0x80) + H(0x7c);
    H(0x80) = v;
    if ((unsigned short)(v + 0x80) < 0x101)
        return;
    v = SH(0x7c); SH(0x7c) = -v;
}
