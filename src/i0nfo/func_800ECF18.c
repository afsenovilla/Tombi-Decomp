// FUNC 800ecf18 192 I0NFO
// MATCHING 800ecf18 192
#include "TOBJ.H"
extern void func_8001FEC0(TObj *o);
extern signed char D_8009C93F[];
extern signed char D_8009C942[];
extern signed char D_800A603D;
extern signed char D_800A603E;
extern signed char D_800A60E4[];

void func_800ECF18(TObj *o)
{
    TObj *p;
    int pad[4];
    switch (o->state) {
    case 0:
        o->state++;
    case 1:
        func_8001FEC0(o);
        p = (TObj *)o->d90;
        if (p->b04 == 2) {
            p->b04 = 3;
            D_8009C942[0] = 0;
            D_8009C93F[0] = 0;
            D_800A60E4[0] = 0;
            if (!(o->w74 & 0x80)) {
                D_800A603D = 0;
                D_800A603E = 0;
            }
            o->b68 = 0;
            o->b6a = 0;
            o->step = 0;
            o->state = 0;
        }
        break;
    }
}
