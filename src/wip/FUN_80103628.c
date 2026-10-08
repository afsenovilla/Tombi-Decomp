// FUNC 80103628 332 X000
#include "TOBJ.H"
extern char *DAT_8009d2e8;
extern void FUN_800eeb5c(TObj *, int);

void FUN_80103628(TObj *o)
{
    char *p = DAT_8009d2e8, *q, *r, k;
    o->h->p.whole = *(short *)(*(char **)(p + 0x40) + 2);
    o->y.p.whole = *(unsigned short *)(p + 0x16) - *(unsigned short *)(p + 0x70);
    o->timer = 0;
    FUN_800eeb5c(o, 0xd);
    o->velH = 0;
    o->velV = 0;
    o->wb2 = 0;
    switch (DAT_8009d2e8[2]) {
    case 0:
    case 0x3a:
        *(char *)&o->wac = 3;
        DAT_8009d2e8[6] = 2;
        break;
    case 10:
    case 0x28:
        goto sw2;
    case 0x13:
        *(char *)&o->wac = 3;
        break;
    case 0x38:
        o->b9c = 0;
    case 0x42:
    default:
        k = 3;
        *(char *)&o->wac = k;
        DAT_8009d2e8[6] = k;
    }
    o->state = 2;
sw2:
    switch (DAT_8009d2e8[2]) {
    case 0:
    case 8:
    case 0x13:
    case 0x1e:
    case 0x1f:
    case 0x3a:
        *(int *)(DAT_8009d2e8 + 0x8c) = 0;
    }
    q = DAT_8009d2e8;
    o->d8c = (unsigned char)q[0x8c];
    *(unsigned short *)(q + 0x2e) = o->animFrame & 1;
}
