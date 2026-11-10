// VERSION SLPM_80165 slpm80165 8002B4BC 552
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPM_80165)
// Demo version of func_8002C070_slps01144 (src/func_8002C070_slps01144.c): on removal no event start and no
// mode / count checks (the player state is reset whenever D_800A7C16 is clear).
typedef struct S {
    unsigned char b00, b01; char p0[2]; unsigned char b04; char p1[5]; unsigned char b0a; char p2[0x6c - 0xb];
    unsigned short w6c, w6e, w70, w72; char p3[0xa4 - 0x74]; unsigned char ba4; char p4[0xbc - 0xa5];
    unsigned short wbc, wbe, wc0, wc2; char p5[0xce - 0xc4]; unsigned short wce, wd0, wd2;
} S;
extern char D_800B26E8[];
extern unsigned char D_8009E4E7, D_8009E4EA, D_8009E4E6, D_800A7C16, D_800A7C24, D_800A7B7C, D_800A7B7D, PLAYER_STEP, D_800A7B7F;
extern char D_800B26EA[];
extern unsigned char D_8009E94A;
extern unsigned char D_800B26EC[], D_800B26ED[], D_800B26EE[], D_800B26EF[];
extern signed char GAME_PLAYER_MODE;
extern unsigned char D_8009E4DB;
extern unsigned short func_8001CE50(int);
extern void func_8002C300(S *);
extern void ObjListPush_1F800230(S *);
extern void setEventStarted(int, int, int);
extern void freeObjectLayer8(S *);
void func_8002C070_slps01144(S *o)
{
    int x, y, x2, y2, c0, c1, c2, c3;
    unsigned short m;
    switch (o->b04) {
    case 0:
        o->b0a = 0x14;
        o->ba4 = 1;
        o->wc2 = func_8001CE50(o->wc0);
        {
            short sl = func_8001CE50(o->wbc);
            int off;
            o->wbe = sl;
            off = sl * 10;
            c1 = D_800B26ED[off];
            c0 = D_800B26EC[off];
            c2 = D_800B26EE[off];
            c3 = D_800B26EF[off];
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
        if (D_8009E94A != 1)
            func_8002C300(o);
        o->b01 = 1;
        ObjListPush_1F800230(o);
        break;
    case 2:
        {
            int off2 = o->wbe * 10;
            if (--*(short *)(D_800B26EA + off2) == 0)
                *(short *)(D_800B26E8 + o->wbe * 10) = -1;
        }
        if (o->wce) {
            D_8009E4E7 = 0;
            D_8009E4EA = 0;
            if (D_800A7C16 == 0) {
                D_800A7B7C = 1;
                D_800A7B7D = 0;
                PLAYER_STEP = 0;
                D_800A7B7F = 0;
            } else {
                D_8009E4E6 = 0;
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
