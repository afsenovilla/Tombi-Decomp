// FUNC 801200ec 1008 X001
// MATCHING 801200ec 1008
#include "TOBJ.H"

extern unsigned char D_8009D084, D_8009CDF1[], D_800A4555[];
extern unsigned char D_8009D084x[]; /* debt: second name, keeps the load after the b68 store */
extern char *D_1F800334;
extern char *D_1F800334x[]; /* debt: second name, the game loads the pointer twice */
extern int ObjCullRegister(TObj *);
extern TObj *ObjAlloc(void);
extern void FUN_8005a8a8(int, int, int);
extern void FUN_80018838(TObj *);

void func_801200EC(TObj *o)
{
    unsigned char s;
    TObj *e;

    switch (o->b04) {
    case 0:
        switch (o->subtype) {
        case 0:
            o->b04++;
            o->box0 = 0x24;
            o->box1 = 0x48;
            o->box2 = 0x2b;
            o->box3 = 0x57;
            o->active = 1;
            o->b68 = 0;
            if (D_8009D084x[0] || D_8009CDF1[0] == 0xff) {
                o->da0 = (int)D_1F800334x[0] + *(int *)(D_1F800334 + 0x34);
            }
            break;
        case 1:
        case 2:
            o->b04++;
            o->active = 2;
            o->b0f = 8;
            if (D_8009CDF1[0] == 0xff) o->y.p.whole += 0x10;
            else o->y.p.whole += 0x76;
            break;
        case 3:
            o->b04++;
            o->b0a = 0x11;
            o->d88 = 0x40;
            o->active = 2;
            o->b0f = 8;
            o->y.p.whole += 0x10;
            break;
        }
        break;
    case 1:
        ObjCullRegister(o);
        switch (o->subtype) {
        case 0:
            s = o->step;
            if (s != 0) break;
            if (D_8009D084 || D_8009CDF1[0] == 0xff) {
                o->step = s + 1;
                break;
            }
            if (o->b68 == 0) break;
            FUN_8005a8a8(0x18, 0, 1);
            D_8009D084 = 1;
            o->da0 = (int)D_1F800334x[0] + *(int *)(D_1F800334 + 0x34);
            e = ObjAlloc();
            if (e != 0) {
                e->type = 0x3d;
                e->active = 1;
                e->subtype = 0;
                e->timer = 0x18;
                e->h->raw = o->h->raw;
                e->y.raw = o->y.raw;
                e->d->raw = o->d->raw;
            }
            o->b68 = 0;
            o->step++;
            break;
        case 1:
        case 2:
            s = o->step;
            switch (s) {
            case 0:
                if (D_800A4555[0] == 4) {
                    o->b0f = 0;
                    o->step++;
                }
                break;
            case 1:
                if (D_800A4555[0] == 7) o->step = s + 1;
                break;
            case 2:
                o->y.raw += 0x18000;
                break;
            }
            break;
        case 3:
            s = o->step;
            switch (s) {
            case 0:
                if (D_800A4555[0] == 4) o->step = s + 1;
                break;
            case 1:
                o->d->p.whole -= 2;
                if (o->d->p.whole < 0x1037) o->step++;
                break;
            case 2:
                break;
            }
            break;
        }
        break;
    case 2:
        break;
    case 3:
        FUN_80018838(o);
        break;
    }
}
