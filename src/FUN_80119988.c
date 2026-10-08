// FUNC 80119988 1984 X000
// MATCHING 80119988 1984
#include "TOBJ.H"
struct V3 { int x, y, z; };
typedef struct {
    TObj o;
    unsigned short c0, c2, c4, c6, c8, ca, cc, ce, d0, d2;
} TObjE;
extern short FUN_8001fe0c(int, int);
extern short FUN_8001fe3c(int, int);
extern void FUN_80025f40(int, int, int, int);
extern void FUN_80026c50(int, int, int);
extern short FUN_8001e4f0(int);
extern void FUN_8001eaa4(int);
extern int FUN_8001f9e0(void);
extern void FUN_8011a148(int, int, int, int);
extern struct V3 D_1F800168;
extern unsigned short D_1F8001F8;
extern short D_1F8001C6;
extern struct V3 D_800A6048;
extern Fix16 *D_800A607C;
extern unsigned char D_8009C939A[];
#define D_8009C939 D_8009C939A[0]
extern unsigned char D_8009C930;
extern unsigned char D_8009C942A[];
#define D_8009C942 D_8009C942A[0]
extern unsigned char D_8009C93FA[];
#define D_8009C93F D_8009C93FA[0]
extern unsigned char D_800A6038A[];
#define D_800A6038 D_800A6038A[0]
extern unsigned char D_800A603C;
extern unsigned char D_800A603DA[];
#define D_800A603D D_800A603DA[0]
extern unsigned char D_800A603EA[];
#define D_800A603E D_800A603EA[0]
extern int D_8009C984;

#define o (&e->o)
void FUN_80119988(TObjE *e)
{
    TObj *p;
    int i;
    int t;
    int k;

    if (o->subtype == 0 || o->subtype == 2 || o->subtype == 6) {
        o->wb4 = (o->wb4 + o->wba) & 0xff;
        o->wb6 = (o->wb6 + o->wbc) & 0xff;
        o->wb8 = (o->wb8 + *(short *)&o->bbe) & 0xff;
        switch (o->step) {
        case 0:
            if (e->cc != 0) {
                o->d38 += 0x10000;
                if (--e->cc == 0) {
                    e->cc = 0x78;
                    e->d0 = o->d30 >> 16;
                    e->d2 = o->d34 >> 16;
                    o->step++;
                }
            }
            break;
        case 1:
            o->d30 += (D_1F800168.x - o->d30) >> 3;
            o->d34 += (D_1F800168.y - o->d34) >> 3;
            o->d38 += (D_1F800168.z - o->d38) >> 3;
            if (e->c0 != 0) e->c0--;
            if (e->c2 != 0) e->c2--;
            if (e->c4 != 0) e->c4 -= 2;
            if (--e->cc == 0) o->step++;
            if ((D_1F8001F8 & 7) == 0)
                FUN_80025f40(0, 0, 0xe0, 4);
            break;
        case 2:
            o->d30 += ((e->d0 << 16) - o->d30) >> 5;
            o->d34 += ((e->d2 << 16) - o->d34) >> 5;
            t = e->d0 << 16;
            if (o->d30 >= t - 0x160000 && o->d30 <= t + 0x160000) {
                D_8009C939 = 1;
                o->wb4 = 0;
                o->wb6 = 0;
                o->wb8 = 0;
                e->cc = 0x8c;
                o->step++;
            }
            goto common;
        case 3:
            o->d30 += ((e->d0 << 16) - o->d30) / 8;
            o->d34 += ((e->d2 << 16) - o->d34) / 8;
            if (e->c0 < e->c6) e->c0 += 4;
            if (e->c2 < e->c8) e->c2 += 2;
            if (e->c4 < e->ca) e->c4 += 4;
            if (--e->cc == 0) {
                FUN_80025f40(0, 1, 0xff, 0x14);
                e->c0 = 0;
                e->c2 = 0;
                e->c4 = 0;
                e->cc = 0x78;
                D_8009C939 = 0;
                o->step++;
            }
        common:
            D_800A6048 = *(struct V3 *)&o->a;
            if ((D_1F8001F8 & 7) == 0)
                FUN_80025f40(0, 0, 0xd0, 4);
            break;
        case 4:
            if (D_800A607C->p.whole > 0) {
                FUN_80025f40(0, 1, 0xff, 0x14);
                D_800A607C->p.whole -= 4;
            } else {
                if (o->subtype == 0)
                    FUN_80026c50(4, 1, 1);
                D_1F8001C6 = 0;
                *(int *)D_800A607C = 0;
                D_800A6038 = 1;
                D_800A603C = 1;
                D_8009C930 = 2;
                D_800A603D = 0;
                D_800A603E = 0;
                D_8009C942 = 0;
                D_8009C93F = 0;
                e->cc = 0x78;
                e->d0 = 0;
                o->step = 9;
            }
            *(struct V3 *)&o->d30 = D_1F800168;
            break;
        case 5:
            *(struct V3 *)&o->d30 = D_1F800168;
            if (D_8009C930 == 4)
                goto snd;
            break;
        case 6:
            FUN_8011a148(o->h->p.whole, o->y.p.whole, o->d->p.whole, 2);
            FUN_8011a148(o->h->p.whole, o->y.p.whole, o->d->p.whole, 2);
            FUN_8011a148(o->h->p.whole, o->y.p.whole, o->d->p.whole, 2);
        snd:
            e->ce = FUN_8001e4f0(0x36);
            o->step++;
            break;
        case 7:
            e->cc = 300;
            o->step++;
        case 8:
            e->c0 += (FUN_8001f9e0() & 3) - 1;
            e->c2 += (FUN_8001f9e0() & 7) - 1;
            e->c4 += FUN_8001f9e0() & 7;
            o->d30 += (FUN_8001f9e0() & 3) << 16;
            o->d34 -= ((FUN_8001f9e0() & 0xf) - 8) << 14;
            o->d64 = (e->c4 << 6) + 0x1000;
            break;
        case 9:
            *(struct V3 *)&o->d30 = D_1F800168;
            o->d64 -= 0x20;
            if (--e->cc == 0) {
                FUN_8001eaa4(e->ce);
                o->step = 5;
            }
            break;
        case 10:
            k = 6;
            if (D_8009C984 & 1) {
                o->wbc = 2;
                e->cc = 0x80;
                o->wba = k;
                *(short *)&o->bbe = k;
                D_800A6038 = 4;
                D_800A603C = 5;
                D_800A603D = 0;
                D_800A603E = 0;
                D_8009C93F = 1;
                D_8009C930 = 1;
                e->ce = FUN_8001e4f0(0x36);
                o->step = 0;
            }
            break;
        }
        o->h->raw = o->d30;
        o->y.raw = o->d34;
        o->d->raw = o->d38;
    } else {
        p = (TObj *)o->d90;
        for (i = 0; i < 12; i++)
            (&o->wb4)[i] = (&p->wb4)[i];
        o->h->raw = p->d30;
        o->y.raw = p->d34;
        o->d->raw = p->d38;
        o->d64 = p->d64;
    }
    o->h->raw += FUN_8001fe0c(o->wb4, (short)e->c0) << 16;
    o->y.raw += FUN_8001fe3c(o->wb6, (short)e->c2) << 16;
    o->d->raw += FUN_8001fe3c(o->wb8, (short)e->c4) << 16;
}
