// FUNC 80028be0 948 MAIN0
// MATCHING 80028be0 948
#include "TOBJ.H"
typedef struct { char p[0x24]; int d24; char q[0x70 - 0x28]; signed char b70, b71, b72, b73; } S;
extern unsigned char D_8009C93F;
extern TObj D_800A6038;
static __inline__ int approach(S *o)
{
    switch (o->b71) {
    case 0:
        if ((o->b73 << 8) < o->d24) {
            o->d24 -= 0x80;
            return 0;
        }
        return 1;
    case 1:
        if (o->d24 >= (o->b73 << 8)) return 1;
        o->d24 += 0x80;
        return 0;
    }
    return 0;
}
static __inline__ int decel(S *o)
{
    int v = o->d24;
    if (v != 0) {
        if (v > 0) v -= 0x80;
        else v += 0x80;
        o->d24 = v;
        return 0;
    }
    o->b71 = 0;
    o->b72 = 0;
    o->b73 = 0;
    return 1;
}

void func_80028BE0(S *o)
{
    TObj *p;
    if (D_8009C93F) {
        if (o->d24 != 0) {
            if (o->d24 > 0) o->d24 -= 0x80;
            else o->d24 += 0x80;
        } else {
            o->b71 = 0;
            o->b72 = 0;
            o->b73 = 0;
        }
        return;
    }
    p = &D_800A6038;
    switch (o->b70) {
    case 0:
        if (p->ba4) {
            o->b70 = 5;
            if (p->animFrame & 1) {
                o->b71 = 0;
                o->b73 = -10;
            } else {
                o->b71 = 1;
                o->b73 = 10;
            }
            break;
        }
        switch (p->b9e) {
        case 2: case 3: case 5: case 6: case 8: case 9:
            o->b70 = 1;
            if (p->animFrame & 1) {
                o->b71 = 1;
                o->b73 = 10;
            } else {
                o->b71 = 0;
                o->b73 = -10;
            }
            break;
        }
        break;
    case 1:
        if ((p->animFrame & 1) != o->b71) {
            o->b71 = p->animFrame & 1;
            o->b73 = -o->b73;
        }
        if (approach(o)) o->b70 = 2;
    case 2:
        switch (p->b9e) {
        case 0: case 1: case 4: case 7: case 10: case 11: case 12:
            o->b70 = 3;
            o->b71 = 0;
            break;
        case 2: case 3: case 5: case 6: case 8: case 9:
            if ((p->animFrame & 1) != o->b71) {
                o->b70 = 1;
                o->b71 = p->animFrame & 1;
                o->b73 = -o->b73;
            }
            break;
        }
        break;
    case 3:
        if (decel(o)) o->b70 = 0;
        break;
    case 4:
        if (approach(o)) o->b70 = 0;
        break;
    case 5:
        if (approach(o)) o->b70++;
    case 6:
        if (!p->ba4) o->b70++;
        break;
    case 7:
        if (decel(o)) o->b70 = 0;
        break;
    }
}
