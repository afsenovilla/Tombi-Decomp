// FUNC 80103628 332 X010
// MATCHING 80103628 332
#include "TOBJ.H"
extern char *BITTEN_OBJECT;
extern void FUN_800eeb5c(TObj *, int);

void playerBiteHoldAttach(TObj *o)
{
    char *p = BITTEN_OBJECT, *q, *r, k;
    o->h->p.whole = *(short *)(*(char **)(p + 0x40) + 2);
    o->y.p.whole = *(unsigned short *)(p + 0x16) - *(unsigned short *)(p + 0x70);
    o->timer = 0;
    FUN_800eeb5c(o, 0xd);
    o->velH = 0;
    o->velV = 0;
    o->wb2 = 0;
    switch (BITTEN_OBJECT[2]) {
    case 0:
    case 0x3a:
        *(char *)&o->wac = 3;
        r = BITTEN_OBJECT;
        r[6] = 2;
        o->state = 2;
        goto sw2;
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
        *(char *)&o->wac = 3;
        r = BITTEN_OBJECT;
        r[6] = 3;
    }
    o->state = 2;
sw2:
    switch (BITTEN_OBJECT[2]) {
    case 0:
    case 8:
    case 0x13:
    case 0x1e:
    case 0x1f:
    case 0x3a:
        *(int *)(BITTEN_OBJECT + 0x8c) = 0;
    }
    q = BITTEN_OBJECT;
    o->d8c = (unsigned char)q[0x8c];
    *(unsigned short *)(q + 0x2e) = o->animFrame & 1;
}
