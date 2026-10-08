// FUNC 8012a6a4 380 X000
// MATCHING 8012a6a4 380
#include "TOBJ.H"
extern void FUN_8001fa88(TObj *, unsigned short);
extern char DAT_80077d0c[];
extern unsigned char DAT_80138fd8[];
extern unsigned short *PTR_DAT_8013a1f8[];

void FUN_8012a6a4(TObj *o)
{
    unsigned char *p;
    int x;
    unsigned short *a;

    switch (o->state) {
    case 0:
        o->b0b = 1;
        o->b0f = 4;
        o->active = 2;
        o->velV = -0x400;
        o->movetab = DAT_80077d0c;
        o->wac = 0x26;
        o->category |= 0x80;
        o->state++;
        a = PTR_DAT_8013a1f8[0];
        o->anim = a;
        p = &DAT_80138fd8[a[1] * 4];
        o->box0 = *p++;
        o->box1 = *p++;
        o->box2 = *p++;
        o->box3 = *p++;
        o->animTimer = ((unsigned short *)o->anim)[3] & 0x3fff;
        break;
    case 1:
        FUN_8001fa88(o, 1 - o->animFrame);
        o->velV += 0x40;
        if (o->velV > 0x400) o->velV = 0x400;
        o->y.raw += o->velV << 8;
        break;
    }
    if (o->animFrame & 1) x = o->d8c + 20;
    else x = o->d8c - 20;
    o->d8c = x & 0xff;
}
