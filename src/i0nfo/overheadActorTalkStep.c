// FUNC 800ece34 228 I0NFO
// MATCHING 800ece34 228
#include "TOBJ.H"
typedef struct { short s[6]; } Pos6;
extern void func_8001FEC0(TObj *o);
extern int func_8002DCF0(int a, int b, Pos6 *p, int t);
extern unsigned char D_8009C93F[];

void overheadActorTalkStep(TObj *o)
{
    Pos6 p;
    switch (o->state) {
    case 0:
        p = *(Pos6 *)&o->a;
        p.s[3] -= 0x40;
        o->d90 = func_8002DCF0(o->w98, o->w9a, &p, o->timer);
        o->state++;
        break;
    case 1:
        func_8001FEC0(o);
        if (--o->timer == 0) {
            D_8009C93F[0] = 0;
            o->b6a = 0;
            o->step = 0;
            o->state = 0;
        }
        break;
    }
}
