// FUNC 8011e1f0 864 X014
// MATCHING 8011e1f0 864
#include "TOBJ.H"

typedef struct {
    unsigned short a;   /* 0xb4 */
    unsigned short b;   /* 0xb6 */
    unsigned short c;   /* 0xb8 */
    unsigned short d;   /* 0xba */
    unsigned short e;   /* 0xbc */
    unsigned short f;   /* 0xbe */
    unsigned short g;   /* 0xc0 */
    unsigned short h;   /* 0xc2 */
    unsigned short i;   /* 0xc4 */
    unsigned short j;   /* 0xc6 */
} Sub;

#define WC6(o) (*(unsigned short *)((char *)(o) + 0xc6))

extern signed char D_8009D2B0;
extern unsigned short D_8009C962[];
extern unsigned short D_1F80016A, D_1F80016E;
extern unsigned char D_80125CA8[];
extern unsigned char D_80125CB8[];
extern unsigned char D_80125CC8[];
extern unsigned char D_80125D80[];
extern unsigned short D_80125E10[];
extern void (*D_80125CD8[])(TObj *);
extern void ObjSetFacingToPlayer(TObj *);
extern int Rand(void);
extern int AnimAdvance(TObj *);
extern int FUN_800205d8(int, int);

static __inline__ int chk(TObj *o)
{
    if (D_8009C962[0] != 7) return 0;
    WC6(o) = 0;
    if ((unsigned short)(o->h->p.whole - D_1F80016A + 0x40) > 0x80) return 0;
    if ((unsigned short)(o->y.p.whole - D_1F80016E + 0x40) > 0x80) return 0;
    if (D_80125D80[Rand() & 0xf]) WC6(o) = 1;
    else WC6(o) = 2;
    return 1;
}

void func_8011E1F0(TObj *o)
{
    Sub *s = (Sub *)&o->wb4;
    unsigned char *t;
    int x;

    switch (o->state) {
    case 0:
        o->d8c = 0;
        ObjSetFacingToPlayer(o);
        if (WC6(o) == 0) {
            if ((unsigned short)o->wb4 < 0x708) {
                t = D_80125CC8;
            } else {
                t = D_80125CA8;
                if ((unsigned short)o->wb4 < 0xe10) {
                    t = D_80125CB8;
                }
            }
            if (t[Rand() & 0xf] == 0) {
                o->state = 0;
                o->step++;
                break;
            }
        }
        o->d38 = FUN_800205d8((short)(D_1F80016A - o->h->p.whole), (short)(D_1F80016E - o->y.p.whole)) & 0xff;
        o->wb6 = D_8009C962[0];
        x = ((o->d38 + 8) & 0xff) >> 4;
        o->wb8 = x;
        o->animFrame = ~((x - 4) >> 3) & 1;
        o->state++;
        if (s->j < 2) {
            if (s->i != 0) {
                if (s->i - 1 < s->g) s->g = 0;
                s->b = D_80125E10[s->g];
                s->g++;
            } else if (D_80125CA8[Rand() & 0xf]) {
                s->b = 7;
            } else {
                o->state = 2;
                o->timer = 0x28;
                break;
            }
        } else {
            s->b = 7;
            s->j = 0;
        }
    case 1:
        if (D_8009D2B0 == 3) break;
        D_80125CD8[s->b](o);
        if (s->b == 7) break;
        if (o->wac != 0x12) break;
        if (chk(o)) {
            o->step = 1;
            o->state = 0;
            o->substep = 0;
        }
        break;
    case 2:
        AnimAdvance(o);
        if (--o->timer == -1) {
            o->step = 0;
            o->state = 0;
            o->substep = 0;
        }
        break;
    }
}
