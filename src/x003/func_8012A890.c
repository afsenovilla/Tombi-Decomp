// FUNC 8012a890 716 X003
// MATCHING 8012a890 716
#include "TOBJ.H"

extern unsigned char D_8009D004, D_8009CDC7, D_8009C940, D_8009C941, D_8009C975, D_8009D094;
extern unsigned char D_800A603C, D_800A603D, D_8009C93F;
extern unsigned char D_1F8003D0, D_1F8001CC, D_1F8001CD;
extern short D_1F8001C6;
extern short D_8009CD94, D_8009CD96, D_8009CDA0;
extern TObj *D_1F8001D4;
extern char D_8001D6A4[];
extern int ObjCullRegister(TObj *o);
extern void FUN_80018790(TObj *o);
extern void FUN_8001d63c(int);
extern void stopBgm(int);
extern void ThreadCreate(int, void *);
extern void removeItemFromInventory(int, int);
extern void func_8012A6D8(TObj *o);
extern void func_80129CC0(TObj *o);

static __inline__ int chk(void)
{
    return D_8009C940 == 1 && D_8009C941 == 0x42;
}

void func_8012A890(TObj *o)
{
    TObj *p;

    switch (o->b04) {
    case 0:
        func_8012A6D8(o);
        break;
    case 1:
        ObjCullRegister(o);
        if (o->subtype) break;
        switch (o->step) {
        case 0:
            if ((D_8009D004 && D_8009CDC7 == 0xff) || chk()) {
                D_8009D004 = 0;
                if (D_8009CDC7 != 0xff) removeItemFromInventory(0x42, 5);
                o->step++;
                o->state = 0;
            }
            break;
        case 1:
            func_80129CC0(o);
            break;
        }
        break;
    case 2:
        switch (o->step) {
        case 0:
            o->step++;
            o->state = 0;
            FUN_8001d63c(7);
            break;
        case 1:
            switch (o->state) {
            case 0:
                D_800A603C = 5;
                D_800A603D = 0x62;
                D_8009C93F = 1;
                D_8009C975 = 3;
                o->state++;
                break;
            case 1:
                if (D_8009C975 == 1) {
                    if (D_8009D094 == 0) {
                        D_1F8003D0 = 0;
                        D_8009D094 = 1;
                    } else {
                        D_1F8003D0 = 1;
                    }
                    D_1F8001CC = 1;
                    D_1F8001CD = 7;
                    D_1F8001C6 = 2;
                    stopBgm(0);
                    ThreadCreate(1, D_8001D6A4);
                    o->step++;
                    o->state = 0;
                }
                break;
            }
            break;
        case 2:
            D_8009CD94 = 10;
            D_8009CD96 = 0;
            D_8009CDA0 = 0;
            p = D_1F8001D4;
            p->w4c = 7;
            p->w4e = 0;
            break;
        }
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
