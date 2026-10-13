// FUNC 8004d3f4 556 MAIN0
// MATCHING 8004d3f4 556
typedef struct S {
    char p0; unsigned char b01; char p1[2]; unsigned char b04; char p2[0x20 - 5];
    short timer; char p3[0xb4 - 0x22];
    unsigned short wb4, wb6; char p4[0xbe - 0xb8];
    unsigned short wbe, wc0; char p5[0xca - 0xc2];
    unsigned short wca;
} S;
typedef struct G { short n, f2, f4, f6; } G;
typedef struct E { short v, pad[3]; } E;
typedef struct M { short a; unsigned short mask; short pad[3]; } M;
extern G MESSAGE_SLOTS[];
extern E MESSAGE_GLYPHS[][128];
extern M GLYPH_CACHE[];
typedef struct M2 { unsigned short mask; short pad[4]; } M2;
extern M2 D_800A5DE2[];
extern M2 D_800A5DE2_st[];
extern void ObjListPush_1F800220(S *);
extern void ObjFree(S *);
void updateInfoMessage(S *o)
{
    int i;
    switch (o->b04) {
    case 0:
        o->b04++;
        if (o->wb6 == 2 && o->wb4 == 0)
            o->timer = 0x28;
        else
            o->timer = 0x78;
        break;
    case 1:
        MESSAGE_SLOTS[o->wca].f4 = o->wbe;
        MESSAGE_SLOTS[o->wca].f6 = o->wc0;
        o->b01 = 1;
        ObjListPush_1F800220(o);
        if (--o->timer == 0 || MESSAGE_SLOTS[o->wca].f2 == -1) {
            MESSAGE_SLOTS[o->wca].f2 = -1;
            o->b04++;
        }
        break;
    case 2:
        for (i = 0; i < MESSAGE_SLOTS[o->wca].n; i++) {
            int v = MESSAGE_GLYPHS[o->wca][i].v * 10;
            if ((*(unsigned short *)((char *)D_800A5DE2 + v) &= ~(1 << o->wca)) == 0)
                *(short *)((char *)GLYPH_CACHE + v) = -1;
        }
        MESSAGE_SLOTS[o->wca].n = -1;
        o->b04++;
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
