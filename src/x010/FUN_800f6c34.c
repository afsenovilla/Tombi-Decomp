// FUNC 800f6c34 332 X010
// MATCHING 800f6c34 332
#include "TOBJ.H"
extern unsigned short DAT_8009d670[];
extern unsigned short DAT_1f8003c6;
extern unsigned char DAT_8009d2b0;
extern unsigned char *DAT_8009c330;
extern unsigned char *DAT_8009f0ec;

void FUN_800f6c34(TObj *o)
{
    int a;
    short s;
    int s1;
    unsigned short u;
    Fix16 *h;
    if (DAT_8009d670[0] & DAT_1f8003c6) {
        DAT_8009d2b0 = 0;
        DAT_8009c330[9] = 0;
        o->step = 0xb;
        o->state = 0;
        DAT_8009f0ec[0x69] = 0;
    }
    if (DAT_8009d670[0] & 0x10) {
        DAT_8009c330[9] = 0;
        o->step = 0xb;
        o->state = 0;
        DAT_8009f0ec[0x69] = 0;
    }
    if (DAT_8009d670[0] & 0x40) {
        DAT_8009c330[9] = 0;
        *(unsigned char *)&o->wac = 1;
        u = o->animFrame;
        h = o->h;
        o->b9e = 0;
        o->wb2 = 0;
        o->velH = 0;
        o->velV = 0;
        o->d30 = 0;
        o->d34 = 0;
        o->velX = 0;
        o->velY = 0;
        s1 = h->p.whole;
        if (u & 1)
            s = s1 + 0xe;
        else
            s = s1 - 0xe;
        h->p.whole = s;
        a = 0x10;
        o->timer = 10;
        o->d84 = 0;
        if (o->animFrame & 1)
            a = 0xf0;
        o->step = 2;
        o->state = 3;
        o->d88 = a;
        o->d8c = 0;
        DAT_8009f0ec[0x69] = 0;
    }
}
