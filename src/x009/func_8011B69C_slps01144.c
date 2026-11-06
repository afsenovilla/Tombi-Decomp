// VERSION SLPS_01144 x09 8011B69C 760
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
typedef struct { short lo, hi; } HL;
typedef union { int i; HL s; } FX;
typedef struct { FX x, y, z; } V3;
extern unsigned char D_8009ECDE;
extern unsigned char D_800A7C1A;
extern unsigned int D_8009E514;
extern unsigned char D_8012A3CC[];
extern int ObjCullRegister(TObj *);
extern void func_8011B230_slps01144(TObj *);
extern void func_8011B564_slps01144(TObj *);
extern void setEventStarted(int, int, int);
extern int func_8001C988(int, int);
extern void spawnItemFromEntry(unsigned char *, V3 *, int, int, TObj *);
extern void ObjFreeDup(TObj *);
#define PTR(o, off) ((TObj *)*(int *)((char *)(o) + (off)))

void func_8011B69C_slps01144(TObj *o)
{
    unsigned char s = o->b04;
    V3 v;
    switch (s) {
    case 0:
        if (D_8009ECDE) {
            o->b04 = 3;
            break;
        }
        o->b04 = s + 1;
        o->b0a = 0x13;
        *(int *)((char *)o + 0x84) = 0;
        *(int *)((char *)o + 0x88) = 0;
        o->d8c = 0;
        *(short *)((char *)o + 0x74) = 0xc00;
        *(short *)((char *)o + 0x76) = 0xc00;
        *(short *)((char *)o + 0x78) = 0xc00;
        o->b69 = 0;
        o->timer = 0x20;
        if (o->b0c == 0) {
            o->box0 = 18;
            o->box1 = 36;
            o->box2 = 36;
            o->box3 = 36;
        } else {
            o->active = 2;
        }
        break;
    case 1:
        ObjCullRegister(o);
        if (o->b0c == 0) {
            func_8011B230_slps01144(o);
            switch (o->step) {
            case 0:
                if (o->b69 == 1 && D_800A7C1A == 1)
                    o->step++;
                break;
            case 1:
                if (D_8009E514 <= 149999) {
                    setEventStarted(0x10, 0, 1);
                    o->step--;
                } else {
                    func_8001C988(0x10, 1);
                    o->b04 = 2;
                    o->step = 0;
                }
                break;
            }
            o->b69 = 0;
        } else {
            o->b04 = PTR(o, 0x90)->b04;
            o->step = PTR(o, 0x90)->step;
            o->state = PTR(o, 0x90)->state;
        }
        break;
    case 2:
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            if (o->b0c == 0) {
                v.x.s.hi = o->h->p.whole;
                v.y.s.hi = o->y.p.whole - 32;
                v.z.s.hi = o->d->p.whole;
                spawnItemFromEntry(D_8012A3CC, &v, 0, -0x400, o);
                PTR(o, 0xb4)->b04 = 2;
                PTR(o, 0xb8)->b04 = 2;
                PTR(o, 0xbc)->b04 = 2;
                PTR(o, 0xc0)->b04 = 2;
                PTR(o, 0xc4)->b04 = 2;
                PTR(o, 0xc8)->b04 = 2;
            }
            o->step++;
            break;
        case 1:
            func_8011B564_slps01144(o);
            break;
        case 2:
            o->b04 = 3;
            break;
        }
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
#endif
