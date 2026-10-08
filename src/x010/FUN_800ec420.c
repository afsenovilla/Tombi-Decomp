// FUNC 800ec420 468 X010
// MATCHING 800ec420 468
#include "TOBJ.H"
typedef struct { short k; short pad; void **anims; int pad2; } T12;
extern T12 DAT_80114c20[];
extern int DAT_1f8002c8[];
extern unsigned short DAT_800a6066;
typedef struct G { char p0; unsigned char b1; char p1[0xf - 2]; unsigned char b0f; } G;
extern G DAT_800a6038;
extern void FUN_8001fe6c(TObj *);
extern int FUN_800202b4(TObj *);
extern void FUN_800ebd40(TObj *);
extern void FUN_800ebf68(TObj *);
extern int FUN_8001fec0(TObj *);
extern void FUN_800187e4(TObj *);

void FUN_800ec420(TObj *o)
{
    unsigned char s = o->b04;
    unsigned short v;
    switch (s) {
    case 0:
        if (o->subtype == 9)
            goto inc;
        o->w1e = 0x13;
        o->d3c = DAT_1f8002c8[DAT_80114c20[o->subtype].k];
        o->anim = DAT_80114c20[o->subtype].anims[o->b0c];
        FUN_8001fe6c(o);
        v = DAT_800a6066;
        o->b0a = 2;
        o->b0d = 0x80;
        *(signed char *)&o->b0f = -7;
        o->d8c = 0;
        o->b6b = 0;
        o->animFrame = v & 1;
        o->category |= 0x80;
        o->b04++;
        break;
    case 1:
        FUN_800202b4(o);
        switch (o->subtype) {
        case 0:
        case 1:
        case 2:
            FUN_800ebd40(o);
            break;
        case 3:
        case 4:
            FUN_800ebf68(o);
            break;
        case 9:
            o->visible = DAT_800a6038.b1;
            o->b0f = DAT_800a6038.b0f - 1;
            if (FUN_8001fec0(o) != 0)
                o->b04 = 3;
            break;
        }
        break;
    case 2:
    inc:
        o->b04 = s + 1;
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
