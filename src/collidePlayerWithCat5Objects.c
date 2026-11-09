// FUNC 80047a3c 180 MAIN0
// MATCHING 80047a3c 180
// VERSION SLPS_01144 slps01144 80052BA8 328
#if defined(VERSION_SLPS_01144)
#include "gvars.h"
typedef struct { char p0[2]; unsigned short s2; } H;
typedef struct {
    char t0, t1, t2, t3, t4, t5, t6, t7; char p0[0x16 - 8]; unsigned short s16;
    char p1[0x40 - 0x18]; H *h40; H *h44;
    char p2[0x6a - 0x48]; char b6a; char b6b; unsigned short s6c; short s6e; unsigned short s70; short s72;
    char p3[0xb4 - 0x74]; unsigned short sb4;
} TO;
extern short LIST_COUNTER;
extern unsigned short CAT5_OBJECT_COUNT;
extern TO **CAT5_OBJECT_LIST;
static __inline__ short hit(TO *a, TO *b)
{
    if ((unsigned short)(a->h44->s2 - b->h44->s2 + 0x2d) >= 0x5b)
        return 0;
    if ((unsigned short)(a->h40->s2 - b->h40->s2 + (a->s6c + b->s6c)) > (a->s6e + b->s6e))
        return 0;
    if ((unsigned short)(a->s16 - b->s16 + (a->s70 + b->s70)) > (a->s72 + b->s72))
        return 0;
    return 1;
}
static __inline__ void one(TO *a, TO *b)
{
    if (hit(a, b)) {
        a->b6a = 1;
        b->t0 = 4;
        b->t4 = 2;
        b->t5 = 0;
        b->t6 = 0;
    }
}
void func_80052BA8(TO *o)
{
    TO **pp;
    TO *p;
    if (GVARS.hurtState != 0) return;
    pp = CAT5_OBJECT_LIST;
    LIST_COUNTER = CAT5_OBJECT_COUNT;
    while (LIST_COUNTER != 0) {
        p = *pp++;
        LIST_COUNTER -= 1;
        if (p->t0 & 3) {
            one(o, p);
        }
    }
}
#else
#include "gvars.h"
#include "TOBJ.H"
extern signed char GAME_PLAYER_MODE;
extern short LIST_COUNTER;
extern unsigned short CAT5_OBJECT_COUNT;
extern unsigned char **CAT5_OBJECT_LIST;
extern void collidePlayerWithCat5Object(TObj *o, unsigned char *p);
void collidePlayerWithCat5Objects(TObj *o)
{
    unsigned char **pp;
    unsigned char *p;
    if (GVARS.hurtState != 0) return;
    if (GAME_PLAYER_MODE == 3) return;
    pp = CAT5_OBJECT_LIST;
    LIST_COUNTER = CAT5_OBJECT_COUNT;
    while (LIST_COUNTER != 0) {
        p = *pp++;
        LIST_COUNTER -= 1;
        if (*p & 3) {
            collidePlayerWithCat5Object(o, p);
        }
    }
}
#endif
