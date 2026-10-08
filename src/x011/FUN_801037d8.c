// FUNC 801037d8 1252 X011
// MATCHING 801037d8 1252
typedef struct { short s0, s2; } H;
typedef struct O {
    char p0[2]; unsigned char b2, b3; char p0b; unsigned char b5, b6; char p1[0x12 - 7]; short x12; char p2[2]; short y16; char p3[2]; short z1a;
    char p4a[4]; short s20; char p4b[0x2e - 0x22]; unsigned short w2e; char p4[0x40 - 0x30]; H *h;
    char p5[0x69 - 0x44]; unsigned char b69; char p5b[0x70 - 0x6a]; short s70, s72; char p6[0x7c - 0x74]; short vx, vy, s80, s82;
    char p7[0x8c - 0x84]; int a8c; char p7b[0x9c - 0x90]; unsigned char b9c; char p8[0xac - 0x9d]; unsigned char bac;
    char p9[0xb2 - 0xad]; unsigned short wb2;
} O;
extern O *DAT_8009d2e8;
extern unsigned short DAT_8009d670;
extern unsigned short D_1f8003c6, D_1f8001fc;
extern void FUN_8010eaf8(O *);
extern void FUN_8001fd94(O *);
extern void FUN_80103628(O *);
extern void FUN_800ee4e0(O *, int);
extern short FUN_8003fd78(O *, int, int);

#define PADCHK() { volatile unsigned short *pad = &DAT_8009d670; if (*pad & 0x80) o->w2e = 1; if (*pad & 0x20) o->w2e = 0; }

void FUN_801037d8(O *o)
{
    short flag;
    O *p;

    o->vy += 8;
    if (o->vy > 0x680) o->vy = 0x680;
    if (o->vy < -0x680) o->vy = -0x680;
    switch (o->b6) {
    case 1:
        o->s20++;
        FUN_8001fd94(o);
        FUN_8010eaf8(o);
        *(int *)o->h += o->vx << 8;
        flag = 0;
        o->y16 += 8;
        if (o->y16 + DAT_8009d2e8->s70 > DAT_8009d2e8->y16) {
            o->y16 = DAT_8009d2e8->y16 - DAT_8009d2e8->s70;
            flag = 1;
        }
        if (o->vx < 0) {
            o->h->s2 -= 8;
            if (o->h->s2 <= DAT_8009d2e8->h->s2) {
                o->h->s2 = DAT_8009d2e8->h->s2;
                if (flag) FUN_80103628(o);
            }
        } else {
            o->h->s2 += 8;
            if (o->h->s2 >= DAT_8009d2e8->h->s2) {
                o->h->s2 = DAT_8009d2e8->h->s2;
                if (flag) FUN_80103628(o);
            }
        }
        if (o->b69 != 0 || FUN_8003fd78(o, (short)(DAT_8009d2e8->y16 - o->y16 - o->s70 + (DAT_8009d2e8->s72 - DAT_8009d2e8->s70)), 1) || o->s20 > 10) {
            FUN_80103628(o);
        }
        return;
    case 2:
        switch (DAT_8009d2e8->b2) {
        case 0x38:
            PADCHK();
            {
                O *q = DAT_8009d2e8;
                o->h->s2 = q->h->s2;
                o->y16 = q->y16 - q->s70;
            }
            break;
        case 10:
            o->h->s2 = DAT_8009d2e8->h->s2;
            o->y16 = DAT_8009d2e8->y16 - DAT_8009d2e8->s70;
            o->b9c = 0;
            PADCHK();
            break;
        default:
            PADCHK();
            p = DAT_8009d2e8;
            p->w2e = o->w2e & 1;
            p->h->s2 = o->h->s2;
            p->y16 = o->y16 + p->s70;
            FUN_8001fd94(o);
            if (o->b69 != 0 || FUN_8003fd78(o, (short)(DAT_8009d2e8->y16 - o->y16 - o->s70 + (DAT_8009d2e8->s72 - DAT_8009d2e8->s70)), 1)) {
                o->b69 = 0;
                o->bac = 3;
                o->b9c = 0;
                o->vx = 0;
                o->vy = 0;
                o->s80 = 0;
                o->s82 = 0;
                o->wb2 = 0;
                FUN_800ee4e0(o, 0);
                DAT_8009d2e8->a8c = ((unsigned char *)o)[0x8c];
            }
            break;
        }
        if (D_1f8001fc & D_1f8003c6) {
            o->b9c = 1;
            o->b69 = 0;
            o->wb2 = 0;
            o->vx = 0;
            o->vy = 0;
            switch (DAT_8009d2e8->b2) {
            case 0: case 0x13: case 0x1f: case 0x28:
            case 0x3a: case 0x3b: case 0x3c: case 0x3d: case 0x3e: case 0x3f: case 0x40: case 0x41: case 0x42:
                DAT_8009d2e8->b6 = 7;
                break;
            case 10:
                DAT_8009d2e8->b6 = DAT_8009d2e8->b6 + 1;
                break;
            case 0x38:
                if (DAT_8009d2e8->b3 != 0) {
                    DAT_8009d2e8->b6 = 6;
                    break;
                }
            default:
                DAT_8009d2e8->b6 = 4;
                break;
            }
            o->b5 = 0xf;
            o->b6 = 0;
        }
        break;
    }
}
