// FUNC 800fbdb4 632 X003
// MATCHING 800fbdb4 632
typedef struct S {
    char p0[5]; unsigned char step, state; char p1[0x20 - 7];
    short timer; char p2[0x2c - 0x22]; unsigned short w2c, frame; char p3[0x69 - 0x30];
    unsigned char b69; char p4[0x7c - 0x6a]; short velX, velY; char p5[0x84 - 0x80];
    int d84; int d88; int d8c; char p6[0x9c - 0x90]; unsigned char b9c, p9d, b9e; char p7[0xac - 0x9f];
    unsigned char bac; char p8[0xb0 - 0xad]; short wb0, wb2, wb4, wb6;
} S;
typedef struct V { short x, y; } V;
extern short D_80115248s[];
extern unsigned short D_8009D670;
extern S *D_8009C330;
extern void ObjSetAnimFromTable(S *);
extern void AnimJump(S *, int);
extern void SfxPlay2(int, int);
void func_800FBDB4(S *o)
{
    int a;
    char pad8;
    short v, k, i, vx;
    S *g;
    if (o->state != 0)
        return;
    a = o->d84;
    o->d8c = 0;
    o->b9e = 0;
    o->b69 = 0;
    o->b9c = 1;
    o->wb0 = 0;
    o->bac = 0;
    if ((unsigned short)(a - 0x40) < 0x80) {
        v = a - 0x40;
        if (v < 0x20) k = 4;
        else if (v < 0x40) k = 5;
        else if (v < 0x60) k = 6;
        else if (v < 0x80) k = 7;
    } else {
        v = a;
        if (v < 0x20) k = 0;
        else if (v < 0x40) k = 1;
        else if (v < 0xe0) k = 2;
        else k = 3;
    }
    i = k * 2;
    vx = D_80115248s[i];
    if (o->frame & 1)
        vx = -vx;
    o->velX = vx;
    o->velY = D_80115248s[i + 1];
    if (o->wb2 == 0) o->velX = 0;
    if (o->wb4 < 0x29) o->velX = 0;
    if (o->frame & 1) {
        o->velX = -0x300;
        o->velY = -0x600;
        if (*(volatile unsigned short *)&D_8009D670 & 0x80) { o->velX = -0x480; o->velY = -0x800; }
        if (*(volatile unsigned short *)&D_8009D670 & 0x20) { o->velX = -0x200; o->velY = -0x400; }
    } else {
        o->velX = 0x300;
        o->velY = -0x600;
        if (*(volatile unsigned short *)&D_8009D670 & 0x80) { o->velX = 0x200; o->velY = -0x400; }
        if (*(volatile unsigned short *)&D_8009D670 & 0x20) { o->velX = 0x480; o->velY = -0x800; }
    }
    g = D_8009C330;
    g->timer = 10;
    o->wb2 = 0;
    o->wb6 = 0;
    g->w2c = 8;
    if (g->frame != 8) {
        g->w2c = 8;
        ObjSetAnimFromTable(o);
        AnimJump(o, 0);
        D_8009C330->frame = D_8009C330->w2c;
    }
    SfxPlay2(2, 4);
    o->step = 2;
    o->state = 2;
}
