// FUNC 8010df84 400 X000
typedef struct { char p0[5]; unsigned char step, state; char p1[0x24 - 7]; int anim; char p2[4]; unsigned int fl; } H;
typedef struct { char p0[0x2e]; unsigned short af; } P;
typedef struct { char p0[8]; unsigned char b8; } Q;
typedef struct {
    char p0[5]; unsigned char step, state; char p1[0x24 - 7]; int anim; char p2[4]; union { unsigned int fl; struct { unsigned short t, af; } s; } u;
    char p3[0x8c - 0x30]; int d8c; char p4[0xa0 - 0x90]; unsigned char da0, pa1, da2, da3; char p5[0xac - 0xa4]; unsigned char wac;
} O;
extern P *DAT_8009c330;
extern Q *DAT_8009c330q;
extern unsigned short G1f8;
extern void FUN_8001fec0(O *), FUN_800eea3c(O *), FUN_800efa80(O *), FUN_800ee9cc(O *), FUN_800ee88c(O *), FUN_8001e4f0(int);

void FUN_8010df84(O *o)
{
    o->wac = 0;
    if (o->u.s.af < 2) {
        if (o->state == 0) {
            o->d8c = 0;
            ((Q *)DAT_8009c330)->b8 = 0;
            o->state = o->state + 1;
        } else if (o->state != 1) {
            return;
        }
        FUN_8001fec0(o);
        FUN_800eea3c(o);
        FUN_800efa80(o);
        FUN_800ee9cc(o);
        FUN_800ee88c(o);
        if ((G1f8 & 7) == 0) {
            if ((G1f8 & 0xf) == 0)
                FUN_8001e4f0(0);
            else
                FUN_8001e4f0(1);
        }
    } else {
        if ((o->u.fl & 0xa0000) == 0xa0000 && (o->da0 & 2)) {
            DAT_8009c330->af = 0xffff;
            o->anim = 0x80010de0;
            o->da2 = 1;
        } else if ((o->u.fl & 0xc0000) == 0xc0000 && (o->da0 & 1)) {
            DAT_8009c330->af = 0xffff;
            o->anim = 0x80010e2c;
            o->da3 = 1;
        } else {
            FUN_800efa80(o);
        }
        o->step = 0;
        o->state = 0;
    }
}
