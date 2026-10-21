// FUNC 800fa424 408 I0NFO2
// MATCHING 800fa424 408
#include "TOBJ.H"
extern void func_8005A8A8(int a, int b, int c);
extern void func_800F7600(TObj *o);
extern void func_800F80D8(TObj *o);
extern void func_800F88A0(TObj *o);
extern void func_800F8D5C(TObj *o);
extern void func_800F8F30(TObj *o);
extern void func_800F91D8(TObj *o);
extern void func_800F9480(TObj *o);
extern void func_800F969C(TObj *o);
extern void func_800F9974(TObj *o);
extern void func_800F9BDC(TObj *o);
extern void func_800FA278(TObj *o);
extern unsigned char D_8009C93A;
extern unsigned char D_8009CDD9;
extern unsigned char D_8009CF2A[];
extern unsigned char D_800A60A3;

void stepNpcSet07(TObj *o)
{
    if (D_800A60A3 != 1 && D_8009CF2A[0] == 1) D_8009CF2A[0] = 2;
    switch (o->step) {
    case 0: func_800F9974(o); break;
    case 1: func_800F7600(o); break;
    case 2: func_800F80D8(o); break;
    case 3: func_800F88A0(o); break;
    case 4: func_800F8F30(o); break;
    case 5: func_800F91D8(o); break;
    case 6: func_800F8D5C(o); break;
    case 8: func_800F9BDC(o); break;
    case 9:
        switch (o->state) {
        case 0:
            if (D_8009CDD9 == 0 && D_8009C93A != 0) {
                func_8005A8A8(0x35, 0, 0);
                o->state++;
            }
            break;
        case 1:
            o->step = 0;
            o->state = 0;
            break;
        }
        break;
    case 10: func_800FA278(o); break;
    case 11: func_800F9480(o); break;
    case 12: func_800F969C(o); break;
    }
}
