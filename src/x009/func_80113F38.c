// FUNC 80113f38 904 X009
// MATCHING 80113f38 904
typedef struct P { short x; short y; } P;
typedef struct V { short x, xh, y, yh, z, zh; } V;
typedef struct S {
    unsigned char b00, b01, b02, b03, b04, step, state, b07; char p0[0x10 - 8];
    V pos; char p1[0x20 - 0x1c]; short timer; char p2[0x2e - 0x22]; unsigned short frame; char p3[0x40 - 0x30];
    P *h; char p4[0x68 - 0x44]; unsigned char b68, b69, b6a; char p5[0x6c - 0x6b];
    short w6c, w6e, w70, w72; char p6[0x7c - 0x74]; short velX, velY, velH, velV; char p7[0x90 - 0x84];
    struct S *p90;
} S;
extern unsigned char D_8009C93A;
extern unsigned short D_800A604E;
extern P *D_800A6078;
extern int D_800A6094;
extern unsigned char D_800A603C, D_800A603D, D_800A603E, D_8009C942, D_8009C93F;
extern short D_800A60EA;
extern int AnimAdvance(S *);
extern void TileCollideAt(S *, int, int);
extern void FUN_80113888(S *), FUN_80113b6c(S *), FUN_80113cc8(S *);
extern S *FUN_8002dcf0(int, int, V *, int);
void func_80113F38(S *o)
{
    V v;
    int f;
    if ((o->b00 & 8) && D_8009C93A != 0 && (unsigned short)(o->pos.yh - D_800A604E + 0x80) < 0x100) {
        if (o->frame & 1) {
            if (o->h->y - o->w6c < D_800A6078->y)
                D_800A6078->y = o->h->y - o->w6c;
        } else {
            if (D_800A6078->y < o->h->y + (o->w6e - o->w6c))
                D_800A6078->y = o->h->y + (o->w6e - o->w6c);
        }
    }
    switch (o->step) {
    case 0:
        AnimAdvance(o);
        break;
    case 1:
        switch (o->state) {
        case 0:
            o->velX = 0;
            o->velY = 0;
            o->velH = 0;
            o->velV = 0;
            o->state++;
        case 1:
            AnimAdvance(o);
            *(int *)&o->pos.y += 0x40000;
            TileCollideAt(o, o->h->y, (short)(o->pos.yh + o->w72 - o->w70));
            if (--o->timer == 0) {
                o->b6a = 0;
                goto reset;
            }
            break;
        }
        break;
    case 2:
        FUN_80113888(o);
        break;
    case 3:
        if (o->b01 == 0)
            break;
        switch (o->state) {
        case 0:
            v = o->pos;
            v.yh -= 0x40;
            o->p90 = FUN_8002dcf0(0xd, 2, &v, o->timer);
            o->state++;
            break;
        case 1:
            AnimAdvance(o);
            if (--o->timer == 0) {
                o->b6a = 0;
                goto reset;
            }
            break;
        }
        break;
    case 4:
        switch (o->state) {
        case 0:
            o->b68 = 0;
            o->state++;
        case 1:
            AnimAdvance(o);
            if (o->p90->b04 == 2) {
                o->p90->b04 = 3;
                f = D_800A6094;
                D_800A603C = 1;
                *(short *)0x1F8001C6 = 0;
                D_8009C942 = 0;
                D_800A60EA = 0;
                D_800A603D = 0;
                D_800A603E = 0;
                if (f == 1) {
                    D_8009C93F = 0;
                    D_800A6094 = 0;
                }
                o->b68 = 0;
                o->b6a = 0;
            reset:
                o->step = 0;
                o->state = 0;
            }
            break;
        }
        break;
    case 5:
        FUN_80113b6c(o);
        break;
    case 7:
        FUN_80113cc8(o);
        break;
    }
}
