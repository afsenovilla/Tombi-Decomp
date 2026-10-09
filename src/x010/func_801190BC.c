// FUNC 801190bc 696 X010
// MATCHING 801190bc 696
#include "TOBJ.H"

extern unsigned char D_8009C93C, D_8009C93F, D_8009C940, D_8009C941;
extern unsigned char D_800A603C, D_800A603D, D_800A603E;
extern short D_8009CD94, D_8009CD96, D_8009CDA0;
extern unsigned char D_8009C975;
extern TObj *D_1F8001D4;
extern int ObjCullRegister(TObj *);
extern void ObjFreeDup(TObj *);
extern void FUN_8005a9a4(int, int);

void func_801190BC(TObj *o)
{
    unsigned char t = o->b04;
    TObj *p;

    switch (t) {
    case 0:
        o->b04 = t + 1;
        o->box0 = 0x18;
        o->box1 = 0x30;
        o->box2 = 8;
        o->box3 = 0x10;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->b69 = 0;
        o->active = 1;
        o->velH = 0;
        break;
    case 1:
        switch (o->step) {
        case 0:
            if (D_8009C940 == 0) break;
            if (D_8009C941 != 0x5c) break;
            o->timer = 0x3c;
            o->step++;
            ObjCullRegister(o);
            D_8009C93F = 1;
            break;
        case 1:
            if (--o->timer == -1) {
                o->step++;
                D_800A603C = 1;
                D_800A603D = 0x12;
                D_800A603E = 0;
            }
            ObjCullRegister(o);
            break;
        case 2:
            if (o->b69) {
                o->step++;
                FUN_8005a9a4(0x96, 0);
            }
            ObjCullRegister(o);
            break;
        case 3:
            o->velH += 8;
            if (o->velH > 0x200) o->velH = 0x200;
            o->h->raw += o->velH << 8;
            if (o->h->p.whole >= 0x385) {
                o->step++;
                D_8009CD94 = 4;
                D_8009CDA0 = 0xb;
                D_8009CD96 = 0;
                D_8009C93C = 0;
                D_8009C975 = 3;
            }
            ObjCullRegister(o);
            break;
        case 4:
            if (D_8009C975 == 1) {
                p = D_1F8001D4;
                p->w4c = 7;
                p->w4e = 0;
            }
            o->h->p.whole += 2;
            ObjCullRegister(o);
            break;
        }
        break;
    case 2:
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
