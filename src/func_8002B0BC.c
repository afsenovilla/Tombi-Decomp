// FUNC 8002b0bc 780 MAIN0
// MATCHING 8002b0bc 780
typedef struct S {
    unsigned char b00, b01; char p0[2]; unsigned char b04; char p1[5]; unsigned char b0a; char p2[0x6c - 0xb];
    unsigned short w6c, w6e, w70, w72; char p3[0xa4 - 0x74]; unsigned char ba4; char p4[0xbc - 0xa5];
    unsigned short wbc, wbe, wc0, wc2; char p5[0xce - 0xc4]; unsigned short wce, wd0, wd2;
} S;
extern char D_800B0BB8[];
extern char D_800B0BBA[];
extern unsigned char D_800B0BBC[], D_800B0BBD[], D_800B0BBE[], D_800B0BBF[];
extern unsigned char D_8009CDA2;
extern signed char D_8009D2B0;
extern unsigned char D_8009C93E, D_8009C933;
extern unsigned char D_8009C93F[];
extern unsigned char D_8009C942[];
extern unsigned char D_800A60D6, D_800A60E4;
extern unsigned char D_800A603C, D_800A603D, D_800A603E, D_800A603F;
extern unsigned short acquireSpriteSlot(int);
extern void func_8002B3C8(S *);
extern void ObjListPush_1F800230(S *);
extern void FUN_8005a8a8(int, int, int);
extern void FUN_8001888c(S *);
void func_8002B0BC(S *o)
{
    int x, y, x2, y2, c0, c1, c2, c3;
    unsigned short m;
    switch (o->b04) {
    case 0:
        o->b0a = 0x14;
        o->ba4 = 1;
        o->wc2 = acquireSpriteSlot(o->wc0);
        if (o->b00 == 1) {
            short sl = acquireSpriteSlot(o->wbc);
            int off;
            o->wbe = sl;
            off = sl * 10;
            c1 = D_800B0BBD[off];
            c0 = D_800B0BBC[off];
            c2 = D_800B0BBE[off];
            c3 = D_800B0BBF[off];
            x = c0 << 2;
            y = c1 << 8;
            o->w6e = x | y;
            x2 = x + c2;
            y2 = y + (c3 << 8);
            o->w72 = y | x2;
            o->w6c = x | y2;
            o->w70 = x2 | y2;
        }
        o->b04++;
        break;
    case 1:
        if (D_8009CDA2 != 1)
            func_8002B3C8(o);
        if (o->b00 & 1) {
            o->b01 = 1;
            ObjListPush_1F800230(o);
        }
        break;
    case 2:
        if (o->b00 == 1) {
            int off2 = o->wbe * 10;
            if (--*(short *)(D_800B0BBA + off2) == 0)
                *(short *)(D_800B0BB8 + o->wbe * 10) = -1;
        }
        if (D_8009D2B0 != 3) {
            if (o->wce & 0x7fff) {
                if (o->wd2)
                    FUN_8005a8a8(o->wd2, 0, 3);
                m = o->wce & 0x7fff;
                if (m == 2)
                    break;
                if (o->wd0 == 0 || m == 3) {
                    D_8009C93F[0] = 0;
                    D_8009C942[0] = 0;
                    if ((o->wce & 0x7fff) != 4 && (D_800A60D6 == 0 || D_800A60E4 < 2)) {
                        D_800A603C = 1;
                        D_800A603D = 0;
                        D_800A603E = 0;
                        D_800A603F = 0;
                    } else {
                        D_8009C93E = 0;
                    }
                }
            }
            if ((o->wce & 0x8000) && (o->wce & 0x7fff) != 2)
                D_8009C933 = 0;
        }
        o->b04++;
        break;
    case 3:
        FUN_8001888c(o);
        break;
    }
}
