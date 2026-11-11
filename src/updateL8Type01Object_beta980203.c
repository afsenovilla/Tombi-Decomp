// VERSION BETA_980203 beta980203 8002D490 704
// FLAGS.BETA_980203 -O2 -G0 -fno-strength-reduce
#if defined(VERSION_BETA_980203)
// PAL updateL8Type01Object without the b00 tests and the 0x7fff masks of wce
typedef struct S {
    unsigned char b00, b01; char p0[2]; unsigned char b04; char p1[5]; unsigned char b0a; char p2[0x6c - 0xb];
    unsigned short w6c, w6e, w70, w72; char p3[0xa4 - 0x74]; unsigned char ba4; char p4[0xbc - 0xa5];
    unsigned short wbc, wbe, wc0, wc2; char p5[0xce - 0xc4]; unsigned short wce, wd0, wd2;
} S;
extern char D_800B6698[];
extern char D_800B669A[];
extern unsigned char D_800B669C[], D_800B669D[], D_800B669E[], D_800B669F[];
extern unsigned char D_800A241F[], D_800A2422[];
extern unsigned char D_800A241E, D_800A2413, D_800ABBBE, D_800ABBCC;
extern unsigned char D_800ABB24, D_800ABB25, D_800ABB26, D_800ABB27;
extern unsigned char D_800A2882;
extern signed char D_800A2D90;
extern unsigned short acquireSpriteSlot(int);
extern void l8Type01MoveStep(S *);
extern void ObjListPush_1F800230(S *);
extern void setEventStarted(int, int, int);
extern void freeObjectLayer8(S *);
void updateL8Type01Object(S *o)
{
    int x, y, x2, y2, c0, c1, c2, c3;
    unsigned short m;
    switch (o->b04) {
    case 0:
        o->b0a = 0x14;
        o->ba4 = 1;
        o->wc2 = acquireSpriteSlot(o->wc0);
        {
            short sl = acquireSpriteSlot(o->wbc);
            int off;
            o->wbe = sl;
            off = sl * 10;
            c1 = D_800B669D[off];
            c0 = D_800B669C[off];
            c2 = D_800B669E[off];
            c3 = D_800B669F[off];
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
        if (D_800A2882 != 1)
            l8Type01MoveStep(o);
        o->b01 = 1;
        ObjListPush_1F800230(o);
        break;
    case 2:
        {
            int off2 = o->wbe * 10;
            if (--*(short *)(D_800B669A + off2) == 0)
                *(short *)(D_800B6698 + o->wbe * 10) = -1;
        }
        if (D_800A2D90 != 3) {
            if (o->wce) {
                if (o->wd2)
                    setEventStarted(o->wd2, 0, 3);
                m = o->wce;
                if (m == 2)
                    break;
                if (o->wd0 == 0 || m == 3) {
                    D_800A241F[0] = 0;
                    D_800A2422[0] = 0;
                    if (o->wce != 4 && (D_800ABBBE == 0 || D_800ABBCC < 2)) {
                        D_800ABB24 = 1;
                        D_800ABB25 = 0;
                        D_800ABB26 = 0;
                        D_800ABB27 = 0;
                    } else {
                        D_800A241E = 0;
                    }
                }
                D_800A2413 = 0;
            }
        }
        o->b04++;
        break;
    case 3:
        freeObjectLayer8(o);
        break;
    }
}
#endif
