// FUNC 80106b50 608 X008
// MATCHING 80106b50 608
#include "TOBJ.H"
#define B(o, n) (*((unsigned char *)(o) + (n)))
extern unsigned char *DAT_8009c330;
extern unsigned char DAT_801152e8[];
extern void FUN_800eeb5c(TObj *, int);
extern void FUN_8001e4f0(int);
extern void FUN_800eea3c(TObj *);
extern void FUN_800ee428(TObj *);
extern void FUN_800ee88c(TObj *);
extern void FUN_8011116c(TObj *);
extern void FUN_800ee9cc(TObj *);
extern void FUN_8001fec0(TObj *);

void FUN_80106b50(TObj *o)
{
    int t;
    unsigned char u;
    switch (o->state) {
    case 0:
        o->wb2 = 0;
        FUN_800eeb5c(o, 0x28);
        FUN_8001e4f0(0x20);
        if (B(o, 0xab) & 0x80) {
            o->active = 3;
            o->timer = 0x3c;
            o->state++;
        } else {
            o->timer = 0x28;
            o->state = 2;
        }
        break;
    case 1:
        FUN_800eea3c(o);
        if (o->velY > 0x400) {
            B(o, 0xac) = 1;
            DAT_8009c330[0x1e] = 0;
            DAT_8009c330[0x1f] = 0;
            FUN_800ee428(o);
            o->step = 2;
            o->state = 3;
        }
        FUN_800ee88c(o);
        FUN_8011116c(o);
        FUN_800ee9cc(o);
        FUN_8001fec0(o);
        if (--o->timer <= 0) {
            o->active = 1;
            o->wb2 = 0;
            B(o, 0xac) = 0;
            B(o, 0xab) = 0;
            u = DAT_801152e8[o->wb0];
            o->b04 = 1;
            o->step = 0;
            o->state = 0;
            o->d8c = u;
            break;
        }
        break;
        {
            o->step = 0;
            o->state = 0;
            o->d8c = t;
        }
        break;
    case 2:
        B(o, 0xab) = 1;
        o->animFrame = (o->animFrame & 1) | 2;
        o->state++;
    case 3:
        FUN_800eea3c(o);
        if (o->velY > 0x400) {
            B(o, 0xac) = 1;
            DAT_8009c330[0x1e] = 0;
            DAT_8009c330[0x1f] = 0;
            FUN_800ee428(o);
            o->step = 2;
            o->state = 3;
        }
        FUN_800ee88c(o);
        FUN_8011116c(o);
        FUN_800ee9cc(o);
        FUN_8001fec0(o);
        if (--o->timer <= 0) {
            t = DAT_801152e8[o->wb0];
            B(o, 0xab) = 0;
            o->b04 = 1;
            o->step = 0;
            o->state = 0;
            o->d8c = t;
        }
        break;
    }
}
