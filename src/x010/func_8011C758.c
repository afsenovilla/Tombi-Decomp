// FUNC 8011c758 680 X010
// MATCHING 8011c758 680
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
extern void FUN_80103cbc(O *);
extern void PlayerSetAnimIfChanged(O *, int);
extern short FUN_8003fd78(O *, int, int);

#define PADCHK() { volatile unsigned short *pad = &DAT_8009d670; if (*pad & 0x80) o->w2e = 1; if (*pad & 0x20) o->w2e = 0; }

void func_8011C758(O *o)
{
    short flag;
    O *p;

    switch (o->b6) {
    case 1:
        o->s20++;
        FUN_8010eaf8(o);
        *(int *)o->h += o->vx << 8;
        o->vy += 8;
        if (o->vy > 0x680) o->vy = 0x680;
        if (o->vy < -0x680) o->vy = -0x680;
        FUN_8001fd94(o);
        flag = 0;
        o->y16 += 8;
        if (o->y16 + DAT_8009d2e8->s70 >= DAT_8009d2e8->y16) {
            o->y16 = DAT_8009d2e8->y16 - DAT_8009d2e8->s70;
            flag = 1;
        }
        if (o->vx < 0) {
            o->h->s2 -= 8;
            if (o->h->s2 <= DAT_8009d2e8->h->s2) {
                o->h->s2 = DAT_8009d2e8->h->s2;
                if (flag) PlayerSetAnimIfChanged(o, 0xd);
            }
        } else {
            o->h->s2 += 8;
            if (o->h->s2 >= DAT_8009d2e8->h->s2) {
                o->h->s2 = DAT_8009d2e8->h->s2;
                if (flag) PlayerSetAnimIfChanged(o, 0xd);
            }
        }
        if (o->b69 != 0 || FUN_8003fd78(o, (short)(DAT_8009d2e8->y16 - o->y16 - o->s70), 1) || o->s20 > 10) {
            FUN_80103cbc(o);
        }
        return;
    case 2:
        PADCHK();
        p = DAT_8009d2e8;
        o->h->s2 = p->h->s2;
        o->y16 = p->y16 - p->s70;
        if (D_1f8001fc & D_1f8003c6) {
            o->b9c = 0;
            o->b69 = 0;
            o->bac = 0;
            o->wb2 = 0;
            DAT_8009d2e8->b5 = 1;
            o->b5 = 0x3e;
            o->b6 = 0;
        }
        break;
    }
}
